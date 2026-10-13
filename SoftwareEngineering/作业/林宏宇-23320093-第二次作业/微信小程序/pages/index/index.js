// index.js
const cloud = require('../../utils/cloud')

Page({
  data: {
    tasks: [],
    currentTab: 0,
    page: 1,
    pageSize: 10,
    hasMore: true,
    loading: false,
    refreshing: false,
    userInfo: null,
    userTasks: {}, // 记录用户已参与的任务
    hasLoginTip: false, // 添加登录提示标记
    selectedDate: ''
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function(options) {
    console.log('首页加载 - onLoad')
    
    // 检查页面访问权限，但添加日志以便调试
    const app = getApp()
    const hasPermission = app.checkPagePermission('pages/index/index')
    console.log('首页权限检查结果:', hasPermission)
    
    if (!hasPermission) {
      console.log('首页没有访问权限，将返回')
      return  // 如果没有权限，直接返回
    }
    
    console.log('开始加载首页内容')
    
    // 设置当前日期 - 直接使用字符串而不是调用formatDate函数
    const today = new Date();
    const formattedDate = `${today.getFullYear()}-${String(today.getMonth() + 1).padStart(2, '0')}-${String(today.getDate()).padStart(2, '0')}`;
    
    this.setData({
      selectedDate: formattedDate
    });
    
    // 加载打卡任务列表
    this.loadTasks();
  },

  // 格式化日期函数，可以在其他地方使用
  formatDate: function(date) {
    if (!date || !(date instanceof Date)) {
      date = new Date();
    }
    return `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`;
  },

  onShow: function() {
    // 获取全局数据
    const app = getApp();
    const taskDataChanged = app.globalData.taskDataChanged;
    
    // 每次显示页面都检查登录状态
    this.loadUserInfo();
    
    // 检查是否需要刷新数据
    if (taskDataChanged) {
      console.log('检测到任务数据变更，刷新首页数据');
      // 重置全局标记
      app.globalData.taskDataChanged = false;
      
      // 清空任务列表并重新加载
      this.setData({
        tasks: [],
        page: 1,
        hasMore: true
      });
      
      // 加载任务
      this.loadTasks();
    } else {
      // 如果任务列表为空，则加载任务
      if (this.data.tasks.length === 0) {
        this.setData({
          page: 1,
          hasMore: true
        });
        this.loadTasks();
      }
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
    
    // 如果用户已登录，获取参与过的任务
    if (userInfo && userInfo._id) {
      this.loadUserTasks()
    }
  },
  
  // 加载用户参与的任务
  loadUserTasks: function() {
    // 这里可以请求获取用户已参与的任务列表
    // 简化处理：实际项目中应当通过云函数查询用户参与的任务
    const userTasks = wx.getStorageSync('userTasks') || {}
    this.setData({ userTasks })
  },

  onPullDownRefresh: function() {
    this.setData({
      tasks: [],
      page: 1,
      hasMore: true,
      refreshing: true
    })
    this.loadTasks()
  },

  onReachBottom: function() {
    if (this.data.hasMore && !this.data.loading) {
      this.loadTasks()
    }
  },

  // 加载任务列表
  loadTasks: function() {
    if (this.data.loading) return
    
    // 检查用户是否已登录 - 添加登录状态检查
    const app = getApp();
    if (!app || !app.globalData || !app.globalData.isLogin || !app.globalData.userInfo) {
      // 用户未登录，提供登录提示
      this.setData({
        loading: false,
        refreshing: false,
        tasks: [], // 清空任务列表
        hasLoginTip: true // 添加登录提示标记
      });
      
      // 停止刷新动画
      wx.stopPullDownRefresh();
      return;
    }
    
    this.setData({ loading: true, hasLoginTip: false })
    
    if (!cloud || !cloud.task) {
      // 如果云函数模块未加载，使用模拟数据
      const mockTasks = [
        {
          _id: '1',
          title: '每日打卡',
          description: '每天完成打卡任务',
          points: 10,
          deadline: '2024-12-31',
          status: '进行中'
        },
        {
          _id: '2',
          title: '运动打卡',
          description: '每天运动30分钟',
          points: 20,
          deadline: '2024-12-31',
          status: '进行中'
        }
      ]

      setTimeout(() => {
        this.setData({
          tasks: [...this.data.tasks, ...mockTasks],
          loading: false,
          refreshing: false,
          hasMore: false
        })
        wx.stopPullDownRefresh()
      }, 500)
      return
    }

    // 使用云函数获取任务列表
    let lastTaskId = app.globalData.lastTaskId;
    cloud.task.getTaskList(this.data.page, this.data.pageSize)
      .then(res => {
        const newTasks = res.list || []
        const hasMore = newTasks.length === this.data.pageSize
        
        // 处理任务状态
        let processedTasks = newTasks.map(task => {
          // 检查用户是否参与了该任务
          const hasJoined = task.participants && task.participants.some(participant => participant === this.data.userInfo._id)
          
          // 检查是否是新创建的任务
          const isNewTask = task._id === lastTaskId;
          
          return {
            ...task,
            hasJoined,
            isNewTask,
            // 根据截止日期判断任务是否已过期
            isExpired: this.isTaskExpired(task),
            // 根据开始时间判断任务是否尚未开始
            isNotStarted: this.isTaskNotStarted(task)
          }
        })
        
        // 如果有新创建的任务，确保它显示在最前面
        if (lastTaskId) {
          processedTasks = processedTasks.sort((a, b) => {
            if (a._id === lastTaskId) return -1;
            if (b._id === lastTaskId) return 1;
            return 0;
          });
          
          // 清除lastTaskId，避免重复处理
          app.globalData.lastTaskId = null;
        }
        
        this.setData({
          tasks: [...this.data.tasks, ...processedTasks],
          page: this.data.page + 1,
          hasMore,
          loading: false,
          refreshing: false
        })
        
        wx.stopPullDownRefresh()
      })
      .catch(err => {
        console.error('获取任务列表失败:', err)
        this.setData({ 
          loading: false,
          refreshing: false
        })
        
        // 错误可能是由于登录状态问题，提供登录入口
        if (err && err.message && (err.message.includes('login') || err.message.includes('auth') || !app.globalData.isLogin)) {
          this.showLoginPrompt();
        } else {
          wx.showToast({
            title: '获取任务列表失败',
            icon: 'none'
          })
        }
        
        wx.stopPullDownRefresh()
      })
  },

  // 判断任务是否过期的辅助函数
  isTaskExpired: function(task) {
    if (!task) return false;
    
    // 先检查endDateTime
    if (task.endDateTime) {
      return new Date(task.endDateTime) < new Date();
    } 
    // 向下兼容，检查老格式的deadline
    else if (task.deadline) {
      return new Date(task.deadline) < new Date();
    }
    
    return false;
  },
  
  // 判断任务是否尚未开始的辅助函数
  isTaskNotStarted: function(task) {
    if (!task) return false;
    
    // 检查startDateTime
    if (task.startDateTime) {
      try {
        return new Date(task.startDateTime.replace(/-/g, '/')) > new Date();
      } catch (e) {
        console.error('解析 startDateTime 出错:', e);
        return false;
      }
    }
    
    return false;
  },

  // 添加显示登录提示的方法
  showLoginPrompt: function() {
    wx.showModal({
      title: '提示',
      content: '请先登录后查看任务列表',
      confirmText: '去登录',
      cancelText: '取消',
      success: (res) => {
        if (res.confirm) {
          // 跳转到登录页面
          wx.navigateTo({
            url: '/pages/login/login',
            fail: (err) => {
              console.error('跳转登录页失败:', err);
              // 备选方案，跳转到个人中心
              wx.switchTab({
                url: '/pages/profile/profile'
              });
            }
          });
        }
      }
    });
  },
  
  // 添加登录按钮点击处理
  onLoginTap: function() {
    wx.navigateTo({
      url: '/pages/login/login',
      fail: (err) => {
        console.error('跳转登录页失败:', err);
        wx.switchTab({
          url: '/pages/profile/profile'
        });
      }
    });
  },

  // 切换底部导航
  switchTab: function(e) {
    const index = e.currentTarget.dataset.index
    if (index === this.data.currentTab) return
    
    this.setData({ currentTab: index })
    
    if (index === 1) {
      // 切换到排行榜
      wx.navigateTo({
        url: '/pages/ranking/ranking'
      })
    } else if (index === 2) {
      // 切换到发布任务
      wx.navigateTo({
        url: '/pages/task/task'
      })
    }
  },

  // 处理任务点击
  handleTaskClick: function(e) {
    // 检查用户是否已登录
    if (!this.data.userInfo || !this.data.userInfo._id) {
      wx.showToast({
        title: '请先登录',
        icon: 'none'
      })
      
      // 跳转到个人中心页面进行登录
      wx.switchTab({
        url: '/pages/profile/profile'
      })
      return
    }
    
    const task = e.currentTarget.dataset.task
    
    // 导航到任务详情页
    wx.navigateTo({
      url: `/pages/task-detail/task-detail?id=${task._id}`
    })
  },

  // 参与任务
  joinTask: function(e) {
    // 阻止事件冒泡
    e.stopPropagation()
    
    // 检查用户是否已登录
    if (!this.data.userInfo || !this.data.userInfo._id) {
      wx.showToast({
        title: '请先登录',
        icon: 'none'
      })
      return
    }
    
    const taskId = e.currentTarget.dataset.id
    if (!taskId) return
    
    // 查找当前任务
    const task = this.data.tasks.find(t => t._id === taskId)
    if (!task) return
    
    // 检查任务是否过期
    if (task.isExpired) {
      wx.showToast({
        title: '任务已截止',
        icon: 'none'
      })
      return
    }
    
    // 检查任务是否尚未开始
    if (task.isNotStarted) {
      wx.showToast({
        title: '任务尚未开始',
        icon: 'none'
      })
      return
    }
    
    wx.showLoading({
      title: '处理中...'
    })
    
    cloud.task.joinTask(taskId, this.data.userInfo._id)
      .then(() => {
        wx.hideLoading()
        
        // 更新本地任务参与状态
        const userTasks = this.data.userTasks || {}
        userTasks[taskId] = true
        wx.setStorageSync('userTasks', userTasks)
        
        // 更新UI
        const tasks = this.data.tasks.map(task => {
          if (task._id === taskId) {
            task.hasJoined = true
          }
          return task
        })
        
        this.setData({
          userTasks,
          tasks
        })
        
        wx.showToast({
          title: '参与成功',
          icon: 'success'
        })
      })
      .catch(err => {
        wx.hideLoading()
        wx.showToast({
          title: err.message || '参与失败',
          icon: 'none'
        })
      })
  }
})
