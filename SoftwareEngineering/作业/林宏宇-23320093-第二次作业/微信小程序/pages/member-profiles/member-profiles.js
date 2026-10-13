const app = getApp()

Page({
  data: {
    members: [],
    loading: true,
    refreshing: false,
    hasMore: true,
    activeCount: 0, // 添加在训队员数量统计
    scrollHeight: 0 // 添加滚动区域高度
  },

  onLoad: function (options) {
    // 计算滚动区域高度
    this.calculateScrollHeight()
    this.loadMembers()
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
      }
    })
  },
  
  onReady: function() {
    // 页面渲染完成后，再次计算滚动区域高度
    this.calculateScrollHeight()
  },
  
  onResize: function() {
    // 页面尺寸变化时，重新计算滚动区域高度
    this.calculateScrollHeight()
  },

  onRefresh: function () {
    this.setData({ 
      refreshing: true,
      hasMore: true
    })
    this.loadMembers(() => {
      this.setData({ refreshing: false })
      wx.stopPullDownRefresh()
    })
  },

  loadMembers: function (callback) {
    this.setData({ loading: true })
    wx.showLoading({
      title: '加载中...',
      mask: true,
    })

    // 获取当前登录用户的ID，用于权限验证
    const currentUserInfo = app.globalData.userInfo || wx.getStorageSync('userInfo') || {}
    const userId = currentUserInfo._id || ''

    if (!userId) {
      wx.hideLoading()
      wx.showToast({
        title: '请先登录',
        icon: 'none'
      })
      if (callback) callback()
      return
    }

    // 调用云函数获取所有成员信息
    wx.cloud.callFunction({
      name: 'team',
      data: {
        action: 'getTeamMembers',
        userId: userId // 传递当前用户ID，用于权限验证
      },
    }).then(res => {
        wx.hideLoading()
        console.log('获取所有队员信息结果:', res)

        if (res.result && res.result.code === 0) {
          let members = res.result.data || []
          
          // 按照队员ID排序 (将ID转为数字进行比较)
          members.sort((a, b) => {
            const idA = parseInt(a._id) || 0;
            const idB = parseInt(b._id) || 0;
            return idA - idB;
          });

          // 统计在训队员数量
          const activeCount = members.filter(member => member.teamStatus === '在训').length

          this.setData({
            members: members,
            loading: false,
            hasMore: false,
            activeCount: activeCount // 设置在训队员数量
          })
        } else {
          console.error('获取队员信息失败:', res.result?.message || '未知错误')
          this.setData({
            members: [],
            loading: false,
            hasMore: false,
            activeCount: 0 // 重置在训队员数量
          })
          wx.showToast({
            title: res.result?.message || '加载失败',
            icon: 'none',
          })
        }
        if (callback) callback()
      }).catch(err => {
        wx.hideLoading()
        console.error('调用云函数失败:', err)
        this.setData({ loading: false, hasMore: false, activeCount: 0 })
        wx.showToast({
          title: '加载失败，请重试',
          icon: 'none'
        })
        if (callback) callback()
      })
  },
  
  // 点击队员卡片跳转到详情页
  onMemberTap: function(e) {
    const userId = e.currentTarget.dataset.id;
    if (userId) {
      wx.navigateTo({
        url: `/pages/member-detail/member-detail?userId=${userId}`
      });
    } else {
      wx.showToast({
        title: '无法获取队员ID',
        icon: 'none'
      });
    }
  }
}) 