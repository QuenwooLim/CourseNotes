const cloud = require('../../utils/cloud')

Page({
  data: {
    taskId: '',
    task: null,
    userInfo: null,
    loading: true,
    isCreator: false,
    hasJoined: false,
    hasCheckedIn: false,
    isExpired: false,
    isNotStarted: false,
    checkinRecord: null,
    debugInfo: '加载中...'
  },

  // 处理导航栏返回按钮点击
  onBackTap: function() {
    wx.navigateBack({
      delta: 1
    })
  },

  onLoad: function(options) {
    console.log('任务详情页面加载，参数:', options)

    if (!cloud || !cloud.task) {
      console.error('cloud模块未正确加载')
      this.setData({
        debugInfo: 'cloud模块未正确加载'
      })
      wx.showToast({
        title: '系统错误，请重试',
        icon: 'none'
      })
      return
    }
    
    if (options.id) {
      this.setData({
        taskId: options.id
      })
      
      // 加载用户信息
      this.loadUserInfo()
      
      // 加载任务详情
      this.loadTaskDetail(options.id)
    } else {
      wx.showToast({
        title: '任务ID不存在',
        icon: 'none'
      })
      setTimeout(() => {
        wx.navigateBack()
      }, 1500)
    }
  },

  // 页面显示时刷新数据
  onShow: function() {
    // 获取全局数据
    const app = getApp();
    const taskDataChanged = app.globalData.taskDataChanged;
    const lastTaskId = app.globalData.lastTaskId;
    
    // 如果数据变更且涉及当前任务，重新加载任务详情
    if (taskDataChanged && this.data.taskId && (lastTaskId === this.data.taskId || !lastTaskId)) {
      console.log('检测到任务数据变更，刷新任务详情');
      this.loadTaskDetail(this.data.taskId);
    }
    
    // 如果已经加载了任务ID，重新检查打卡状态
    if (this.data.taskId) {
      this.checkCheckinStatus(this.data.taskId);
    }
  },

  // 加载用户信息
  loadUserInfo: function() {
    const app = getApp()
    let userInfo = app.globalData.userInfo
    
    if (!userInfo) {
      userInfo = wx.getStorageSync('userInfo') || null
      if (userInfo) {
        app.globalData.userInfo = userInfo
      }
    }
    
    this.setData({ userInfo })
  },

  // 加载任务详情
  loadTaskDetail: function(taskId) {
    wx.showLoading({
      title: '加载中...'
    })
    
    console.log('正在加载任务详情，ID:', taskId)
    
    wx.cloud.callFunction({
      name: 'task',
      data: {
        type: 'detail',
        taskId: taskId,
        includeParticipantDetails: true 
      }
    })
    .then(res => {
      console.log('任务详情云函数返回:', res)
      
      if (res.result && res.result.code === 0 && res.result.data) {
        const task = res.result.data
        
        // 添加字段兼容性处理
        task.startDateTime = task.startDateTime || task.startTime || ''
        task.endDateTime = task.endDateTime || task.endTime || ''
        task.deadline = task.deadline || task.endDateTime // 使用endDateTime作为deadline
        
        const userInfo = this.data.userInfo
        const isCreator = userInfo && userInfo._id === task.createdBy
        
        // 判断用户是否已参与任务
        let hasJoined = false;
        if (userInfo && userInfo._id) {
          if (task.participants && task.participants.length > 0 && typeof task.participants[0] === 'object') {
            hasJoined = task.participants.some(p => p._id === userInfo._id);
          } else {
            hasJoined = task.participants && task.participants.includes(userInfo._id);
          }
        }
        
        // 格式化创建时间
        if (task.createTime) {
          const date = new Date(task.createTime)
          task.createTimeFormatted = `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')} ${String(date.getHours()).padStart(2, '0')}:${String(date.getMinutes()).padStart(2, '0')}`
        }
        
        // 检查任务是否已过期 (使用 endDateTime 或 deadline)
        let isExpired = false;
        let isNotStarted = false;
        const now = new Date();
        
        // 检查任务是否尚未开始
        if (task.startDateTime) {
          try {
            isNotStarted = new Date(task.startDateTime.replace(/-/g, '/')) > now; // 兼容iOS日期格式
            console.log(`任务开始时间: ${task.startDateTime}, 是否未开始: ${isNotStarted}`);
          } catch (e) {
            console.error('解析 startDateTime 出错:', e);
          }
        }
        
        // 检查任务是否已过期
        if (task.endDateTime) {
          try {
            isExpired = new Date(task.endDateTime.replace(/-/g, '/')) < now; // 兼容ios日期格式
          } catch (e) {
            console.error('解析 endDateTime 出错:', e);
            // 尝试使用 deadline
            if (task.deadline) {
              try {
                isExpired = new Date(task.deadline.replace(/-/g, '/')) < now;
              } catch (e2) {
                console.error('解析 deadline 出错:', e2);
              }
            }
          }
        } else if (task.deadline) {
           try {
             isExpired = new Date(task.deadline.replace(/-/g, '/')) < now;
           } catch (e) {
             console.error('解析 deadline 出错:', e);
           }
        }
        console.log(`任务过期状态: ${isExpired}, 截止时间: ${task.endDateTime || task.deadline}`);

        this.setData({
          task,
          loading: false,
          isCreator,
          hasJoined,
          isExpired,
          isNotStarted,
          debugInfo: '数据加载成功'
        })

        // 如果已登录且参与了任务，检查打卡状态
        if (userInfo && userInfo._id) { 
          this.checkCheckinStatus(taskId)
        }
      } else {
        const errorMsg = (res.result && res.result.message) || '获取任务详情失败'
        console.error('云函数返回错误:', errorMsg)
        this.setData({ loading: false, debugInfo: '云函数返回错误: ' + errorMsg })
        wx.showToast({ title: errorMsg, icon: 'none' })
      }
      wx.hideLoading()
    })
    .catch(err => {
      console.error('调用云函数失败:', err)
      this.setData({ loading: false, debugInfo: '调用云函数失败: ' + (err.message || JSON.stringify(err)) })
      wx.hideLoading()
      wx.showToast({ title: err.message || '获取任务详情失败', icon: 'none' })
    })
  },

  // 检查用户是否已打卡 (只更新状态，不影响按钮逻辑判断)
  checkCheckinStatus: function(taskId) {
    if (!this.data.userInfo || !this.data.userInfo._id) return

    const today = new Date()
    const dateStr = `${today.getFullYear()}-${String(today.getMonth() + 1).padStart(2, '0')}-${String(today.getDate()).padStart(2, '0')}`

    wx.cloud.callFunction({
      name: 'checkin',
      data: { type: 'checkStatus', taskId, date: dateStr, userId: this.data.userInfo._id }
    })
    .then(res => {
      if (res.result && res.result.code === 0) {
        this.setData({
          hasCheckedIn: res.result.data.hasCheckedIn,
          checkinRecord: res.result.data.record || null
        })
      } else {
         // 检查失败或未打卡，确保状态正确
         this.setData({ hasCheckedIn: false, checkinRecord: null })
      }
    })
    .catch(err => {
      console.error('检查打卡状态失败:', err)
      this.setData({ hasCheckedIn: false, checkinRecord: null }) // 出错时也认为未打卡
    })
  },

  // 参与任务并直接跳转到打卡页
  joinTask: function() {
    if (!this.data.userInfo || !this.data.userInfo._id) {
      wx.showToast({ title: '请先登录', icon: 'none' })
      return
    }
    if (this.data.isExpired) {
      wx.showToast({ title: '任务已截止', icon: 'none' })
      return
    }
    if (this.data.isNotStarted) {
      wx.showToast({ title: '任务尚未开始', icon: 'none' })
      return
    }
    
    // 显示加载提示
    wx.showLoading({
      title: '处理中...',
      mask: true
    })

    // 不再先调用云函数将用户加入任务
    // 而是直接跳转到打卡页面，让用户完成打卡后再加入任务
    // 修改导航方式
    wx.hideLoading()
    
    // 设置全局任务数据变更标记，以便列表页等刷新
    const app = getApp();
    app.globalData.taskDataChanged = true;
    app.globalData.lastTaskId = this.data.taskId;

    // 导航到打卡页面，带上join=1参数表示这是首次参与模式
    wx.navigateTo({
      url: `/pages/checkin/checkin?id=${this.data.taskId}&join=1`,
      fail: (err) => {
        console.error('跳转到打卡页失败:', err);
        wx.showToast({ title: '页面跳转失败', icon: 'none' });
      }
    });
  },

  // 新增：查看打卡详情
  viewCheckin: function() {
    if (!this.data.hasCheckedIn || !this.data.checkinRecord) {
      wx.showToast({ title: '您尚未打卡', icon: 'none' })
      return
    }
    wx.navigateTo({
      url: `/pages/checkin/checkin?id=${this.data.taskId}&view=1&recordId=${this.data.checkinRecord._id}`
    })
  },

  // 返回列表
  goBack: function() {
    wx.navigateBack()
  },
  
  // 查看参与者打卡内容
  viewParticipantCheckin: function(e) {
    const userId = e.currentTarget.dataset.userId;
    const userName = e.currentTarget.dataset.userName;
    const taskId = this.data.taskId; // 获取当前任务ID
    
    if (!userId || !taskId) {
      wx.showToast({
        title: '信息不完整',
        icon: 'none'
      });
      return;
    }
    
    wx.showLoading({
      title: '查询中...'
    });
    
    // 调用云函数获取该用户的最后一次打卡记录
    wx.cloud.callFunction({
      name: 'checkin',
      data: {
        type: 'getUserLastCheckin', // 使用新的操作类型
        taskId: taskId,
        userId: userId
      }
    }).then(res => {
      wx.hideLoading();
      console.log('获取用户最后一次打卡记录:', res);

      if (res.result && res.result.code === 0 && res.result.data.record) {
        // 如果找到了记录，跳转到打卡详情页 (查看模式)
        wx.navigateTo({
          url: `/pages/checkin/checkin?id=${taskId}&view=1&recordId=${res.result.data.record._id}`
        });
      } else {
        // 如果没有找到记录，提示用户
        wx.showToast({
          title: userName + ' 尚未打卡此任务',
          icon: 'none'
        });
      }
    }).catch(err => {
      wx.hideLoading();
      console.error('查询用户打卡记录失败:', err);
      wx.showToast({
        title: '查询失败，请重试',
        icon: 'none'
      });
    });
  },

  onShareAppMessage: function () {
    // ... existing code ...
  }
}) 