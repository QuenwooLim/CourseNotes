const cloud = require('../../utils/cloud.js')

Page({
  data: {
    rankingList: [],
    loading: true,
    userId: wx.getStorageSync('userInfo') ? wx.getStorageSync('userInfo')._id : '',
    
    // 筛选相关
    timePeriods: ['全部'],
    timePeriodIndex: 0,
    checkinTypes: ['全部'],
    checkinTypeIndex: 0,
    
    // 当前选中的筛选条件
    selectedTimePeriod: '',
    selectedCheckinType: ''
  },

  // 处理导航栏返回按钮点击
  onBackTap: function() {
    wx.navigateBack({
      delta: 1
    })
  },

  onLoad: function() {
    this.setData({
      userId: wx.getStorageSync('userInfo') ? wx.getStorageSync('userInfo')._id : ''
    })
    
    // 加载筛选选项
    this.loadFilterOptions()
    
    // 加载排行榜
    this.loadRanking()
  },

  onShow: function() {
    this.loadRanking()
  },

  onRefresh: function() {
    this.setData({ refreshing: true })
    this.loadRanking().finally(() => {
      this.setData({ refreshing: false })
    })
  },
  
  // 加载筛选选项
  loadFilterOptions: function() {
    // 初始默认值
    const defaultTimePeriods = ['全部', '第1周', '第2周', '第3周', '第4周', '第5周', '第6周', '第7周', '第8周', '第9周', '第10周', 
                    '第11周', '第12周', '第13周', '第14周', '第15周', '第16周', '第17周', '第18周', '寒训', '暑训'];
    const defaultCheckinTypes = ['全部', '非集训', '集训上午', '集训下午'];
    
    // 先设置默认值，防止API调用失败时没有选项
    this.setData({
      timePeriods: defaultTimePeriods,
      checkinTypes: defaultCheckinTypes
    });
    
    // 获取时间周期选项
    cloud.task.getTimePeriods()
      .then(res => {
        console.log('时间周期选项详情:', res);
        try {
          // 处理时间周期数据
          let allPeriods = ['全部'];
          
          // 尝试直接将WEEKS和TRAINING合并
          if (res && res.WEEKS && Array.isArray(res.WEEKS)) {
            allPeriods = [...allPeriods, ...res.WEEKS];
          }
          
          if (res && res.TRAINING && Array.isArray(res.TRAINING)) {
            allPeriods = [...allPeriods, ...res.TRAINING];
          }
          
          // 如果合并后的数组长度大于1，说明成功获取了选项
          if (allPeriods.length > 1) {
            console.log('使用从服务器获取的时间周期选项:', allPeriods);
            this.setData({ timePeriods: allPeriods });
          } else {
            console.warn('服务器返回的时间周期选项无效，使用默认值');
          }
        } catch (error) {
          console.error('处理时间周期选项出错:', error);
        }
      })
      .catch(err => {
        console.error('获取时间周期选项失败:', err);
      });
    
    // 获取打卡类型选项
    cloud.task.getCheckinTypes()
      .then(res => {
        console.log('打卡类型选项详情:', res);
        try {
          // 如果返回的是有效数组，则使用
          if (res && Array.isArray(res) && res.length > 0) {
            const types = ['全部', ...res];
            console.log('使用从服务器获取的打卡类型选项:', types);
            this.setData({ checkinTypes: types });
          } else {
            console.warn('服务器返回的打卡类型选项无效，使用默认值');
          }
        } catch (error) {
          console.error('处理打卡类型选项出错:', error);
        }
      })
      .catch(err => {
        console.error('获取打卡类型选项失败:', err);
      });
  },

  // 时间周期选择变化
  onTimePeriodChange: function(e) {
    const index = parseInt(e.detail.value)
    const selectedTimePeriod = index === 0 ? '' : this.data.timePeriods[index]
    
    this.setData({
      timePeriodIndex: index,
      selectedTimePeriod
    })
    
    // 重新加载排行榜
    this.loadRanking()
  },
  
  // 打卡类型选择变化
  onCheckinTypeChange: function(e) {
    const index = parseInt(e.detail.value)
    const selectedCheckinType = index === 0 ? '' : this.data.checkinTypes[index]
    
    this.setData({
      checkinTypeIndex: index,
      selectedCheckinType
    })
    
    // 重新加载排行榜
    this.loadRanking()
  },

  // 加载排行榜
  loadRanking: function() {
    if (!cloud || !cloud.checkin) {
      console.error('cloud module not properly loaded')
      return Promise.reject(new Error('cloud module not properly loaded'))
    }

    this.setData({ loading: true })
    
    // 构建筛选参数
    const filters = {}
    if (this.data.selectedTimePeriod) {
      filters.timePeriod = this.data.selectedTimePeriod
    }
    if (this.data.selectedCheckinType) {
      filters.checkinType = this.data.selectedCheckinType
    }
    
    console.log('加载排行榜, 筛选条件:', filters)
    
    // 显示加载提示
    wx.showLoading({
      title: '加载中...',
      mask: true
    })
    
    // 调用云函数获取排行榜
    return wx.cloud.callFunction({
      name: 'checkin',
      data: {
        type: 'ranking',
        rankingType: 'all',
        filters: filters
      }
    })
    .then(res => {
      wx.hideLoading()
      console.log('排行榜数据:', res.result)
      
      if (res.result && res.result.code === 0) {
        this.setData({
          rankingList: res.result.data?.list || [],
          loading: false
        })
        
        // 如果数据为空，显示提示消息
        if (!res.result.data?.list || res.result.data?.list.length === 0) {
          wx.showToast({
            title: res.result.message || '暂无符合条件的数据',
            icon: 'none',
            duration: 2000
          })
        }
      } else {
        this.setData({
          rankingList: [],
          loading: false
        })
        wx.showToast({
          title: res.result?.message || '获取排行榜失败',
          icon: 'none'
        })
      }
    })
    .catch(err => {
      wx.hideLoading()
      console.error('获取排行榜失败：', err)
      wx.showToast({
        title: '获取排行榜失败，请稍后重试',
        icon: 'none',
        duration: 2000
      })
      this.setData({ loading: false })
      return Promise.reject(err)
    })
  }
}) 