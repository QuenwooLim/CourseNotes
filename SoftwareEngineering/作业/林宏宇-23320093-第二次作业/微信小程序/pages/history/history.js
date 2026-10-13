const cloud = require('../../utils/cloud')

Page({
  data: {
    userId: '', // 当前用户的ID
    viewUserId: '', // 正在查看的用户的ID（可能是自己，也可能是队长查看的队员）
    viewUserName: '', // 正在查看的用户的名字
    historyList: [],
    totalCount: 0,
    loading: false,
    refreshing: false,
    page: 1,
    pageSize: 10,
    hasMore: true,
    isViewingOther: false // 标记是否正在查看他人的历史记录
  },

  onLoad: function(options) {
    // 获取当前登录用户的信息
    const app = getApp()
    const currentUserInfo = app.globalData.userInfo || wx.getStorageSync('userInfo') || {}
    const currentUserId = currentUserInfo._id || ''
    
    // 检查是否从队长模式跳转过来
    const viewUserId = options.userId || currentUserId // 如果没有传userId，则默认查看自己的
    const viewUserName = options.userName ? decodeURIComponent(options.userName) : (viewUserId === currentUserId ? '我' : '队员')
    const isViewingOther = viewUserId !== currentUserId
    
    console.log(`History page onLoad: currentUserId=${currentUserId}, viewUserId=${viewUserId}, viewUserName=${viewUserName}, isViewingOther=${isViewingOther}`)
    
    this.setData({
      userId: currentUserId,
      viewUserId: viewUserId,
      viewUserName: viewUserName,
      isViewingOther: isViewingOther
    })
    
    // 根据查看的用户设置页面标题
    if (isViewingOther) {
      wx.setNavigationBarTitle({
        title: `${viewUserName}的打卡记录`
      })
    }
    
    // 加载打卡历史记录
    this.loadHistory()
  },

  onShow: function() {
    // 如果需要每次显示页面都刷新，可以取消下面这行的注释
    // this.loadHistoryData(true)
  },

  // 处理下拉刷新
  onPullDownRefresh: function() {
    this.setData({
      historyList: [],
      page: 1,
      hasMore: true,
      refreshing: true
    })
    this.loadHistory()
  },

  // 处理上拉加载更多
  onReachBottom: function() {
    if (this.data.hasMore && !this.data.loading) {
      this.loadHistory()
    }
  },

  // 加载打卡历史记录
  loadHistory: function() {
    if (this.data.loading) return
    
    // 使用 viewUserId 加载数据
    const targetUserId = this.data.viewUserId
    if (!targetUserId) {
      wx.showToast({ title: '用户ID无效', icon: 'none' })
      this.setData({ loading: false, refreshing: false })
      wx.stopPullDownRefresh()
      return
    }
    
    this.setData({ loading: true })
    
    // 调用云函数获取历史记录
    wx.cloud.callFunction({
      name: 'checkin',
      data: {
        type: 'userHistory',
        userId: targetUserId, // 使用目标用户ID
        page: this.data.page,
        pageSize: this.data.pageSize
      }
    })
    .then(res => {
      console.log('获取打卡历史结果:', res)
      
      if (res.result && res.result.code === 0 && res.result.data) {
        const data = res.result.data
        const newHistory = data.list || []
        const totalCount = data.total || 0
        const hasMore = data.hasMore !== undefined ? data.hasMore : (newHistory.length === this.data.pageSize)
        
        this.setData({
          historyList: this.data.page === 1 ? newHistory : [...this.data.historyList, ...newHistory],
          totalCount: totalCount,
          page: this.data.page + 1,
          hasMore: hasMore,
          loading: false,
          refreshing: false
        })
      } else {
        // 处理错误或空数据情况
        this.setData({
          loading: false,
          refreshing: false,
          hasMore: false // 没有更多数据了
        })
        if (this.data.page === 1) { // 只有第一页加载失败或无数据才清空列表
          this.setData({ historyList: [] })
        }
        wx.showToast({
          title: res.result?.message || '加载失败',
          icon: 'none'
        })
      }
      
      wx.stopPullDownRefresh()
    })
    .catch(err => {
      console.error('加载打卡历史失败:', err)
      this.setData({
        loading: false,
        refreshing: false
      })
      wx.stopPullDownRefresh()
      wx.showToast({
        title: '加载失败，请重试',
        icon: 'none'
      })
    })
  },

  // 点击历史记录项
  onHistoryItemTap: function(e) {
    const recordId = e.currentTarget.dataset.id
    
    // 跳转到打卡详情页(checkin.js)，并告知是查看模式
    // 如果是队长查看，需要传递userName
    let url = `/pages/checkin/checkin?recordId=${recordId}&view=1`
    if (this.data.isViewingOther) {
      url += `&userName=${encodeURIComponent(this.data.viewUserName)}`
    }
    
    wx.navigateTo({
      url: url
    })
  },

  // 处理导航栏返回按钮点击
  onBackTap: function() {
    wx.navigateBack({
      delta: 1
    })
  }
}) 