const cloud = require('../../utils/cloud')

Page({
  data: {
    userInfo: {},
    stats: {
      totalCheckins: 0,
      rank: '未上榜'
    },
    isCaptain: false,
    refreshing: false,
    showLoginButton: false
  },

  onLoad: function() {
    console.log('cloud module:', cloud)
    if (!cloud || !cloud.checkin) {
      console.error('cloud module not properly loaded')
      return
    }
    
    this.loadUserInfo()
  },

  onShow: function() {
    this.loadUserInfo()
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
    
    // 同步队长状态
    const isCaptain = userInfo.isCaptain || false
    wx.setStorageSync('isCaptain', isCaptain)
    
    this.setData({ 
      userInfo,
      isCaptain: isCaptain
    })
    
    // 获取用户打卡统计
    this.loadUserStats()
  },

  // 加载用户统计数据
  loadUserStats: function() {
    if (!cloud || !cloud.checkin) {
      console.error('cloud module not properly loaded')
      return
    }

    // 使用默认数据
    const defaultStats = {
      totalCheckins: 0,
      rank: '未上榜'
    }

    // 设置默认值先
    this.setData({ stats: defaultStats })
    
    // 如果用户未登录，不加载统计数据
    if (!this.data.userInfo || !this.data.userInfo._id) {
      return
    }

    // 简单使用直接调用，不使用嵌套的setTimeout
    wx.showLoading({ title: '加载中' })
    
    // 在finally中确保隐藏loading
    cloud.checkin.getUserStats()
      .then(data => {
        this.setData({
          stats: {
            totalCheckins: data.totalCheckins || 0,
            rank: data.rank || '未上榜'
          }
        })
      })
      .catch(err => {
        console.error('获取用户统计数据失败:', err)
      })
      .finally(() => {
        wx.hideLoading()
      })
  },

  // 处理登录
  onLoginTap: function() {
    // 如果已经在退出过程中，不重复处理
    const app = getApp()
    if (app.globalData.isLoggingOut) {
      wx.showToast({
        title: '请稍候再试',
        icon: 'none',
        duration: 2000
      })
      return
    }
    
    // 直接导航到登录页面
    wx.navigateTo({
      url: '/pages/login/login',
      fail: (err) => {
        console.error('跳转到登录页失败:', err)
        wx.showToast({
          title: '跳转失败，请重试',
          icon: 'none'
        })
      }
    })
  },

  // 点击发布任务
  onPublishTaskTap: function() {
    wx.navigateTo({
      url: '/pages/task/task'
    })
  },

  // 点击打卡历史
  onHistoryTap: function() {
    wx.navigateTo({
      url: '/pages/history/history'
    })
  },

  // 点击设置
  onSettingsTap: function() {
    wx.navigateTo({
      url: '/pages/settings/settings'
    })
  },

  // 点击关于
  onAboutTap: function() {
    wx.showToast({
      title: '打卡小程序 v1.0',
      icon: 'none'
    })
  },

  navigateToTask() {
    wx.navigateTo({
      url: '/pages/task/task'
    });
  },

  // 退出登录
  onLogoutTap: function() {
    const app = getApp()
    
    // 如果已经在登录/退出过程中，不重复处理
    if (app.globalData.isLoggingIn || app.globalData.isLoggingOut) {
      wx.showToast({
        title: '操作进行中，请稍候',
        icon: 'none',
        duration: 2000
      })
      return
    }
    
    wx.showModal({
      title: '提示',
      content: '确定要退出登录吗？',
      success: (res) => {
        if (res.confirm) {
          // 标记退出状态
          app.globalData.isLoggingOut = true
          
          wx.showLoading({
            title: '退出中...',
            mask: true
          })
          
          // 重置页面数据
          this.setData({
            userInfo: {},
            stats: {
              totalCheckins: 0,
              rank: '未上榜'
            },
            isCaptain: false
          })

          // 清除全局数据
          app.globalData.userInfo = null
          app.globalData.isLogin = false
          
          // 使用同步方法确保清理完成
          try {
            wx.removeStorageSync('userInfo')
            wx.removeStorageSync('isLogin')
            wx.removeStorageSync('isCaptain')
            wx.removeStorageSync('userTasks')
            console.log('本地存储清理完成')
          } catch (e) {
            console.error('清理本地存储失败', e)
          }

          wx.hideLoading()
          
          // 显示退出成功提示
          wx.showToast({
            title: '已退出登录',
            icon: 'success',
            mask: true,
            duration: 2000
          })
          
          // 延长等待时间，确保操作完成
          setTimeout(() => {
            app.globalData.isLoggingOut = false
            
            // 跳转到登录页面
            wx.reLaunch({
              url: '/pages/login/login',
              complete: () => {
                console.log('已跳转到登录页面')
              }
            })
          }, 1000)
        }
      }
    })
  },

  // 点击队长管理（原队长模式）
  onCaptainModeTap: function() {
    // 如果已经是队长，直接跳转
    if (this.data.isCaptain) {
      wx.navigateTo({
        url: '/pages/captain/captain'
      });
      return;
    }
    
    // 否则弹出验证码输入框
    wx.showModal({
      title: '队长验证',
      content: '请输入队长验证码',
      editable: true,
      placeholderText: '请输入验证码',
      success: (res) => {
        if (res.confirm) {
          // 验证码为"full"
          if (res.content === 'full') {
            // 显示加载提示
            wx.showLoading({
              title: '验证中...',
              mask: true
            });
            
            // 调用云函数更新队长状态
            wx.cloud.callFunction({
              name: 'user',
              data: {
                type: 'verifyCaptain',
                userId: this.data.userInfo._id,
                password: 'SYSU'  // 使用云函数中已设置的队长密码
              }
            }).then(res => {
              wx.hideLoading();
              
              if (res.result && res.result.code === 0) {
                // 验证成功，将用户标记为队长
                const app = getApp();
                let userInfo = this.data.userInfo;
                
                userInfo.isCaptain = true;
                app.globalData.userInfo = userInfo;
                
                // 更新存储
                try {
                  wx.setStorageSync('userInfo', userInfo);
                  wx.setStorageSync('isCaptain', true);
                } catch (e) {
                  console.error('保存队长状态失败', e);
                }
                
                // 更新页面数据
                this.setData({
                  userInfo: userInfo,
                  isCaptain: true
                });
                
                wx.showToast({
                  title: '验证成功',
                  icon: 'success',
                  duration: 2000
                });
                
                // 跳转到队长页面
                setTimeout(() => {
                  wx.navigateTo({
                    url: '/pages/captain/captain'
                  });
                }, 1000);
              } else {
                wx.showToast({
                  title: res.result?.message || '验证失败',
                  icon: 'error',
                  duration: 2000
                });
              }
            }).catch(err => {
              wx.hideLoading();
              console.error('调用云函数失败:', err);
              wx.showToast({
                title: '验证失败，请重试',
                icon: 'error',
                duration: 2000
              });
            });
          } else {
            // 验证码错误
            wx.showToast({
              title: '验证码错误',
              icon: 'error',
              duration: 2000
            });
          }
        }
      }
    });
  },

  // 下拉刷新
  onPullDownRefresh: function() {
    this.setData({ refreshing: true });
    this.loadUserInfo();
    this.loadUserStats()
      .then(() => {
        this.setData({ refreshing: false });
      })
      .catch(() => {
        this.setData({ refreshing: false });
      });
  },

  // 队员档案 - 新增功能（待开发）
  onTeamMemberProfileTap: function() {
    console.log('onTeamMemberProfileTap function triggered');
    wx.navigateTo({
      url: '/pages/member-profiles/member-profiles'
    })
  },

  // 队伍荣誉 - 新增功能（待开发）
  onTeamHonorsTap: function() {
    wx.showToast({
      title: '功能开发中',
      icon: 'none'
    })
  }
}); 