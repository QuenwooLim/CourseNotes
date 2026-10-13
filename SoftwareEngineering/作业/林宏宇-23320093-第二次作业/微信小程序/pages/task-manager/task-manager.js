const cloud = require('../../utils/cloud')

Page({
  data: {
    loading: true,
    taskList: [],
    userInfo: null,
    refreshing: false,
    scrollHeight: 0 // 滚动区域高度
  },
  
  onLoad: function() {
    this.loadUserInfo()
    this.loadTaskList()
    // 计算滚动区域高度
    this.calculateScrollHeight()
  },
  
  onShow: function() {
    // 页面显示时检查队长权限
    let isCaptain = wx.getStorageSync('isCaptain') || false
    
    // 同时检查用户对象中的isCaptain属性
    const app = getApp();
    const userInfo = app.globalData.userInfo || wx.getStorageSync('userInfo') || {}
    if (userInfo.isCaptain) {
      isCaptain = true
      // 确保两个地方的状态保持一致
      wx.setStorageSync('isCaptain', true)
    }
    
    if (!isCaptain) {
      wx.showModal({
        title: '提示',
        content: '您不是队长，无法访问此页面',
        showCancel: false,
        success: () => {
          wx.navigateBack()
        }
      })
      return
    }
    
    // 检查任务数据是否有变更
    const taskDataChanged = app.globalData.taskDataChanged;
    if (taskDataChanged) {
      console.log('检测到任务数据变更，刷新任务管理页面数据');
      // 重置全局标记（在这个页面重置它，这样首页也能检测到变化）
      // app.globalData.taskDataChanged = false;
      // 刷新任务列表
      this.loadTaskList();
    }
  },
  
  // 处理导航栏返回按钮点击
  onBackTap: function() {
    wx.navigateBack({
      delta: 1
    })
  },
  
  // 处理滚动到底部
  onReachBottom: function() {
    console.log('滚动到底部');
    // 可以在这里添加加载更多的逻辑
  },
  
  // 加载用户信息
  loadUserInfo: function() {
    const app = getApp()
    let userInfo = app.globalData.userInfo
    
    if (!userInfo) {
      userInfo = wx.getStorageSync('userInfo') || {}
      if (userInfo._id) {
        app.globalData.userInfo = userInfo
      }
    }
    
    this.setData({ userInfo })
  },
  
  // 加载任务列表
  loadTaskList: function() {
    this.setData({ loading: true })
    
    if (!cloud || !cloud.task) {
      console.error('cloud module not properly loaded')
      this.setData({ loading: false })
      wx.showToast({
        title: '加载失败',
        icon: 'none'
      })
      return
    }
    
    // 获取最后操作的任务ID
    const app = getApp();
    const lastTaskId = app.globalData.lastTaskId;
    
    // 调用云函数获取所有任务
    return cloud.task.getAllTasks()
      .then(result => {
        console.log('获取任务列表成功:', result)
        
        // 格式化任务数据
        let taskList = result.map(task => {
          // 格式化日期
          if (task.createdAt) {
            const date = new Date(task.createdAt)
            task.createdAtFormatted = `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
          }
          
          // 标记最新操作的任务
          task.isRecentlyModified = (task._id === lastTaskId);
          
          return task
        })
        
        // 如果有最新操作的任务，将其放在最前面
        if (lastTaskId) {
          taskList = taskList.sort((a, b) => {
            if (a._id === lastTaskId) return -1;
            if (b._id === lastTaskId) return 1;
            return 0;
          });
        }
        
        this.setData({
          taskList,
          loading: false,
          refreshing: false
        })
        
        return result;
      })
      .catch(err => {
        console.error('获取任务列表失败:', err)
        this.setData({ loading: false, refreshing: false })
        wx.showToast({
          title: '加载任务失败',
          icon: 'none'
        })
        return Promise.reject(err);
      })
  },
  
  // 跳转到任务详情
  onTaskDetail: function(e) {
    const taskId = e.currentTarget.dataset.id
    wx.navigateTo({
      url: `/pages/task-detail/task-detail?id=${taskId}`
    })
  },
  
  // 创建新任务
  onCreateTask: function() {
    wx.navigateTo({
      url: '/pages/task/task'
    })
  },
  
  // 编辑任务
  onEditTask: function(e) {
    const taskId = e.currentTarget.dataset.id
    wx.navigateTo({
      url: `/pages/task/task?id=${taskId}&edit=1`
    })
  },
  
  // 删除任务
  onDeleteTask: function(e) {
    const taskId = e.currentTarget.dataset.id
    
    wx.showModal({
      title: '确认删除',
      content: '删除后无法恢复，确认要删除该任务吗？',
      success: (res) => {
        if (res.confirm) {
          this.setData({ loading: true })
          
          cloud.task.deleteTask(taskId)
            .then(() => {
              // 删除成功后，更新任务列表
              const taskList = this.data.taskList.filter(task => task._id !== taskId)
              
              this.setData({
                taskList,
                loading: false
              })
              
              // 设置全局数据变更标记
              const app = getApp();
              app.globalData.taskDataChanged = true;
              app.globalData.lastTaskId = null;
              
              wx.showToast({
                title: '删除成功',
                icon: 'success'
              })
            })
            .catch(err => {
              console.error('删除任务失败:', err)
              this.setData({ loading: false })
              wx.showToast({
                title: '删除失败',
                icon: 'none'
              })
            })
        }
      }
    })
  },
  
  // 下拉刷新
  onPullDownRefresh: function() {
    this.setData({ refreshing: true });
    this.loadTaskList()
      .then(() => {
        this.setData({ refreshing: false });
      })
      .catch(() => {
        this.setData({ refreshing: false });
      });
  },
  
  // 计算滚动区域高度
  calculateScrollHeight: function() {
    wx.getSystemInfo({
      success: (res) => {
        // 计算滚动区域高度：屏幕高度 - 导航栏高度 - 底部安全区域高度
        const scrollHeight = res.windowHeight - (res.platform === 'ios' ? 44 : 48) - (res.safeArea ? (res.screenHeight - res.safeArea.bottom) : 0)
        this.setData({
          scrollHeight: scrollHeight
        })
        console.log('滚动区域高度计算: ', scrollHeight)
      }
    })
  },
  
  onReady: function() {
    // 页面渲染完成后再次计算
    this.calculateScrollHeight()
  },
  
  onResize: function() {
    // 在页面大小变化时重新计算
    this.calculateScrollHeight()
  }
}) 