// 云开发工具类
const cloud = {
  // 通用云函数调用方法（带重试）
  callFunction: function(name, data, maxRetries = 1) {
    let retries = 0;
    
    const call = function() {
      return new Promise((resolve, reject) => {
        console.log(`调用云函数: ${name}, 参数:`, data);
        wx.cloud.callFunction({
          name: name,
          data: data
        })
        .then(res => {
          // 验证返回结果
          if (res.result) {
            // 检查是否包含错误码
            if (res.result.code !== undefined && res.result.code !== 0) {
              console.error(`云函数 ${name} 返回错误码:`, res.result.code, res.result.message || '');
              reject(new Error(res.result.message || `错误码: ${res.result.code}`));
              return;
            }
            
            // 记录结果到控制台，方便调试
            console.log(`云函数 ${name} 返回结果:`, res.result);
            
            // 返回数据对象，确保能获取到结果的data字段
            if (res.result.data !== undefined) {
              resolve(res.result.data);
            } else {
              // 如果没有data字段，但有其它内容，直接返回result
              resolve(res.result);
            }
          } else {
            console.warn(`云函数 ${name} 返回空结果`);
            resolve(null);
          }
        })
        .catch(err => {
          console.error(`云函数 ${name} 调用异常:`, err);
          if (retries < maxRetries) {
            retries++;
            console.log(`重试 ${name} (${retries}/${maxRetries})...`);
            // 等待一段时间再重试
            setTimeout(() => {
              call().then(resolve).catch(reject);
            }, 300);
          } else {
            // 达到最大重试次数，返回错误
            reject(err);
          }
        });
      });
    };
    
    return call();
  },

  // 任务相关
  task: {
    // 创建任务
    createTask: function(data) {
      // 额外检查确保日期时间字段存在
      if (!data.startDateTime || !data.endDateTime) {
        return Promise.reject(new Error('起始时间和截止时间必须设置'));
      }
      
      // 创建一个新的数据对象，将 startDateTime 映射为 startTime，endDateTime 映射为 endTime
      const taskData = {
        ...data,
        startTime: data.startDateTime,
        endTime: data.endDateTime
      };
      
      return module.exports.callFunction('task', {
        type: 'create',
        data: taskData,
        userId: getApp().globalData.userInfo._id
      }, 1).then(result => {
        // 设置任务数据变更标记
        const app = getApp();
        app.globalData.taskDataChanged = true;
        
        // 保存最后操作的任务ID
        if (result && result._id) {
          app.globalData.lastTaskId = result._id;
        } else if (result && result.id) { 
          // 兼容可能的不同返回格式
          app.globalData.lastTaskId = result.id;
        }
        
        console.log('任务创建成功，ID:', app.globalData.lastTaskId);
        return result;
      });
    },

    // 获取任务列表
    getTaskList: function(page = 1, pageSize = 10) {
      return module.exports.callFunction('task', {
        type: 'list',
        page,
        pageSize
      }, 1);
    },
    
    // 获取任务详情
    getTaskDetail: function(taskId) {
      return module.exports.callFunction('task', {
        type: 'detail',
        taskId: taskId
      }, 1);
    },
    
    // 加入任务
    joinTask: function(taskId, userId) {
      if (!taskId) {
        return Promise.reject(new Error('任务ID不能为空'));
      }
      
      return module.exports.callFunction('task', {
        type: 'join',
        taskId,
        userId: userId || getApp().globalData.userInfo._id
      }, 1);
    },
    
    // 获取所有任务
    getAllTasks: function() {
      return module.exports.callFunction('task', {
        type: 'getAll'
      }, 1);
    },
    
    // 更新任务
    updateTask: function(data) {
      // 额外检查确保日期时间字段存在
      if (!data.startDateTime || !data.endDateTime) {
        return Promise.reject(new Error('起始时间和截止时间必须设置'));
      }
      
      // 创建一个新的数据对象，将 startDateTime 映射为 startTime，endDateTime 映射为 endTime
      const taskData = {
        ...data,
        startTime: data.startDateTime,
        endTime: data.endDateTime
      };
      
      return module.exports.callFunction('task', {
        type: 'update',
        data: taskData,
        userId: getApp().globalData.userInfo._id
      }, 0);
    },
    
    // 删除任务
    deleteTask: function(taskId) {
      return module.exports.callFunction('task', {
        type: 'delete',
        taskId: taskId,
        userId: getApp().globalData.userInfo._id
      }, 0).then(result => {
        // 设置任务数据变更标记
        const app = getApp();
        app.globalData.taskDataChanged = true;
        app.globalData.lastTaskId = null; // 删除任务时清空最后任务ID
        return result;
      });
    },
    
    // 获取时间周期选项
    getTimePeriods: function() {
      return module.exports.callFunction('task', {
        type: 'getTimePeriods'
      }, 1).then(res => {
        // 处理返回的对象格式
        if (res && typeof res === 'object' && (res.WEEKS || res.TRAINING)) {
          console.log('返回的时间周期数据:', res);
          return res; // 直接返回对象格式，让页面处理
        } else {
          console.warn('时间周期数据格式不正确:', res);
          // 如果格式不对，返回默认格式
          return {
            WEEKS: ['第1周', '第2周', '第3周', '第4周', '第5周', '第6周', '第7周', '第8周', '第9周', '第10周', 
                  '第11周', '第12周', '第13周', '第14周', '第15周', '第16周', '第17周', '第18周'],
            TRAINING: ['寒训', '暑训']
          };
        }
      });
    },
    
    // 获取打卡类型选项
    getCheckinTypes: function() {
      return module.exports.callFunction('task', {
        type: 'getCheckinTypes'
      }, 1).then(res => {
        // 处理返回的数组格式
        if (res && Array.isArray(res)) {
          console.log('返回的打卡类型数据:', res);
          return res;
        } else {
          console.warn('打卡类型数据格式不正确:', res);
          return ['非集训', '集训上午', '集训下午']; // 如果格式不对，返回默认值
        }
      });
    }
  },

  // 打卡相关
  checkin: {
    // 提交打卡
    submitCheckin: function(data) {
      const userId = getApp().globalData.userInfo._id;
      return module.exports.callFunction('checkin', {
        type: 'submit',
        data: data,
        userId
      }, 1).then(result => {
        // 设置任务数据变更标记
        const app = getApp();
        app.globalData.taskDataChanged = true;
        if (data && data.taskId) {
          app.globalData.lastTaskId = data.taskId;
        }
        return result;
      });
    },

    // 获取排行榜
    getRanking: function(type = 'all', filters = {}) {
      return module.exports.callFunction('checkin', {
        type: 'ranking',
        rankingType: type,
        filters: filters
      }, 1);
    },

    // 获取用户统计数据
    getUserStats: function() {
      const userId = getApp().globalData.userInfo._id;
      return module.exports.callFunction('checkin', {
        type: 'userStats',
        userId
      }, 2);  // 统计数据允许重试2次
    },

    // 获取用户打卡历史
    getUserHistory: function(page = 1, pageSize = 10) {
      const userId = getApp().globalData.userInfo._id;
      return module.exports.callFunction('checkin', {
        type: 'userHistory',
        userId,
        page,
        pageSize
      }, 1);
    },
    
    // 获取打卡记录详情
    getCheckinDetail: function(recordId) {
      const userId = getApp().globalData.userInfo ? getApp().globalData.userInfo._id : '';
      
      // 如果用户未登录，显示提示
      if (!userId) {
        wx.showToast({
          title: '请先登录后查看',
          icon: 'none'
        });
        return Promise.reject({ message: '用户未登录' });
      }
      
      return module.exports.callFunction('checkin', {
        type: 'getRecord',
        recordId,
        userId
      }, 1);
    }
  },

  // 用户相关
  user: {
    // 创建或更新用户
    createUser: function(userData) {
      // 确保userData中包含需要的额外字段
      const data = {
        ...userData,
        // 基本信息
        nickName: userData.nickName || '',
        avatarUrl: userData.avatarUrl || '',
        studentId: userData.studentId || '',
        gender: userData.gender || 'male',
        college: userData.college || '',
        grade: userData.grade || '',
        
        // 队员信息
        teamStatus: userData.teamStatus || '在训',
        paddleSide: userData.paddleSide || '',
        competitions: userData.competitions || [],
        
        // 个人详细信息
        birthday: userData.birthday || '',
        joinDate: userData.joinDate || '',
        weight: userData.weight || '',
        height: userData.height || '',
        testLevel: userData.testLevel || '',
        phone: userData.phone || ''
      };
      
      return module.exports.callFunction('user', {
        type: 'create',
        data: data
      }, 1);
    },

    // 获取用户信息
    getUser: function(userId) {
      return module.exports.callFunction('user', {
        type: 'get',
        userId: userId
      }, 1);
    },
    
    // 更新用户信息
    updateUser: function(userData) {
      return module.exports.callFunction('user', {
        type: 'update',
        data: userData
      }, 1);
    }
  }
};

module.exports = cloud 