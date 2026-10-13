const app = getApp()

Page({
  data: {
    userId: '',
    memberInfo: null,
    loading: true,
  },

  onLoad: function (options) {
    if (options.userId) {
      this.setData({
        userId: options.userId,
      })
      this.loadMemberDetail()
    } else {
      wx.showToast({
        title: '缺少队员ID',
        icon: 'none',
      })
      // 可以选择返回上一页
      // setTimeout(() => { wx.navigateBack() }, 1500)
    }
  },

  loadMemberDetail: function () {
    this.setData({ loading: true })
    wx.showLoading({
      title: '加载中...',
      mask: true,
    })

    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'get', // 使用 'get' 类型获取用户信息
        userId: this.data.userId,
      },
    })
    .then(res => {
      wx.hideLoading()
      console.log('获取队员详情结果:', res)

      if (res.result && res.result.code === 0 && res.result.data) {
        this.setData({
          memberInfo: res.result.data,
          loading: false,
        })
        // 可以在获取信息后设置导航栏标题为人名
        if (res.result.data.nickName) {
          wx.setNavigationBarTitle({ title: res.result.data.nickName })
        }
      } else {
        this.handleLoadError(res.result?.message)
      }
    })
    .catch(err => {
      wx.hideLoading()
      console.error('调用云函数失败:', err)
      this.handleLoadError('网络错误，请重试')
    })
  },

  handleLoadError: function (message) {
    this.setData({ loading: false })
    wx.showToast({
      title: message || '加载失败',
      icon: 'none',
    })
  },
}) 