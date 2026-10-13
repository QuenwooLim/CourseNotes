const app = getApp()
const cloud = require('../../utils/cloud')

Page({
  data: {
    form: {
      studentId: '',
      password: ''
    },
    passwordDisplay: '', // 用于显示星号的字符串
    passwordFocus: false, // 控制密码输入框的焦点
    loading: false,
    showError: false,
    errorMessage: ''
  },

  onLoad: function(options) {
    // 获取应用实例 - 在整个函数开始时获取一次
    const app = getApp();
    
    // 检查是否已登录
    if (app && app.globalData && app.globalData.isLogin && app.globalData.userInfo) {
      // 已登录状态下，直接跳转到首页
      wx.reLaunch({
        url: '/pages/index/index'
      })
      return; // 提前返回，避免后续代码执行
    }
    
    // 如果有登录/退出操作正在进行，显示提示
    if (app && app.globalData && (app.globalData.isLoggingIn || app.globalData.isLoggingOut)) {
      wx.showToast({
        title: '请稍候再试',
        icon: 'none',
        duration: 2000
      })
    }

    // 可选: 从本地存储加载上次使用的学号
    const lastStudentId = wx.getStorageSync('lastStudentId')
    if (lastStudentId) {
      this.setData({
        'form.studentId': lastStudentId
      })
    }
  },

  // 输入框变更事件
  onStudentIdInput: function(e) {
    this.setData({
      'form.studentId': e.detail.value
    })
  },

  onPasswordInput: function(e) {
    const password = e.detail.value
    // 生成对应数量的星号
    const passwordDisplay = '*'.repeat(password.length)
    
    this.setData({
      'form.password': password,
      passwordDisplay: passwordDisplay
    })
  },

  // 点击密码输入框时设置焦点
  onPasswordContainerTap: function() {
    this.setData({ passwordFocus: true })
  },

  // 提交表单
  onSubmit: function() {
    // 获取应用实例
    const app = getApp();
    
    // 安全检查
    if (!app || !app.globalData) {
      wx.showToast({
        title: '应用初始化失败，请重启小程序',
        icon: 'none',
        duration: 2000
      });
      return;
    }
    
    // 检查是否有其他登录/退出操作进行中
    if (app.globalData.isLoggingIn || app.globalData.isLoggingOut) {
      wx.showToast({
        title: '请稍候再试',
        icon: 'none',
        duration: 2000
      })
      return
    }
    
    // 表单验证
    const { studentId, password } = this.data.form
    if (!studentId) {
      wx.showToast({
        title: '请输入学号',
        icon: 'none'
      })
      return
    }
    
    if (!password) {
      wx.showToast({
        title: '请输入密码',
        icon: 'none'
      })
      return
    }
    
    // 设置登录状态标记
    app.globalData.isLoggingIn = true
    
    // 显示加载中
    this.setData({ loading: true })
    wx.showLoading({
      title: '登录中...',
      mask: true
    })
    
    // 安全的使用Promise和错误处理
    const loginPromise = this.clearLoginState()
      .then(() => {
        // 根据当前模式调用不同的接口
        return this.handleLogin()
      })
      .catch(err => {
        console.error('登录失败:', err)
        throw err; // 继续传递错误
      })
      .finally(() => {
        // 无论成功失败，都清理登录中状态
        if (app && app.globalData) {
          app.globalData.isLoggingIn = false;
        }
        this.setData({ loading: false });
        wx.hideLoading();
      });
    
    // 单独处理错误显示，避免finally后错误被吞掉
    loginPromise.catch(err => {
      wx.showToast({
        title: err && err.message ? err.message : '操作失败，请重试',
        icon: 'none',
        duration: 2000
      });
    });
  },

  // 处理登录逻辑
  handleLogin: function() {
    const { studentId, password } = this.data.form
    
    return new Promise((resolve, reject) => {
      if (!wx.cloud) {
        return reject(new Error('云开发环境未初始化'));
      }
      
      wx.cloud.callFunction({
        name: 'user',
        data: {
          type: 'login',
          data: {
            studentId,
            password
          }
        }
      })
      .then(res => {
        if (res && res.result && res.result.code === 0) {
          const userData = res.result.data
          const app = getApp()
          
          // 保存用户信息到全局
          if (app && app.globalData) {
            app.globalData.userInfo = userData
            app.globalData.isLogin = true
          }
          
          // 保存到本地存储
          try {
            wx.setStorageSync('userInfo', userData)
            wx.setStorageSync('isLogin', true)
            console.log('登录成功，保存用户信息到本地', userData)
          } catch (e) {
            console.error('保存用户信息到本地存储失败:', e);
            // 继续执行，不影响后续流程
          }
          
          wx.hideLoading()
          
          // 显示成功提示
          wx.showToast({
            title: '登录成功',
            icon: 'success',
            mask: true,
            duration: 2000
          })
          
          // 延迟跳转确保数据保存完成
          setTimeout(() => {
            wx.reLaunch({
              url: '/pages/index/index',
              complete: (res) => {
                console.log('登录后跳转完成', res)
                resolve() // 无论跳转成功还是失败，都认为流程完成
              },
              fail: (err) => {
                console.error('跳转失败:', err)
                // 不要reject，因为登录流程已经完成
              }
            })
          }, 2000)
        } else {
          const errorMsg = res && res.result && res.result.message ? res.result.message : '登录失败';
          reject(new Error(errorMsg))
        }
      })
      .catch(error => {
        console.error('云函数调用失败:', error);
        reject(new Error('网络错误，请稍后再试'))
      })
    })
  },

  // 清理登录状态
  clearLoginState: function() {
    return new Promise((resolve, reject) => {
      try {
        // 清理本地存储
        try {
          wx.removeStorageSync('userInfo')
          wx.removeStorageSync('isLogin')
          wx.removeStorageSync('isCaptain')
          wx.removeStorageSync('userTasks')
        } catch (e) {
          console.error('清理本地存储失败:', e)
          // 继续执行，不影响主流程
        }
        
        // 清理全局状态
        const app = getApp()
        if (app && app.globalData) {
          app.globalData.userInfo = null
          app.globalData.isLogin = false
        }
        
        resolve()
      } catch (e) {
        console.error('清理登录状态整体失败:', e)
        // 即使失败也继续流程
        resolve()
      }
    })
  },

  // 导航到注册页面
  navigateToRegister: function() {
    wx.navigateTo({
      url: '/pages/register/register',
      fail: (err) => {
        console.error('跳转到注册页面失败:', err);
        wx.showToast({
          title: '跳转失败，请重试',
          icon: 'none'
        });
      }
    })
  },

  // 返回按钮处理
  onBackTap: function() {
    wx.navigateBack({
      fail: () => {
        // 如果返回失败，可能是没有上一页，跳转到首页
        wx.reLaunch({
          url: '/pages/index/index'
        })
      }
    })
  }
}) 