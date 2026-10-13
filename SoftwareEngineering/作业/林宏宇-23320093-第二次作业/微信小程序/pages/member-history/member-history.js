const app = getApp()

Page({
  /**
   * 页面的初始数据
   */
  data: {
    memberId: '',
    memberName: '',
    memberInfo: null,
    history: [],
    loading: true,
    isPullDownRefresh: false,
    filterType: 'all', // 'all', 'month', 'week'
    totalCheckins: 0,
    continuousCheckins: 0
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    if (options.id && options.name) {
      this.setData({
        memberId: options.id,
        memberName: options.name
      })
      
      this.loadMemberInfo()
      this.loadCheckinHistory()
    } else {
      wx.showToast({
        title: '参数错误',
        icon: 'none'
      })
      setTimeout(() => {
        wx.navigateBack()
      }, 1500)
    }
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    // 队长模式已验证，无需重复检查
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
    this.setData({ isPullDownRefresh: true })
    
    this.loadCheckinHistory(() => {
      wx.stopPullDownRefresh()
      this.setData({ isPullDownRefresh: false })
    })
  },

  /**
   * 返回上一页
   */
  onBackTap: function () {
    wx.navigateBack()
  },

  /**
   * 加载队员基本信息
   */
  loadMemberInfo: function () {
    wx.cloud.callFunction({
      name: 'user',
      data: {
        type: 'get',
        userId: this.data.memberId
      }
    }).then(res => {
      if (res.result && res.result.data) {
        this.setData({
          memberInfo: res.result.data
        })
      } else {
        console.error('获取用户信息失败: 返回数据格式错误', res)
        wx.showToast({
          title: '获取用户信息失败，请重试',
          icon: 'none',
          duration: 2000
        })
      }
    }).catch(err => {
      console.error('获取用户信息失败', err)
      wx.showToast({
        title: '网络错误，请检查网络后重试',
        icon: 'none',
        duration: 2000
      })
    })
  },

  /**
   * 加载打卡记录
   */
  loadCheckinHistory: function (callback) {
    this.setData({ loading: true })
    
    // 根据筛选类型确定时间范围
    let startDate = null
    if (this.data.filterType === 'month') {
      // 本月第一天
      const now = new Date()
      startDate = new Date(now.getFullYear(), now.getMonth(), 1)
    } else if (this.data.filterType === 'week') {
      // 本周第一天（周一）
      const now = new Date()
      const dayOfWeek = now.getDay() || 7 // 将周日的0转为7
      const diff = dayOfWeek - 1 // 计算与周一的差值
      startDate = new Date(now)
      startDate.setDate(now.getDate() - diff)
      startDate.setHours(0, 0, 0, 0)
    }
    
    console.log('加载用户打卡记录，ID:', this.data.memberId)
    
    // 调用云函数获取打卡记录
    wx.cloud.callFunction({
      name: 'checkin',
      data: {
        type: 'getMemberCheckins',
        userId: this.data.memberId,
        startDate: startDate ? startDate.toISOString() : null
      }
    }).then(res => {
      console.log('获取打卡记录结果:', res)
      
      if (res.result && res.result.code === 0) {
        const checkins = res.result.data || []
        const stats = res.result.stats || {}
        
        // 处理打卡记录，添加展示所需信息
        const history = checkins.map(item => {
          // 创建日期对象
          const checkinDate = item.createTime ? new Date(item.createTime) : new Date()
          
          // 格式化日期、星期、时间
          const dateText = `${checkinDate.getMonth() + 1}.${checkinDate.getDate()}`
          const weekdays = ['周日', '周一', '周二', '周三', '周四', '周五', '周六']
          const weekday = weekdays[checkinDate.getDay()]
          
          // 返回处理后的记录
          return {
            ...item,
            dateText,
            weekday,
            timeText: item.createTimeFormat ? item.createTimeFormat.split(' ')[1] : '00:00',
            taskName: item.taskTitle || '打卡任务',
            location: item.location || '未记录位置'
          }
        })
        
        this.setData({
          history,
          totalCheckins: stats.totalCount || 0,
          continuousCheckins: stats.continuousCount || 0,
          loading: false
        })
      } else {
        console.error('获取打卡记录失败: 返回数据格式错误', res)
        this.setData({
          history: [],
          loading: false
        })
        
        wx.showToast({
          title: '暂无打卡记录',
          icon: 'none',
          duration: 2000
        })
      }
      
      if (callback) callback()
    }).catch(err => {
      console.error('获取打卡记录失败', err)
      this.setData({ loading: false })
      
      wx.showToast({
        title: '网络错误，请检查网络后重试',
        icon: 'none',
        duration: 2000
      })
      
      if (callback) callback()
    })
  },
  
  /**
   * 获取用户统计数据
   */
  loadUserStats: function() {
    wx.cloud.callFunction({
      name: 'checkin',
      data: {
        type: 'userStats',
        userId: this.data.memberId
      }
    }).then(res => {
      if (res.result && res.result.data) {
        this.setData({
          continuousCheckins: res.result.data.continuousCheckins || 0
        })
      }
    }).catch(err => {
      console.error('获取用户统计数据失败', err)
    })
  },

  /**
   * 切换筛选类型
   */
  onFilterTap: function (e) {
    const type = e.currentTarget.dataset.type
    if (type !== this.data.filterType) {
      this.setData({ filterType: type })
      this.loadCheckinHistory()
    }
  },

  /**
   * 预览图片
   */
  previewImage: function (e) {
    const current = e.currentTarget.dataset.current
    const urls = e.currentTarget.dataset.urls
    
    wx.previewImage({
      current,
      urls
    })
  }
}) 