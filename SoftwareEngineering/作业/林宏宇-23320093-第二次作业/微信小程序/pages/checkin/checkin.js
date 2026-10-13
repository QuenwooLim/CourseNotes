const cloud = require('../../utils/cloud')

Page({
  data: {
    taskId: '',
    task: null,
    form: {
      // 非集训打卡内容
      exercise1: { name: '', content: '' },
      exercise2: { name: '', content: '' },
      exercise3: { name: '', content: '' },
      exercise4: { name: '', content: '' },
      exercise5: { name: '', content: '' },
      exercise6: { name: '', content: '' },
      // 集训打卡内容
      trainingContent: '',
      // 公共内容
      remark: '',
      date: new Date().toISOString().split('T')[0],
      imageUrl: '', // 图片URL
      imagePath: '' // 本地图片路径
    },
    isView: false,
    isTrainingCheckin: false, // 是否为集训打卡
    userInfo: null,
    loading: false,
    isJoining: false, // 添加是否参与任务标记
    viewingUserName: '', // 记录正在查看的用户名
    hasSubmitted: false // 添加一个标记来记录是否已完成打卡提交
  },

  // 处理导航栏返回按钮点击
  onBackTap: function() {
    // 如果是参与任务模式且未打卡完成，需要提示用户
    if (this.data.isJoining && !this.data.hasSubmitted) {
      wx.showModal({
        title: '提示',
        content: '您尚未完成打卡，退出将不会记录为参与任务。确定要退出吗？',
        confirmText: '确定退出',
        cancelText: '继续打卡',
        success: (res) => {
          if (res.confirm) {
            wx.navigateBack({
              delta: 1
            })
          }
        }
      })
      return
    }
    
    // 正常退出
    wx.navigateBack({
      delta: 1
    })
  },

  onLoad: function(options) {
    console.log('打卡页面加载，参数:', options)
    
    if (!cloud || !cloud.task || !cloud.checkin) {
      console.error('cloud module not properly loaded')
      wx.showToast({
        title: '加载失败，请重试',
        icon: 'none'
      })
      return
    }
    
    const taskId = options.id || ''
    const recordId = options.recordId || ''
    const isView = options.mode === 'view' || options.view === '1'
    const isJoining = options.join === '1' // 添加是否参与任务标记
    const userName = options.userName ? decodeURIComponent(options.userName) : '' // 查看特定用户的打卡
    
    this.setData({
      taskId,
      recordId,
      isView,
      isJoining,
      viewingUserName: userName, // 记录正在查看的用户名
      hasSubmitted: false // 添加一个标记来记录是否已完成打卡提交
    })
    
    // 更新导航栏标题，如果有用户名则显示
    if (isView && userName) {
      wx.setNavigationBarTitle({
        title: `${userName}的打卡详情`
      })
    }
    
    // 设置当前日期
    this.setCurrentDate()
    
    // 加载用户信息
    this.loadUserInfo()
    
    // 如果是从历史记录直接进入，先加载打卡记录
    if (recordId) {
      this.loadCheckinRecordById(recordId)
    } 
    // 如果有任务ID，加载任务详情
    else if (taskId) {
      this.loadTaskDetail(taskId)
    }
  },

  // 设置当前日期
  setCurrentDate: function() {
    const now = new Date()
    const year = now.getFullYear()
    const month = String(now.getMonth() + 1).padStart(2, '0')
    const day = String(now.getDate()).padStart(2, '0')
    const today = `${year}-${month}-${day}`
    
    this.setData({
      'form.date': today
    })
  },

  // 加载用户信息
  loadUserInfo: function() {
    const app = getApp()
    let userInfo = app.globalData.userInfo
    
    if (!userInfo) {
      userInfo = wx.getStorageSync('userInfo') || null
      if (userInfo) {
        app.globalData.userInfo = userInfo
      }
    }
    
    this.setData({ userInfo })
    
    // 检查登录状态
    if (!userInfo || !userInfo._id) {
      wx.showModal({
        title: '提示',
        content: '请先登录后再打卡',
        showCancel: false,
        success: () => {
          wx.switchTab({
            url: '/pages/profile/profile'
          })
        }
      })
    }
  },

  // 加载任务详情
  loadTaskDetail: function(taskId) {
    wx.showLoading({
      title: '加载中...'
    })
    
    cloud.task.getTaskDetail(taskId)
      .then(task => {
        console.log('获取到任务详情:', task)
        
        // 判断是否为集训打卡
        const isTrainingCheckin = task.checkinType && (task.checkinType.includes('集训上午') || task.checkinType.includes('集训下午'))
        
        // 检查任务是否已开始
        let isNotStarted = false;
        if (task.startDateTime) {
          try {
            const startTime = new Date(task.startDateTime.replace(/-/g, '/'));
            const now = new Date();
            isNotStarted = startTime > now;
            console.log(`任务开始时间: ${task.startDateTime}, 是否未开始: ${isNotStarted}`);
            
            if (isNotStarted && !this.data.isView) {
              // 如果任务尚未开始且不是查看模式，显示提示并返回
              wx.hideLoading();
              wx.showModal({
                title: '提示',
                content: '该任务尚未开始，暂不能打卡',
                showCancel: false,
                success: () => {
                  wx.navigateBack();
                }
              });
              return;
            }
          } catch (e) {
            console.error('解析任务开始时间出错:', e);
          }
        }
        
        this.setData({ 
          task,
          isTrainingCheckin
        })
        
        // 如果是查看模式，加载打卡记录
        if (this.data.isView) {
          this.loadCheckinRecord(taskId)
        }
        
        wx.hideLoading()
      })
      .catch(err => {
        console.error('获取任务详情失败:', err)
        
        wx.hideLoading()
        wx.showToast({
          title: '加载任务失败，请重试',
          icon: 'none'
        })
      })
  },

  // 加载打卡记录（查看模式）
  loadCheckinRecord: function(taskId) {
    if (!this.data.isView) return

    const recordId = this.data.recordId

    if (recordId) {
      // 如果有指定记录ID，直接加载该记录
      wx.showLoading({
        title: '加载记录中...'
      })

      // 调用云函数获取记录详情
      cloud.checkin.getCheckinDetail(recordId)
        .then(data => {
          wx.hideLoading()
          
          if (data) {
            // 解析打卡内容，兼容新旧格式
            this.parseCheckinData(data)
          } else {
            wx.showToast({
              title: '加载记录失败',
              icon: 'none'
            })
          }
        })
        .catch(err => {
          console.error('获取打卡记录失败:', err)
          wx.hideLoading()
          wx.showToast({
            title: '加载记录失败',
            icon: 'none'
          })
        })
    } else {
      // 如果没有指定记录ID，检查今天是否已经打卡
      const today = new Date().toISOString().split('T')[0]
      
      wx.cloud.callFunction({
        name: 'checkin',
        data: {
          type: 'checkStatus',
          taskId: taskId,
          date: today
        }
      })
      .then(res => {
        if (res.result && res.result.code === 0 && res.result.data.hasCheckedIn && res.result.data.record) {
          // 解析打卡内容
          this.parseCheckinData(res.result.data.record)
        } else {
          // 没有找到记录，使用默认值
          if (this.data.isTrainingCheckin) {
            this.setData({
              form: {
                ...this.data.form,
                trainingContent: '今天暂无训练内容',
                remark: '无备注信息',
                date: today
              }
            })
          } else {
            this.setData({
              form: {
                ...this.data.form,
                exercise1: { name: '', content: '' },
                exercise2: { name: '', content: '' },
                exercise3: { name: '', content: '' },
                exercise4: { name: '', content: '' },
                exercise5: { name: '', content: '' },
                exercise6: { name: '', content: '' },
                remark: '无备注信息',
                date: today
              }
            })
          }
        }
      })
      .catch(err => {
        console.error('检查打卡状态失败:', err)
        // 出错时使用默认值
        this.setData({
          form: {
            ...this.data.form,
            remark: '获取失败，请重试',
            date: today
          }
        })
      })
    }
  },

  // 解析打卡记录数据（兼容新旧格式）
  parseCheckinData: function(data) {
    try {
      let formData = { ...this.data.form }
      formData.date = data.date || this.data.form.date
      formData.remark = data.remark || ''
      
      // 处理图片URL - 优先使用 fileID 获取临时链接
      if (data.fileID) {
        formData.fileID = data.fileID // 保存fileID，供后续使用
        this.refreshImageUrl(data.fileID) // 异步获取临时链接并更新imageUrl
      } else if (data.imageUrl) {
        // 兼容旧数据可能只存了 imageUrl 的情况
        formData.imageUrl = data.imageUrl
      }
      
      // 处理打卡内容
      if (this.data.isTrainingCheckin) {
        // 集训打卡
        formData.trainingContent = data.trainingContent || data.training || data.content || ''
      } else {
        // 非集训打卡 - 检查是否为新格式
        if (data.exercises && Array.isArray(data.exercises)) {
          // 新格式 - 使用exercises数组
          for (let i = 0; i < 6; i++) {
            const exercise = data.exercises[i] || { name: '', content: '' }
            formData[`exercise${i+1}`] = exercise
          }
        } else if (data.content) {
          // 旧格式 - 尝试解析content字段
          try {
            // 尝试解析为JSON
            const exercises = JSON.parse(data.content)
            if (exercises && Array.isArray(exercises)) {
              for (let i = 0; i < 6 && i < exercises.length; i++) {
                formData[`exercise${i+1}`] = exercises[i]
              }
            } else {
              // 如果不是数组，使用旧格式的内容
              formData.exercise1 = { name: '训练动作', content: data.content || '' }
              formData.exercise2 = { name: '附加训练', content: data.training || '' }
            }
          } catch (e) {
            // 如果解析失败，使用旧格式的内容
            formData.exercise1 = { name: '训练动作', content: data.content || '' }
            formData.exercise2 = { name: '附加训练', content: data.training || '' }
          }
        }
      }
      
      this.setData({ form: formData })
    } catch (e) {
      console.error('解析打卡记录数据失败:', e)
    }
  },

  // 刷新图片临时链接
  refreshImageUrl: function(fileID) {
    if (!fileID) return;
    
    console.log('刷新图片URL, fileID:', fileID);
    
    wx.cloud.getTempFileURL({
      fileList: [fileID],
      success: res => {
        console.log('获取临时链接成功:', res);
        if (res.fileList && res.fileList.length > 0) {
          const tempFileURL = res.fileList[0].tempFileURL;
          if (tempFileURL) {
            this.setData({
              'form.imageUrl': tempFileURL
            });
          }
        }
      },
      fail: err => {
        console.error('获取图片临时链接失败:', err);
      }
    });
  },

  // 通过记录ID加载打卡记录
  loadCheckinRecordById: function(recordId) {
    wx.showLoading({
      title: '加载记录中...'
    })

    // 调用云函数获取记录详情
    cloud.checkin.getCheckinDetail(recordId)
      .then(data => {
        console.log('获取到打卡记录:', data)
        
        // 如果记录包含任务信息，同步任务信息
        if (data.taskId) {
          this.setData({ taskId: data.taskId })
          this.loadTaskDetail(data.taskId)
        } else {
          wx.hideLoading()
        }
        
        // 解析打卡内容
        this.parseCheckinData(data)
      })
      .catch(err => {
        console.error('获取打卡记录失败:', err)
        wx.hideLoading()
        
        // 显示更详细的错误信息
        let errMsg = '加载记录失败';
        if (err.errCode === 403) {
          errMsg = '无权限查看该记录';
        } else if (err.message) {
          errMsg = `加载失败: ${err.message}`;
        }
        
        wx.showToast({
          title: errMsg,
          icon: 'none',
          duration: 3000
        })
        
        // 1.5秒后返回上一页
        setTimeout(() => {
          wx.navigateBack();
        }, 1500);
      })
  },

  // 日期选择变化
  onDateChange: function(e) {
    this.setData({
      'form.date': e.detail.value
    })
  },

  // 非集训打卡表单输入处理 - 训练动作1
  onExercise1NameInput: function(e) {
    this.setData({
      'form.exercise1.name': e.detail.value
    })
  },
  
  onExercise1ContentInput: function(e) {
    this.setData({
      'form.exercise1.content': e.detail.value
    })
  },
  
  // 训练动作2
  onExercise2NameInput: function(e) {
    this.setData({
      'form.exercise2.name': e.detail.value
    })
  },
  
  onExercise2ContentInput: function(e) {
    this.setData({
      'form.exercise2.content': e.detail.value
    })
  },
  
  // 训练动作3
  onExercise3NameInput: function(e) {
    this.setData({
      'form.exercise3.name': e.detail.value
    })
  },
  
  onExercise3ContentInput: function(e) {
    this.setData({
      'form.exercise3.content': e.detail.value
    })
  },
  
  // 训练动作4
  onExercise4NameInput: function(e) {
    this.setData({
      'form.exercise4.name': e.detail.value
    })
  },
  
  onExercise4ContentInput: function(e) {
    this.setData({
      'form.exercise4.content': e.detail.value
    })
  },
  
  // 训练动作5
  onExercise5NameInput: function(e) {
    this.setData({
      'form.exercise5.name': e.detail.value
    })
  },
  
  onExercise5ContentInput: function(e) {
    this.setData({
      'form.exercise5.content': e.detail.value
    })
  },
  
  // 训练动作6
  onExercise6NameInput: function(e) {
    this.setData({
      'form.exercise6.name': e.detail.value
    })
  },
  
  onExercise6ContentInput: function(e) {
    this.setData({
      'form.exercise6.content': e.detail.value
    })
  },
  
  // 集训打卡内容输入
  onTrainingContentInput: function(e) {
    this.setData({
      'form.trainingContent': e.detail.value
    })
  },

  // 备注输入变化
  onRemarkInput: function(e) {
    this.setData({
      'form.remark': e.detail.value
    })
  },

  // 图片上传相关
  chooseImage: function() {
    wx.chooseImage({
      count: 1,
      sizeType: ['compressed'],
      sourceType: ['album', 'camera'],
      success: res => {
        const tempFilePath = res.tempFilePaths[0]
        
        this.setData({
          'form.imagePath': tempFilePath,
          'form.imageUrl': tempFilePath // 先用本地路径预览
        })
      }
    })
  },
  
  // 删除图片
  deleteImage: function() {
    this.setData({
      'form.imagePath': '',
      'form.imageUrl': ''
    })
  },
  
  // 预览图片
  previewImage: function() {
    if (!this.data.form.imageUrl) return;
    
    // 检查是否是云存储fileID
    if (this.data.form.imageUrl.startsWith('cloud://') || this.data.form.imageUrl.startsWith('https://')) {
      wx.showLoading({ title: '加载图片...' });
      
      // 获取新的临时链接以确保预览正常
      wx.cloud.getTempFileURL({
        fileList: [this.data.form.imageUrl],
        success: res => {
          wx.hideLoading();
          if (res.fileList && res.fileList.length > 0) {
            const tempFileURL = res.fileList[0].tempFileURL;
            if (tempFileURL) {
              // 更新图片URL
              this.setData({
                'form.imageUrl': tempFileURL
              });
              
              // 预览图片
              wx.previewImage({
                urls: [tempFileURL],
                current: tempFileURL
              });
            } else {
              // 如果获取临时链接失败，尝试直接使用原URL
              wx.previewImage({
                urls: [this.data.form.imageUrl],
                current: this.data.form.imageUrl
              });
            }
          }
        },
        fail: err => {
          console.error('获取预览图片链接失败:', err);
          wx.hideLoading();
          
          // 尝试直接预览
          wx.previewImage({
            urls: [this.data.form.imageUrl],
            current: this.data.form.imageUrl
          });
        }
      });
    } else {
      // 本地图片直接预览
      wx.previewImage({
        urls: [this.data.form.imageUrl],
        current: this.data.form.imageUrl
      });
    }
  },
  
  // 上传图片到云存储
  uploadImage: function() {
    return new Promise((resolve, reject) => {
      if (!this.data.form.imagePath) {
        // 没有选择图片，直接返回成功
        resolve(null)
        return
      }
      
      const filePath = this.data.form.imagePath
      const cloudPath = `checkin_images/${this.data.userInfo._id}_${new Date().getTime()}${filePath.match(/\.[^.]+?$/)[0]}`
      
      wx.showLoading({ title: '上传图片中...' })
      
      wx.cloud.uploadFile({
        cloudPath,
        filePath,
        success: res => {
          console.log('图片上传成功:', res)
          wx.hideLoading()
          // 只返回 fileID
          resolve(res.fileID)
        },
        fail: err => {
          console.error('图片上传失败:', err)
          wx.hideLoading()
          reject(err)
        }
      })
    })
  },

  // 提交打卡
  handleSubmit: function() {
    if (this.data.loading) return
    
    // 检查用户是否已登录
    if (!this.data.userInfo || !this.data.userInfo._id) {
      wx.showToast({
        title: '请先登录',
        icon: 'none'
      })
      return
    }
    
    // 检查任务是否已开始
    if (this.data.task && this.data.task.startDateTime) {
      try {
        const startTime = new Date(this.data.task.startDateTime.replace(/-/g, '/'));
        const now = new Date();
        if (startTime > now) {
          wx.showToast({
            title: '任务尚未开始，不能打卡',
            icon: 'none'
          });
          return;
        }
      } catch (e) {
        console.error('解析任务开始时间出错:', e);
      }
    }
    
    // 表单验证 - 根据打卡类型进行不同验证
    if (this.data.isTrainingCheckin) {
      // 集训打卡验证
      if (!this.data.form.trainingContent.trim()) {
        wx.showToast({
          title: '请填写训练内容',
          icon: 'none'
        })
        return
      }
    } else {
      // 非集训打卡验证 - 前三个训练动作必填
      if (!this.data.form.exercise1.name.trim() || !this.data.form.exercise1.content.trim()) {
        wx.showToast({
          title: '请完善训练动作一',
          icon: 'none'
        })
        return
      }
      
      if (!this.data.form.exercise2.name.trim() || !this.data.form.exercise2.content.trim()) {
        wx.showToast({
          title: '请完善训练动作二',
          icon: 'none'
        })
        return
      }
      
      if (!this.data.form.exercise3.name.trim() || !this.data.form.exercise3.content.trim()) {
        wx.showToast({
          title: '请完善训练动作三',
          icon: 'none'
        })
        return
      }
    }
    
    this.setData({ loading: true })
    wx.showLoading({ title: '提交中...' })
    
    // 先上传图片（如果有）
    this.uploadImage()
      .then(imageResult => {
        // 准备打卡数据
        let checkinData = {
          taskId: this.data.taskId,
          date: this.data.form.date,
          remark: this.data.form.remark,
          userInfo: {
            _id: this.data.userInfo._id,
            nickName: this.data.userInfo.nickName,
            avatarUrl: this.data.userInfo.avatarUrl
          }
        }
        
        // 添加图片信息（如果上传成功）
        if (imageResult) {
          // 只保存 fileID
          checkinData.fileID = imageResult
          // 不再保存 imageUrl
          // checkinData.imageUrl = imageResult.imageUrl
        }
        
        // 根据打卡类型设置不同的内容
        if (this.data.isTrainingCheckin) {
          // 集训打卡
          checkinData.trainingContent = this.data.form.trainingContent
          // 兼容旧接口
          checkinData.content = this.data.form.trainingContent
          checkinData.training = this.data.form.trainingContent
        } else {
          // 非集训打卡 - 将训练动作打包为数组
          const exercises = [
            this.data.form.exercise1,
            this.data.form.exercise2,
            this.data.form.exercise3,
            this.data.form.exercise4,
            this.data.form.exercise5,
            this.data.form.exercise6
          ]
          
          // 新格式 - 保存为数组
          checkinData.exercises = exercises
          
          // 兼容旧接口 - 将数据序列化为JSON字符串保存在content字段中
          checkinData.content = JSON.stringify(exercises)
          
          // 使用第一个训练动作作为training字段，兼容旧接口
          checkinData.training = `${this.data.form.exercise1.name}: ${this.data.form.exercise1.content}`
        }
        
        return cloud.checkin.submitCheckin(checkinData)
      })
      .then(() => {
        // 标记打卡已提交成功
        this.setData({ hasSubmitted: true })
        
        // 更新系统中的参与状态是在云函数中处理的，不需要在这里调用 joinTask
        // 如果是首次参与任务，原本是这里需要调用joinTask云函数
        // 但现在我们已在打卡云函数中处理了参与任务的逻辑
        // 以下代码可以删除或保留作为注释
        /*
        if (this.data.isJoining) {
          return wx.cloud.callFunction({
            name: 'task',
            data: {
              type: 'join',
              taskId: this.data.taskId,
              userId: this.data.userInfo._id
            }
          })
          .then(res => {
            console.log('参与任务成功:', res)
            return Promise.resolve()
          })
          .catch(err => {
            console.error('参与任务失败:', err)
            return Promise.resolve()
          })
        }
        */
        return Promise.resolve()
      })
      .then(() => {
        wx.hideLoading()
        this.setData({ loading: false })
        
        // 更新本地任务参与状态
        if (this.data.isJoining) {
          const userTasks = wx.getStorageSync('userTasks') || {}
          userTasks[this.data.taskId] = true
          wx.setStorageSync('userTasks', userTasks)
        }
        
        wx.showToast({
          title: '打卡成功',
          icon: 'success'
        })
        
        // 延迟返回
        setTimeout(() => {
          wx.navigateBack()
        }, 1500)
      })
      .catch(err => {
        console.error('打卡失败:', err)
        wx.hideLoading()
        this.setData({ loading: false })
        
        wx.showToast({
          title: err.message || '打卡失败',
          icon: 'none'
        })
      })
  },

  // 处理图片加载错误
  handleImageError: function(e) {
    console.error('图片加载错误:', e);
    
    // 如果是查看模式且有fileID，尝试刷新图片URL
    if (this.data.isView && this.data.recordId) {
      // 重新加载记录以获取fileID
      wx.cloud.callFunction({
        name: 'checkin',
        data: {
          type: 'getDetail',
          recordId: this.data.recordId
        }
      }).then(res => {
        if (res.result && res.result.code === 0 && res.result.data) {
          const fileID = res.result.data.fileID;
          if (fileID) {
            this.refreshImageUrl(fileID);
          }
        }
      }).catch(err => {
        console.error('重新获取记录失败:', err);
      });
    }
  },

  // 当页面卸载时，如果是参与任务模式但未完成打卡，清理相关状态
  onUnload: function() {
    if (this.data.isJoining && !this.data.hasSubmitted) {
      console.log('用户未完成打卡就退出了')
      
      // 可以在这里添加额外的清理逻辑，如果需要的话
      // 例如标记任务未参与等操作
      // 这里不需要做特殊处理，因为我们已经修改了云函数逻辑
      // 只有打卡完成后才会将用户添加到参与者列表
    }
  }
}) 