const cloud = require('../../utils/cloud')

// 日期时间选择器辅助函数
function getDateTimePickerArray() {
  // 获取当前时间和未来两年的范围
  const now = new Date();
  const years = [];
  const months = [];
  const days = [];
  const hours = [];
  const minutes = [];
  
  // 年份范围：今年和之后的5年
  for (let i = now.getFullYear(); i <= now.getFullYear() + 1; i++) {
    years.push(i + '年');
  }
  
  // 月份范围：1-12月
  for (let i = 1; i <= 12; i++) {
    months.push(i + '月');
  }
  
  // 日期范围：1-31日（后续会根据年月动态调整）
  for (let i = 1; i <= 31; i++) {
    days.push(i + '日');
  }
  
  // 小时范围：0-23时
  for (let i = 0; i < 24; i++) {
    hours.push(i + '时');
  }
  
  // 分钟范围：0-59分，每5分钟一个选项
  for (let i = 0; i < 60; i += 5) {
    minutes.push(i + '分');
  }
  
  // 获取当前时间对应的索引
  const currentYearIndex = 0; // 始终从当前年开始
  const currentMonthIndex = now.getMonth(); // 当前月份索引
  const currentDayIndex = now.getDate() - 1; // 当前日期索引
  const currentHourIndex = now.getHours(); // 当前小时索引
  const currentMinuteIndex = Math.floor(now.getMinutes() / 5); // 当前分钟索引（取整到5分钟）
  
  return {
    data: [years, months, days, hours, minutes],
    indexs: [currentYearIndex, currentMonthIndex, currentDayIndex, currentHourIndex, currentMinuteIndex]
  };
}

// 格式化日期时间字符串
function formatDateTime(year, month, day, hour, minute) {
  // 去掉年月日时分的单位，并补零
  year = year.replace('年', '');
  month = month.replace('月', '').padStart(2, '0');
  day = day.replace('日', '').padStart(2, '0');
  hour = hour.replace('时', '').padStart(2, '0');
  minute = minute.replace('分', '').padStart(2, '0');
  
  return `${year}-${month}-${day} ${hour}:${minute}`;
}

