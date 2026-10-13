// app.js
App({
  onLaunch: function () {
    console.log('小程序启动 - onLaunch')
    
    // 初始化全局数据，确保所有状态都有默认值
    this.globalData = {
      userInfo: null,
      isLogin: false,
      isLoggingIn: false,
      isLoggingOut: false,
      hasInitialized: false,
      taskDataChanged: false,  // 添加任务数据变更标记
      lastTaskId: null         // 最后操作的任务ID
    }
    
    // 安全地初始化云开发环境
    this.initCloud();
    
    // 启动时检查登录状态
    this.checkLoginStatus();
    
    // 标记初始化完成
    this.globalData.hasInitialized = true;
    console.log('应用初始化完成');
  },
  
  // 初始化云开发环境
  initCloud: function() {
    try {
      if (!wx.cloud) {
        console.error('请使用 2.2.3 或以上的基础库以使用云能力')
        wx.showModal({
          title: '提示',
          content: '当前微信版本过低，无法使用该功能，请升级到最新微信版本后重试。',
          showCancel: false
        })
        return false;
      }
      
      wx.cloud.init({
        env: 'cloud1-8g5yt7n443fd27f9',
        traceUser: true
      });
      
      console.log('云开发环境初始化成功');
      return true;
    } catch (e) {
      console.error('初始化云开发环境失败:', e);
      return false;
    }
  },
  
  // 检查登录状态
  checkLoginStatus: function() {
    console.log('执行登录状态检查')
    try {
      const userInfo = wx.getStorageSync('userInfo')
      const isLogin = wx.getStorageSync('isLogin')
      
      if (userInfo && isLogin) {
        this.globalData.userInfo = userInfo
        this.globalData.isLogin = true
        console.log('已检测到登录状态')
        
        // 如果已登录，尝试从服务器获取最新用户数据
        this.refreshUserInfo()
      } else {
        // 状态不一致，清理
        if (userInfo || isLogin) {
          console.log('登录状态不一致，执行清理')
          try {
            wx.removeStorageSync('userInfo')
            wx.removeStorageSync('isLogin')
            wx.removeStorageSync('isCaptain')
            wx.removeStorageSync('userTasks')
          } catch (e) {
            console.error('清理存储失败:', e);
          }
          
          this.globalData.userInfo = null
          this.globalData.isLogin = false
        }
      }
    } catch (e) {
      console.error('检查登录状态失败', e)
      // 错误情况下默认为未登录
      this.globalData.userInfo = null
      this.globalData.isLogin = false
    }
  },
  
  // 刷新用户信息
  refreshUserInfo: function() {
    if (!this.globalData.userInfo || !this.globalData.userInfo._id) {
      console.log('无用户信息，跳过刷新')
      return
    }
    
    console.log('从服务器刷新用户信息')
    // 从服务器获取最新用户数据
    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'get',
        userId: this.globalData.userInfo._id
      }
    }).then(res => {
      if (res.result && res.result.code === 0 && res.result.data) {
        // 更新本地用户信息
        const serverUserInfo = res.result.data
        this.globalData.userInfo = serverUserInfo
        
        try {
          wx.setStorageSync('userInfo', serverUserInfo)
          console.log('已更新本地用户信息', serverUserInfo)
        } catch (e) {
          console.error('保存用户信息失败:', e)
        }
      } else {
        console.error('获取用户信息失败:', res)
      }
    }).catch(err => {
      console.error('刷新用户信息失败:', err)
    })
  },
  
  // 检查页面访问权限
  checkPagePermission: function(pagePath) {
    console.log('检查页面权限:', pagePath)
    
    // 无需登录即可访问的页面
    const publicPages = [
      'pages/index/index',
      'pages/ranking/ranking',
      'pages/login/login',
      'pages/profile/profile'
    ]
    
    // 如果是公共页面，允许访问
    if (publicPages.includes(pagePath)) {
      return true
    }
    
    // 检查登录状态
    const isLoggedIn = this.globalData.isLogin && this.globalData.userInfo
    
    // 如果已登录，允许访问所有页面
    if (isLoggedIn) {
      return true
    }
    
    // 未登录且不是公共页面，拒绝访问
    console.log('未登录，无法访问:', pagePath)
    
    try {
      // 引导至个人中心登录
      wx.switchTab({
        url: '/pages/profile/profile',
        fail: (err) => {
          console.error('跳转到个人中心失败:', err);
          // 尝试使用reLaunch代替
          wx.reLaunch({
            url: '/pages/profile/profile',
            fail: (reLaunchErr) => {
              console.error('reLaunch到个人中心也失败:', reLaunchErr);
            }
          });
        }
      })
    } catch (e) {
      console.error('重定向到登录页失败:', e);
    }
    
    return false
  },

  onShow: function() {
    console.log('小程序显示 - onShow')
    
    // 确保全局数据已初始化
    if (!this.globalData) {
      this.globalData = {
        userInfo: null,
        isLogin: false,
        isLoggingIn: false,
        isLoggingOut: false,
        hasInitialized: false
      }
      
      // 尝试重新初始化
      this.initCloud();
      this.checkLoginStatus();
      this.globalData.hasInitialized = true;
    } else if (this.globalData.isLogin && this.globalData.userInfo) {
      // 如果已登录，刷新用户信息
      this.refreshUserInfo();
    }
  },
  
  // 错误处理
  onError: function(err) {
    console.error('小程序发生错误:', err);
    // 记录错误并重置关键状态
    if (this.globalData) {
      this.globalData.isLoggingIn = false;
      this.globalData.isLoggingOut = false;
    }
  },
  
  // 全局数据获取
  getGlobalData: function() {
    return this.globalData || {
      userInfo: null,
      isLogin: false,
      isLoggingIn: false,
      isLoggingOut: false,
      hasInitialized: false
    };
  }
})
