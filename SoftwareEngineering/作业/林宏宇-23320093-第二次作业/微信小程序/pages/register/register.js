const app = getApp()

Page({
  data: {
    form: {
      studentId: '',
      nickName: '',
      password: '',
      confirmPassword: '',
      gender: 'male',
      college: '',
      grade: '',
      teamStatus: '在训'
    },
    statusIndex: 0, // 队员状态索引，默认是"在训"
    teamStatusOptions: ['在训', '退役', '退队'],
    errorMessage: '',
    loading: false
  },

  onLoad: function(options) {
    // 页面加载
  },

  // 处理学号输入(单独处理因为是必填)
  onStudentIdInput: function(e) {
    this.setData({
      'form.studentId': e.detail.value,
      errorMessage: ''
    })
  },

  // 通用输入处理函数
  onInput: function(e) {
    const field = e.currentTarget.dataset.field
    const value = e.detail.value
    
    // 使用对象计算属性名语法设置表单字段
    this.setData({
      [`form.${field}`]: value,
      errorMessage: ''
    })
  },
  
  // 处理队员状态选择变化
  onStatusChange: function(e) {
    const index = e.detail.value
    const status = this.data.teamStatusOptions[index]
    
    this.setData({
      statusIndex: index,
      'form.teamStatus': status,
      errorMessage: ''
    })
  },

  // 处理注册按钮点击
  onRegister: function() {
    // 表单验证
    if (!this.data.form.studentId) {
      this.setData({ errorMessage: '请输入学号' })
      return
    }
    
    if (!this.data.form.nickName) {
      this.setData({ errorMessage: '请输入姓名' })
      return
    }
    
    if (!this.data.form.password) {
      this.setData({ errorMessage: '请输入密码' })
      return
    }
    
    if (this.data.form.password !== this.data.form.confirmPassword) {
      this.setData({ errorMessage: '两次输入的密码不一致' })
      return
    }
    
    if (!this.data.form.college) {
      this.setData({ errorMessage: '请输入学院' })
      return
    }
    
    if (!this.data.form.grade) {
      this.setData({ errorMessage: '请输入年级' })
      return
    }
    
    this.setData({ loading: true, errorMessage: '' })
    
    // 准备提交的数据(移除confirmPassword)
    const { confirmPassword, ...submitData } = this.data.form

    // 调用云函数注册
    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'register',
        data: submitData
      }
    }).then(res => {
      console.log('注册结果', res)
      
      if (res.result && res.result.code === 0) {
        // 注册成功，保存用户信息
        const userInfo = res.result.data
        app.globalData.userInfo = userInfo
        
        try {
          wx.setStorageSync('userInfo', userInfo)
          wx.setStorageSync('isLogin', true)
        } catch (e) {
          console.error('保存用户信息失败', e)
        }
        
        wx.showToast({
          title: '注册成功',
          icon: 'success',
          duration: 2000,
          complete: () => {
            // 注册成功后跳转到首页
            setTimeout(() => {
              wx.switchTab({
                url: '/pages/index/index'
              })
            }, 2000)
          }
        })
      } else {
        // 注册失败
        this.setData({
          errorMessage: res.result?.message || '注册失败，请重试',
          loading: false
        })
      }
    }).catch(err => {
      console.error('注册失败', err)
      this.setData({
        errorMessage: '注册失败: ' + (err.message || err.errMsg || '未知错误'),
        loading: false
      })
    })
  },

  // 跳转到登录页
  goToLogin: function() {
    wx.navigateTo({
      url: '/pages/login/login'
    })
  },

  // 返回按钮处理
  onBackTap: function() {
    wx.navigateBack()
  }
}) 