Page({
  data: {
    form: {
      title: '',
      description: '',
      timePeriod: '',   // 时间周期选项
      checkinType: '',  // 打卡类型
      startDateTime: '', // 打卡起始时间（完整日期时间）
      endDateTime: '',   // 打卡截止时间（完整日期时间）
      requirements: '',
      remark: ''
    },
    userInfo: null,
    loading: false,
    isEdit: false,
    taskId: null,
    // 时间周期选项
    timePeriodOptions: [],
    timePeriodIndex: 0,
    // 打卡类型选项
    checkinTypes: [],
    checkinTypeIndex: 0,
    // 日期时间选择器数据
    startDateTimeArray: {data: [], indexs: []},
    endDateTimeArray: {data: [], indexs: []},
    // 滚动相关
    scrollTop: 0
  },

  onLoad: function(options) {
    // 初始化日期时间选择器
    const dateTimeArray = getDateTimePickerArray();
    this.setData({
      startDateTimeArray: dateTimeArray,
      endDateTimeArray: JSON.parse(JSON.stringify(dateTimeArray))
    });
    
    // 检查云函数模块是否正确加载
    console.log('cloud module:', cloud)
    if (!cloud || !cloud.task) {
      console.error('cloud module not properly loaded')
      wx.showToast({
        title: '加载失败，请重试',
        icon: 'none'
      })
      return
    }
    
    // 加载时间周期选项和打卡类型选项
    this.loadTimePeriods()
    this.loadCheckinTypes()
    
    // 检查是否是编辑模式
    if (options.id && options.edit === '1') {
      this.setData({
        isEdit: true,
        taskId: options.id
      })
      
      // 加载任务详情
      this.loadTaskDetail(options.id)
    }
    
    this.loadUserInfo()
  },

  onShow: function() {
    this.loadUserInfo()
    this.checkLogin()
  },

  // 加载时间周期选项
  loadTimePeriods: function() {
    wx.showLoading({
      title: '加载中...'
    })
    
    cloud.task.getTimePeriods()
      .then(data => {
        console.log('获取时间周期选项成功:', data)
        
        // 合并所有时间周期选项
        const allOptions = [...data.WEEKS, ...data.TRAINING]
        
        this.setData({
          timePeriodOptions: allOptions
        })
        
        wx.hideLoading()
      })
      .catch(err => {
        console.error('获取时间周期选项失败:', err)
        wx.hideLoading()
        
        // 使用默认选项
        const defaultOptions = ['第1周', '第2周', '第3周', '寒训', '暑训']
        this.setData({
          timePeriodOptions: defaultOptions
        })
      })
  },

  // 加载打卡类型选项
  loadCheckinTypes: function() {
    cloud.task.getCheckinTypes()
      .then(data => {
        console.log('获取打卡类型选项成功:', data)
        
        this.setData({
          checkinTypes: data
        })
      })
      .catch(err => {
        console.error('获取打卡类型选项失败:', err)
        
        // 使用默认选项
        const defaultTypes = ['非集训', '集训上午', '集训下午']
        this.setData({
          checkinTypes: defaultTypes
        })
      })
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

  // 检查登录状态
  checkLogin: function() {
    if (!this.data.userInfo || !this.data.userInfo._id) {
      wx.showModal({
        title: '提示',
        content: '请先登录后再发布任务',
        showCancel: false,
        success: () => {
          wx.switchTab({
            url: '/pages/profile/profile'
          })
        }
      })
    }
  },

  // 输入框变化处理函数
  onTitleInput: function(e) {
    const form = this.data.form
    form.title = e.detail.value
    this.setData({ form })
  },

  onDescriptionInput: function(e) {
    const form = this.data.form
    form.description = e.detail.value
    this.setData({ form })
  },

  onTimePeriodChange: function(e) {
    const index = e.detail.value
    const timePeriod = this.data.timePeriodOptions[index]
    
    const form = this.data.form
    form.timePeriod = timePeriod
    
    this.setData({ 
      form,
      timePeriodIndex: index
    })
  },

  onCheckinTypeChange: function(e) {
    const index = e.detail.value
    const checkinType = this.data.checkinTypes[index]
    
    const form = this.data.form
    form.checkinType = checkinType
    
    this.setData({ 
      form,
      checkinTypeIndex: index
    })
  },

  onRequirementInput: function(e) {
    const form = this.data.form
    form.requirements = e.detail.value
    this.setData({ form })
  },

  onRemarkInput: function(e) {
    const form = this.data.form
    form.remark = e.detail.value
    this.setData({ form })
  },

  // 日期时间选择器列变化处理
  onStartColumnChange: function(e) {
    // 获取列索引和选中值
    const column = e.detail.column;
    const value = e.detail.value;
    const data = this.data.startDateTimeArray.data;
    const indexs = this.data.startDateTimeArray.indexs;
    
    // 更新索引
    indexs[column] = value;
    
    // 如果修改了年或月，需要动态更新日期范围
    if (column === 0 || column === 1) {
      // 获取选中的年和月
      const year = parseInt(data[0][indexs[0]].replace('年', ''));
      const month = parseInt(data[1][indexs[1]].replace('月', ''));
      
      // 根据年月获取该月的天数
      const daysInMonth = new Date(year, month, 0).getDate();
      
      // 更新日期数组
      const days = [];
      for (let i = 1; i <= daysInMonth; i++) {
        days.push(i + '日');
      }
      
      // 更新日期选择器数据
      data[2] = days;
      
      // 确保日期索引不超出范围
      if (indexs[2] >= days.length) {
        indexs[2] = days.length - 1;
      }
    }
    
    // 更新选择器数据
    this.setData({
      'startDateTimeArray.data': data,
      'startDateTimeArray.indexs': indexs
    });
  },
  
  onEndColumnChange: function(e) {
    // 与起始时间选择器逻辑相同
    const column = e.detail.column;
    const value = e.detail.value;
    const data = this.data.endDateTimeArray.data;
    const indexs = this.data.endDateTimeArray.indexs;
    
    indexs[column] = value;
    
    if (column === 0 || column === 1) {
      const year = parseInt(data[0][indexs[0]].replace('年', ''));
      const month = parseInt(data[1][indexs[1]].replace('月', ''));
      
      const daysInMonth = new Date(year, month, 0).getDate();
      
      const days = [];
      for (let i = 1; i <= daysInMonth; i++) {
        days.push(i + '日');
      }
      
      data[2] = days;
      
      if (indexs[2] >= days.length) {
        indexs[2] = days.length - 1;
      }
    }
    
    this.setData({
      'endDateTimeArray.data': data,
      'endDateTimeArray.indexs': indexs
    });
  },
  
  // 日期时间选择器值变化处理
  onStartDateTimeChange: function(e) {
    const indexs = e.detail.value;
    const data = this.data.startDateTimeArray.data;
    
    // 获取选中的年月日时分
    const year = data[0][indexs[0]];
    const month = data[1][indexs[1]];
    const day = data[2][indexs[2]];
    const hour = data[3][indexs[3]];
    const minute = data[4][indexs[4]];
    
    // 格式化日期时间字符串
    const dateTimeStr = formatDateTime(year, month, day, hour, minute);
    
    // 更新表单数据
    this.setData({
      'form.startDateTime': dateTimeStr,
      'startDateTimeArray.indexs': indexs
    });
  },
  
  onEndDateTimeChange: function(e) {
    const indexs = e.detail.value;
    const data = this.data.endDateTimeArray.data;
    
    const year = data[0][indexs[0]];
    const month = data[1][indexs[1]];
    const day = data[2][indexs[2]];
    const hour = data[3][indexs[3]];
    const minute = data[4][indexs[4]];
    
    const dateTimeStr = formatDateTime(year, month, day, hour, minute);
    
    this.setData({
      'form.endDateTime': dateTimeStr,
      'endDateTimeArray.indexs': indexs
    });
  },

  // 提交任务
  handleSubmit: function() {
    // 再次检查登录状态
    if (!this.data.userInfo || !this.data.userInfo._id) {
      wx.showToast({
        title: '请先登录',
        icon: 'none'
      })
      return
    }

    if (!cloud || !cloud.task) {
      console.error('cloud module not properly loaded')
      wx.showToast({
        title: '系统错误',
        icon: 'none'
      })
      return
    }

    const { form } = this.data

    // 表单验证
    if (!form.title.trim()) {
      wx.showToast({
        title: '请填写任务标题',
        icon: 'none'
      })
      return
    }

    if (!form.description.trim()) {
      wx.showToast({
        title: '请填写任务描述',
        icon: 'none'
      })
      return
    }

    if (!form.timePeriod) {
      wx.showToast({
        title: '请选择时间周期',
        icon: 'none'
      })
      return
    }

    if (!form.checkinType) {
      wx.showToast({
        title: '请选择打卡类型',
        icon: 'none'
      })
      return
    }

    if (!form.startDateTime) {
      wx.showToast({
        title: '请选择打卡起始时间',
        icon: 'none'
      })
      return
    }

    if (!form.endDateTime) {
      wx.showToast({
        title: '请选择打卡截止时间',
        icon: 'none'
      })
      return
    }

    // 验证时间格式
    if (form.startDateTime >= form.endDateTime) {
      wx.showToast({
        title: '起始时间必须早于截止时间',
        icon: 'none'
      })
      return
    }

    // 设置加载状态
    this.setData({ loading: true })

    wx.showLoading({
      title: this.data.isEdit ? '更新中...' : '发布中...'
    })

    // 如果是编辑模式
    if (this.data.isEdit && this.data.taskId) {
      // 更新任务数据
      const taskData = {
        ...form,
        _id: this.data.taskId,
        updatedAt: new Date().getTime()
      }

      cloud.task.updateTask(taskData)
        .then(() => {
          wx.hideLoading()
          
          this.setData({ loading: false })
          
          // 设置全局数据变更标记
          const app = getApp();
          app.globalData.taskDataChanged = true;
          app.globalData.lastTaskId = this.data.taskId;
          
          wx.showToast({
            title: '更新成功',
            icon: 'success'
          })
          
          setTimeout(() => {
            wx.navigateBack()
          }, 1500)
        })
        .catch(err => {
          console.error('更新任务失败:', err)
          
          this.setData({ loading: false })
          
          wx.hideLoading()
          wx.showToast({
            title: err.message || '更新失败，请重试',
            icon: 'none'
          })
        })
      
      return
    }

    // 添加用户信息和时间戳到任务数据
    const taskData = {
      ...form,
      createdBy: this.data.userInfo._id,
      creatorName: this.data.userInfo.nickName,
      createdAt: new Date().getTime(),
      status: 'active', // 标记任务状态为活跃
      participants: [], // 初始化参与者列表
      completedCount: 0 // 初始化完成计数
    }

    cloud.task.createTask(taskData)
      .then((result) => {
        wx.hideLoading()
        
        // 数据变更标记已在cloud.js中设置，这里不需要重复设置
        // 但保存任务ID以便后续使用
        let taskId = null;
        if (result && result._id) {
          taskId = result._id;
        } else if (result && result.id) {
          taskId = result.id;
        }
        
        // 重置表单数据
        this.setData({
          form: {
            title: '',
            description: '',
            timePeriod: '',
            checkinType: '',
            startDateTime: '',
            endDateTime: '',
            requirements: '',
            remark: ''
          },
          loading: false
        })
        
        wx.showToast({
          title: '发布成功',
          icon: 'success'
        })
        
        // 使用setTimeout确保toast显示完成
        setTimeout(() => {
          // 如果创建的是紧急任务或有特殊需求，可以直接跳转到任务详情页
          if (taskId && taskData.checkinType && taskData.checkinType.includes('集训')) {
            wx.navigateTo({
              url: `/pages/task-detail/task-detail?id=${taskId}`
            });
          } else {
            // 否则返回任务列表页
            wx.switchTab({
              url: '/pages/index/index'
            });
          }
        }, 1500)
      })
      .catch(err => {
        console.error('创建任务失败:', err)
        
        this.setData({ loading: false })
        
        wx.hideLoading()
        wx.showToast({
          title: err.message || '发布失败，请重试',
          icon: 'none'
        })
      })
  },

  // 加载任务详情（编辑模式）
  loadTaskDetail: function(taskId) {
    wx.showLoading({
      title: '加载中...'
    })
    
    cloud.task.getTaskDetail(taskId)
      .then(task => {
        console.log('获取任务详情成功:', task)
        
        // 设置表单数据
        this.setData({
          form: {
            title: task.title || '',
            description: task.description || '',
            timePeriod: task.timePeriod || '',
            checkinType: task.checkinType || '',
            startDateTime: task.startDateTime || task.startTime || '',
            endDateTime: task.endDateTime || task.endTime || '',
            requirements: task.requirements || '',
            remark: task.remark || ''
          }
        })
        
        // 设置时间周期索引
        if (task.timePeriod) {
          const index = this.data.timePeriodOptions.findIndex(item => item === task.timePeriod)
          if (index !== -1) {
            this.setData({ timePeriodIndex: index })
          }
        }
        
        // 设置打卡类型索引
        if (task.checkinType) {
          const index = this.data.checkinTypes.findIndex(item => item === task.checkinType)
          if (index !== -1) {
            this.setData({ checkinTypeIndex: index })
          }
        }
        
        // 兼容处理：如果有旧格式的时间字段，尝试转换为新格式
        if (!this.data.form.startDateTime && task.startTime && task.deadline) {
          const startDateTime = `${task.deadline} ${task.startTime}`;
          this.setData({ 'form.startDateTime': startDateTime });
        }
        
        if (!this.data.form.endDateTime && task.endTime && task.deadline) {
          const endDateTime = `${task.deadline} ${task.endTime}`;
          this.setData({ 'form.endDateTime': endDateTime });
        }
        
        wx.hideLoading()
      })
      .catch(err => {
        console.error('获取任务详情失败:', err)
        wx.hideLoading()
        wx.showToast({
          title: '加载任务失败',
          icon: 'none'
        })
      })
  },

  // 处理导航栏返回按钮点击
  onBackTap: function() {
    wx.navigateBack({
      delta: 1
    })
  },

  // 处理页面滚动
  onPageScroll: function(e) {
    this.setData({
      scrollTop: e.scrollTop
    });
  },
  
  // 滚动到底部触发
  onReachBottom: function() {
    console.log('滚动到底部');
  },
  
  // 滚动到顶部触发
  onScrollToUpper: function() {
    console.log('滚动到顶部');
  },
  
  // 滚动事件触发
  onScroll: function(e) {
    // 可以在这里处理滚动过程中的逻辑
  }
}) 