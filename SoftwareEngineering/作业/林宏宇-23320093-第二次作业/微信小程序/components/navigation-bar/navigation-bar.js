Component({
  options: {
    multipleSlots: true // 在组件定义时的选项中启用多slot支持
  },
  /**
   * 组件的属性列表
   */
  properties: {
    extClass: {
      type: String,
      value: ''
    },
    title: {
      type: String,
      value: ''
    },
    background: {
      type: String,
      value: ''
    },
    color: {
      type: String,
      value: ''
    },
    back: {
      type: Boolean,
      value: true
    },
    loading: {
      type: Boolean,
      value: false
    },
    homeButton: {
      type: Boolean,
      value: false,
    },
    animated: {
      // 显示隐藏的时候opacity动画效果
      type: Boolean,
      value: true
    },
    show: {
      // 显示隐藏导航，隐藏的时候navigation-bar的高度占位还在
      type: Boolean,
      value: true,
      observer: '_showChange'
    },
    // back为true的时候，返回的页面深度
    delta: {
      type: Number,
      value: 1
    },
  },
  /**
   * 组件的初始数据
   */
  data: {
    displayStyle: '',
    statusBarHeight: 0,
    navBarHeight: 48
  },
  lifetimes: {
    attached() {
      try {
        const appBaseInfo = wx.getAppBaseInfo()
        const windowInfo = wx.getWindowInfo()
        
        this.setData({
          statusBarHeight: appBaseInfo.statusBarHeight || 20,
          navBarHeight: 48
        })
      } catch (err) {
        console.error('获取系统信息失败：', err)
      }
    },
  },
  /**
   * 组件的方法列表
   */
  methods: {
    _showChange(show) {
      const animated = this.data.animated
      let displayStyle = ''
      if (animated) {
        displayStyle = `opacity: ${
          show ? '1' : '0'
        };transition:opacity 0.5s;`
      } else {
        displayStyle = `display: ${show ? '' : 'none'}`
      }
      this.setData({
        displayStyle
      })
    },
    back() {
      const data = this.data
      if (data.delta) {
        const currentPages = getCurrentPages();
        const currentPage = currentPages[currentPages.length - 1];
        // 如果页面有onBackTap方法，则调用
        if (currentPage && typeof currentPage.onBackTap === 'function') {
          currentPage.onBackTap();
        } else {
          wx.navigateBack({
            delta: data.delta
          });
        }
      }
      this.triggerEvent('back', { delta: data.delta }, {})
    }
  },
})
