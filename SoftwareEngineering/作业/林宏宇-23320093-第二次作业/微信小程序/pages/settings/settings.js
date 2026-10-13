const cloud = require('../../utils/cloud')

Page({
  data: {
    userInfo: null,
    targetUserId: '', // 要编辑的用户ID
    targetUserName: '', // 要编辑的用户名称
    isCaptainEdit: false, // 是否是队长编辑模式
    loading: false,
    paddleSideOptions: ['左桨', '右桨', '双桨'],
    paddleSideIndex: 0,
    teamStatusOptions: ['在训', '退役', '退队'],
    teamStatusIndex: 0,
    newPassword: '',
    confirmPassword: '',
    showPassword: true, // 始终显示密码
    showConfirmPassword: true, // 始终显示确认密码
    savingToCloud: false,
    scrollTop: 0 // 新增滚动位置记录
  },

  onLoad: function(options) {
    // 获取全局配置
    const app = getApp()
    
    // 检查是否是队长编辑模式
    if (options.isCaptainEdit === 'true' && options.userId) {
      const targetUserId = options.userId
      const targetUserName = options.userName ? decodeURIComponent(options.userName) : '队员'
      
      this.setData({
        targetUserId: targetUserId,
        targetUserName: targetUserName,
        isCaptainEdit: true
      })
      
      // 修改导航栏标题
      wx.setNavigationBarTitle({
        title: `编辑${targetUserName}的信息`
      })
      
      // 加载目标用户的信息
      this.loadTargetUserInfo(targetUserId)
    } else {
      // 普通用户编辑自己的信息
      let userInfo = app.globalData.userInfo
    
      if (!userInfo) {
        // 尝试从本地获取
        userInfo = wx.getStorageSync('userInfo')
        // 更新全局状态
        if (userInfo) {
          app.globalData.userInfo = userInfo
        }
      }
    
      // 设置左右桨的默认索引
      const paddleSideIndex = this.data.paddleSideOptions.findIndex(
        side => side === userInfo.paddleSide
      )
    
      this.setData({
        userInfo: userInfo,
        paddleSideIndex: paddleSideIndex >= 0 ? paddleSideIndex : 0
      })
    }
  },

  // 加载用户信息
  loadUserInfo: function() {
    // 先显示加载状态
    wx.showLoading({
      title: '加载中...',
      mask: true
    })

    // 先从全局数据获取用户信息
    const app = getApp()
    let userInfo = app.globalData.userInfo
    
    // 如果全局数据没有，则从本地存储获取
    if (!userInfo) {
      userInfo = wx.getStorageSync('userInfo') || {}
      if (userInfo._id) {
        app.globalData.userInfo = userInfo
      }
    }
    
    // 为性别数据添加默认值，用于单选按钮组
    if (!userInfo.gender) {
      userInfo.gender = 'male'
    }
    
    // 设置左右桨的默认索引
    const paddleSideIndex = this.data.paddleSideOptions.findIndex(
      side => side === userInfo.paddleSide
    )
    
    this.setData({ 
      userInfo,
      paddleSideIndex: paddleSideIndex >= 0 ? paddleSideIndex : 0
    })
    
    // 检查登录状态
    if (!userInfo || !userInfo._id) {
      wx.hideLoading()
      wx.showModal({
        title: '提示',
        content: '请先登录后再设置个人信息',
        showCancel: false,
        success: () => {
          wx.switchTab({
            url: '/pages/profile/profile'
          })
        }
      })
      return
    }

    // 从服务器获取最新的用户信息
    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'get',
        userId: userInfo._id
      }
    }).then(res => {
      wx.hideLoading()
      if (res.result && res.result.code === 0 && res.result.data) {
        const serverUserInfo = res.result.data
        
        // 使用服务器返回的最新数据更新本地数据
        const updatedUserInfo = {
          ...userInfo,
          ...serverUserInfo
        }
        
        // 设置左右桨的默认索引
        const paddleSideIndex = this.data.paddleSideOptions.findIndex(
          side => side === updatedUserInfo.paddleSide
        )
        
        this.setData({
          userInfo: updatedUserInfo,
          paddleSideIndex: paddleSideIndex >= 0 ? paddleSideIndex : 0
        })
        
        // 更新全局状态和本地存储
        app.globalData.userInfo = updatedUserInfo
        wx.setStorageSync('userInfo', updatedUserInfo)
        
        console.log('已从服务器获取最新用户信息', updatedUserInfo)
      } else {
        console.error('获取用户信息失败', res)
      }
    }).catch(err => {
      wx.hideLoading()
      console.error('从服务器获取用户信息失败', err)
      wx.showToast({
        title: '获取用户信息失败',
        icon: 'none'
      })
    })
  },

  // 从服务器加载目标用户的信息
  loadTargetUserInfo: function(userId) {
    wx.showLoading({
      title: '加载中...',
      mask: true
    })
    
    // 调用云函数获取用户信息
    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'get',
        userId: userId
      }
    }).then(res => {
      wx.hideLoading()
      if (res.result && res.result.code === 0 && res.result.data) {
        const userInfo = res.result.data
        
        // 设置左右桨的默认索引
        const paddleSideIndex = this.data.paddleSideOptions.findIndex(
          side => side === userInfo.paddleSide
        )
        
        // 设置队员状态的默认索引
        const teamStatusIndex = this.data.teamStatusOptions.findIndex(
          status => status === userInfo.teamStatus
        )
        
        this.setData({
          userInfo: userInfo,
          paddleSideIndex: paddleSideIndex >= 0 ? paddleSideIndex : 0,
          teamStatusIndex: teamStatusIndex >= 0 ? teamStatusIndex : 0
        })
        
        console.log('已从服务器获取目标用户信息', userInfo)
      } else {
        console.error('获取用户信息失败', res)
        wx.showToast({
          title: '获取用户信息失败',
          icon: 'none'
        })
      }
    }).catch(err => {
      wx.hideLoading()
      console.error('从服务器获取用户信息失败', err)
      wx.showToast({
        title: '获取用户信息失败',
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

  // 选择头像
  onChooseAvatar: function() {
    wx.chooseImage({
      count: 1,
      sizeType: ['compressed'],
      sourceType: ['album', 'camera'],
      success: (res) => {
        // 获取图片临时路径
        const tempFilePath = res.tempFilePaths[0]
        
        // 上传图片到云存储
        this.uploadAvatar(tempFilePath)
      }
    })
  },

  // 上传头像到云存储
  uploadAvatar: function(filePath) {
    wx.showLoading({
      title: '上传中...',
      mask: true
    })

    // 生成随机文件名
    const timestamp = new Date().getTime()
    const randomStr = Math.random().toString(36).substring(2, 8)
    const cloudPath = `avatars/${this.data.userInfo._id}_${timestamp}_${randomStr}.jpg`

    // 上传图片
    wx.cloud.uploadFile({
      cloudPath,
      filePath,
      success: res => {
        // 获取图片链接
        const fileID = res.fileID
        
        // 更新用户头像URL
        this.setData({
          'userInfo.avatarUrl': fileID
        })
        
        wx.hideLoading()
        wx.showToast({
          title: '上传成功',
          icon: 'success'
        })
      },
      fail: err => {
        console.error('上传头像失败:', err)
        wx.hideLoading()
        wx.showToast({
          title: '上传失败',
          icon: 'none'
        })
      }
    })
  },

  // 密码相关函数
  onPasswordInput: function(e) {
    this.setData({ 
      newPassword: e.detail.value 
    })
  },
  
  onConfirmPasswordInput: function(e) {
    this.setData({ 
      confirmPassword: e.detail.value 
    })
  },
  
  // 左右桨变化处理
  onPaddleSideChange: function(e) {
    const index = e.detail.value
    this.setData({
      paddleSideIndex: index,
      'userInfo.paddleSide': this.data.paddleSideOptions[index]
    })
  },
  
  // 入队时间变化处理
  onJoinDateChange: function(e) {
    this.setData({
      'userInfo.joinDate': e.detail.value
    })
  },
  
  // 生日选择变化
  onBirthdayChange: function(e) {
    this.setData({
      'userInfo.birthday': e.detail.value
    })
  },
  
  // 身高输入变化
  onHeightInput: function(e) {
    this.setData({
      'userInfo.height': e.detail.value
    })
  },
  
  // 体重输入变化
  onWeightInput: function(e) {
    this.setData({
      'userInfo.weight': e.detail.value
    })
  },

  // 手机号输入变化
  onPhoneInput: function(e) {
    this.setData({
      'userInfo.phone': e.detail.value
    })
  },

  // 队长模式下的输入处理函数
  onNickNameInput: function(e) {
    this.setData({
      'userInfo.nickName': e.detail.value
    })
  },
  
  onStudentIdInput: function(e) {
    this.setData({
      'userInfo.studentId': e.detail.value
    })
  },
  
  onGenderChange: function(e) {
    this.setData({
      'userInfo.gender': e.detail.value
    })
  },
  
  onCollegeInput: function(e) {
    this.setData({
      'userInfo.college': e.detail.value
    })
  },
  
  onGradeInput: function(e) {
    this.setData({
      'userInfo.grade': e.detail.value
    })
  },
  
  onTeamStatusChange: function(e) {
    const index = e.detail.value
    this.setData({
      teamStatusIndex: index,
      'userInfo.teamStatus': this.data.teamStatusOptions[index]
    })
  },
  
  onTestLevelInput: function(e) {
    this.setData({
      'userInfo.testLevel': e.detail.value
    })
  },

  // 保存设置
  saveSettings: function() {
    if (!this.data.userInfo) {
      wx.showToast({
        title: '用户信息不完整',
        icon: 'none'
      })
      return
    }
    
    // 验证密码是否匹配（如果设置了新密码）
    if (this.data.newPassword) {
      if (this.data.newPassword.length < 6) {
        wx.showToast({
          title: '密码至少需要6位',
          icon: 'none'
        })
        return
      }
      
      if (this.data.newPassword !== this.data.confirmPassword) {
        wx.showToast({
          title: '两次密码输入不一致',
          icon: 'none'
        })
        return
      }
    }
    
    this.setData({ loading: true })
    
    // 构建要更新的用户数据对象
    let updateData = {
      _id: this.data.isCaptainEdit ? this.data.targetUserId : this.data.userInfo._id,
      avatarUrl: this.data.userInfo.avatarUrl,
      paddleSide: this.data.paddleSideOptions[this.data.paddleSideIndex],
      joinDate: this.data.userInfo.joinDate,
      birthday: this.data.userInfo.birthday,
      height: this.data.userInfo.height,
      weight: this.data.userInfo.weight,
      phone: this.data.userInfo.phone
    }
    
    // 队长编辑模式下，添加可修改的基本信息字段
    if (this.data.isCaptainEdit) {
      updateData = {
        ...updateData,
        nickName: this.data.userInfo.nickName,
        studentId: this.data.userInfo.studentId,
        gender: this.data.userInfo.gender,
        college: this.data.userInfo.college,
        grade: this.data.userInfo.grade,
        teamStatus: this.data.userInfo.teamStatus,
        testLevel: this.data.userInfo.testLevel
      }
    }
    
    // 如果设置了新密码，添加到更新数据中
    if (this.data.newPassword) {
      updateData.password = this.data.newPassword
    }
    
    // 获取当前登录用户信息
    const app = getApp()
    const currentUserInfo = app.globalData.userInfo || wx.getStorageSync('userInfo') || {}
    const currentUserId = currentUserInfo._id || ''
    
    if (this.data.isCaptainEdit && !currentUserId) {
      this.setData({ loading: false })
      wx.showToast({
        title: '队长身份验证失败，请重新登录',
        icon: 'none'
      })
      return
    }
    
    // 调用云函数更新用户信息
    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'update',
        data: updateData,
        isCaptainEdit: this.data.isCaptainEdit, // 添加队长编辑标志
        callerId: currentUserId // 传递当前登录用户ID用于验证队长身份
      }
    }).then(res => {
      this.setData({ loading: false })
      
      if (res.result && res.result.code === 0) {
        // 如果是队长编辑模式，不更新本地用户信息
        if (!this.data.isCaptainEdit) {
          // 更新全局用户信息和本地存储
          const app = getApp()
          const userInfo = {
            ...this.data.userInfo,
            ...updateData
          }
          app.globalData.userInfo = userInfo
          wx.setStorageSync('userInfo', userInfo)
        }
        
        // 清空密码字段
        this.setData({
          newPassword: '',
          confirmPassword: '',
          showPassword: true,
          showConfirmPassword: true
        })
        
        wx.showToast({
          title: '保存成功',
          icon: 'success',
          duration: 2000
        })
        
        // 如果是队长编辑模式，保存成功后返回上一页
        if (this.data.isCaptainEdit) {
          setTimeout(() => {
            wx.navigateBack({ delta: 1 })
          }, 2000)
        }
      } else {
        wx.showToast({
          title: res.result?.message || '保存失败',
          icon: 'none'
        })
      }
    }).catch(err => {
      console.error('保存设置失败:', err)
      this.setData({ loading: false })
      wx.showToast({
        title: '保存失败，请重试',
        icon: 'none'
      })
    })
  },

  // 滚动事件处理
  onPageScroll: function(e) {
    this.setData({
      scrollTop: e.scrollTop
    })
  },
  
  // 恢复滚动位置
  restoreScrollPosition: function() {
    if (this.data.scrollTop > 0) {
      wx.pageScrollTo({
        scrollTop: this.data.scrollTop,
        duration: 0
      })
    }
  },
  
  onShow: function() {
    // 页面显示时恢复滚动位置
    setTimeout(() => {
      this.restoreScrollPosition()
    }, 300)
  },
}) 