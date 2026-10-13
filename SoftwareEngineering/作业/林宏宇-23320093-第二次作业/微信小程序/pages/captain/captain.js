const cloud = require('../../utils/cloud')

Page({
  data: {
    userInfo: null
  },

  onLoad: function() {
    this.loadUserInfo()
  },

  onShow: function() {
    // 每次页面显示时检查队长权限
    let isCaptain = wx.getStorageSync('isCaptain') || false
    
    // 同时检查用户对象中的isCaptain属性
    const userInfo = getApp().globalData.userInfo || wx.getStorageSync('userInfo') || {}
    
    console.log('队长状态检查:', {
      缓存中的队长状态: isCaptain,
      用户信息中的队长状态: userInfo.isCaptain,
      用户ID: userInfo._id
    })
    
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
    
    this.loadUserInfo()
  },

  // 处理导航栏返回按钮点击
  onBackTap: function() {
    wx.navigateBack({
      delta: 1
    })
  },

  // 加载用户信息
  loadUserInfo: function() {
    // 先从全局数据获取用户信息
    const app = getApp()
    let userInfo = app.globalData.userInfo
    
    // 如果全局数据没有，则从本地存储获取
    if (!userInfo) {
      userInfo = wx.getStorageSync('userInfo') || {}
      app.globalData.userInfo = userInfo
    }
    
    this.setData({ userInfo })
  },

  // 管理打卡任务
  onManageTaskTap: function() {
    wx.showActionSheet({
      itemList: ['发布新任务', '管理现有任务'],
      success: (res) => {
        if (res.tapIndex === 0) {
          // 发布新任务
          wx.navigateTo({
            url: '/pages/task/task'
          });
        } else if (res.tapIndex === 1) {
          // 管理现有任务，跳转到任务管理页面
          wx.navigateTo({
            url: '/pages/task-manager/task-manager'
          });
        }
      }
    });
  },

  // 查看队员打卡
  onViewMembersTap: function() {
    wx.navigateTo({
      url: '/pages/members/members'
    })
  },
  
  // 管理个人信息设置
  onManageSettingsTap: function() {
    wx.navigateTo({
      url: '/pages/members/members?action=settings'
    })
  }
}) 