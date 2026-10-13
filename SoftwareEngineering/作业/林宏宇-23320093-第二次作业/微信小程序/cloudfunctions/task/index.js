const cloud = require('wx-server-sdk')

cloud.init({
  env: cloud.DYNAMIC_CURRENT_ENV
})

const db = cloud.database()
const _ = db.command

// 定义打卡时间周期选项
const TIME_PERIODS = {
  WEEKS: ['第1周', '第2周', '第3周', '第4周', '第5周', '第6周', '第7周', '第8周', '第9周', '第10周', 
          '第11周', '第12周', '第13周', '第14周', '第15周', '第16周', '第17周', '第18周'],
  TRAINING: ['寒训', '暑训']
}

// 定义打卡类型选项
const CHECKIN_TYPES = ['非集训', '集训上午', '集训下午']

// 云函数入口函数
exports.main = async (event, context) => {
  const { type, data, page = 1, pageSize = 10, taskId, userId, includeParticipantDetails } = event
  const wxContext = cloud.getWXContext()

  switch (type) {
    case 'create':
      return createTask(data, userId)
    case 'list':
      return getTaskList(page, pageSize)
    case 'detail':
      return getTaskDetail(taskId, includeParticipantDetails)
    case 'join':
      return joinTask(taskId, userId)
    case 'getAll':
      return getAllTasks()
    case 'update':
      return updateTask(data, userId)
    case 'delete':
      return deleteTask(taskId, userId)
    case 'getTimePeriods':
      return getTimePeriods()
    case 'getCheckinTypes':
      return getCheckinTypes()
    default:
      return {
        code: 400,
        message: '未知的操作类型'
      }
  }
}

// 获取时间周期选项
function getTimePeriods() {
  console.log('获取时间周期选项，返回:', TIME_PERIODS);
  
  // 确保TIME_PERIODS结构完整
  if (!TIME_PERIODS.WEEKS || !TIME_PERIODS.TRAINING) {
    console.warn('TIME_PERIODS结构不完整，使用默认值');
  }
  
  return {
    code: 0,
    data: TIME_PERIODS,
    message: '获取成功'
  }
}

// 获取打卡类型选项
function getCheckinTypes() {
  console.log('获取打卡类型选项，返回:', CHECKIN_TYPES);
  
  return {
    code: 0,
    data: CHECKIN_TYPES,
    message: '获取成功'
  }
}

// 创建任务
async function createTask(data, userId) {
  if (!userId) {
    return {
      code: 403,
      message: '用户未登录'
    }
  }

  // 验证必填字段
  if (!data.title) {
    return {
      code: 400,
      message: '任务标题不能为空'
    }
  }

  if (!data.timePeriod) {
    return {
      code: 400,
      message: '时间周期不能为空'
    }
  }

  if (!data.checkinType) {
    return {
      code: 400,
      message: '打卡类型不能为空'
    }
  }

  if (!data.startTime || !data.endTime) {
    return {
      code: 400,
      message: '打卡起始时间和截止时间不能为空'
    }
  }

  try {
    // 确保创建者ID与当前用户一致
    if (data.createdBy !== userId) {
      data.createdBy = userId
    }

    // 获取创建者信息
    const userInfo = await db.collection('users').doc(userId).get()
    const creatorName = userInfo.data ? userInfo.data.nickName || '未知用户' : '未知用户'

    const result = await db.collection('tasks').add({
      data: {
        ...data,
        creatorName,
        // 新增的时间周期和打卡类型字段
        timePeriod: data.timePeriod, // 第1-18周/寒训/暑训
        checkinType: data.checkinType, // 非集训/集训上午/集训下午
        startTime: data.startTime, // 打卡起始时间
        endTime: data.endTime, // 打卡截止时间
        createTime: db.serverDate(),
        updateTime: db.serverDate(),
        status: 'active',
        participants: [],
        completedCount: 0
      }
    })

    // 记录任务创建日志
    await db.collection('activity_logs').add({
      data: {
        type: 'task_create',
        userId: userId,
        taskId: result._id,
        taskTitle: data.title,
        timestamp: db.serverDate()
      }
    })

    return {
      code: 0,
      data: result._id,
      message: '创建成功'
    }
  } catch (err) {
    console.error('创建任务失败：', err)
    return {
      code: 500,
      message: '创建任务失败：' + err.message
    }
  }
}

// 获取任务列表
async function getTaskList(page, pageSize) {
  try {
    const skip = (page - 1) * pageSize
    
    // 只获取活跃状态的任务，按截止时间排序
    const result = await db.collection('tasks')
      .where({
        status: 'active'
      })
      .orderBy('endTime', 'desc')
      .skip(skip)
      .limit(pageSize)
      .get()
    
    const total = await db.collection('tasks')
      .where({ status: 'active' })
      .count()
    
    return {
      code: 0,
      data: {
        list: result.data,
        total: total.total,
        page,
        pageSize
      },
      message: '获取成功'
    }
  } catch (err) {
    console.error('获取任务列表失败：', err)
    return {
      code: 500,
      message: '获取任务列表失败：' + err.message
    }
  }
}

// 获取任务详情
async function getTaskDetail(taskId, includeParticipantDetails = false) {
  if (!taskId) {
    return {
      code: 400,
      message: '任务ID不能为空'
    }
  }

  try {
    const task = await db.collection('tasks').doc(taskId).get()
    
    if (!task.data) {
      return {
        code: 404,
        message: '任务不存在'
      }
    }
    
    // 如果需要获取参与者详细信息
    if (includeParticipantDetails && task.data.participants && task.data.participants.length > 0) {
      // 获取所有参与者的详细信息
      const participantsDetails = [];
      
      // 使用Promise.all同时查询所有参与者信息
      const promises = task.data.participants.map(async (userId) => {
        try {
          const userInfo = await db.collection('users').doc(userId).get();
          if (userInfo.data) {
            return {
              _id: userId,
              avatarUrl: userInfo.data.avatarUrl || '/images/default-avatar.png',
              nickName: userInfo.data.nickName || '未知用户'
            };
          }
          return {
            _id: userId,
            avatarUrl: '/images/default-avatar.png',
            nickName: '未知用户'
          };
        } catch (err) {
          console.error(`获取用户 ${userId} 信息失败:`, err);
          return {
            _id: userId,
            avatarUrl: '/images/default-avatar.png',
            nickName: '未知用户'
          };
        }
      });
      
      // 等待所有用户查询完成
      task.data.participants = await Promise.all(promises);
    }
    
    return {
      code: 0,
      data: task.data,
      message: '获取成功'
    }
  } catch (err) {
    console.error('获取任务详情失败：', err)
    return {
      code: 500,
      message: '获取任务详情失败：' + err.message
    }
  }
}

// 参与任务
async function joinTask(taskId, userId) {
  if (!taskId || !userId) {
    return {
      code: 400,
      message: '参数不完整'
    }
  }

  try {
    // 检查任务是否存在
    const task = await db.collection('tasks').doc(taskId).get()
    
    if (!task.data) {
      return {
        code: 404,
        message: '任务不存在'
      }
    }
    
    // 检查是否已参与
    if (task.data.participants && task.data.participants.includes(userId)) {
      return {
        code: 400,
        message: '您已参与此任务'
      }
    }
    
    // 检查任务是否已过期
    const now = new Date()
    const endTime = new Date(task.data.endTime)
    if (now > endTime) {
      return {
        code: 400,
        message: '该任务已经截止，无法参与'
      }
    }
    
    // 仍然记录用户尝试参与任务的活动日志
    await db.collection('activity_logs').add({
      data: {
        type: 'task_join_attempt', // 修改类型以区分尝试参与和实际参与
        userId: userId,
        taskId: taskId,
        taskTitle: task.data.title,
        timestamp: db.serverDate()
      }
    })
    
    // 获取当前用户信息，以便返回给前端显示
    let userInfo = null;
    try {
      const userRecord = await db.collection('users').doc(userId).get();
      if (userRecord.data) {
        userInfo = {
          _id: userId,
          avatarUrl: userRecord.data.avatarUrl || '/images/default-avatar.png',
          nickName: userRecord.data.nickName || '未知用户'
        };
      }
    } catch (err) {
      console.error('获取用户信息失败：', err);
      userInfo = {
        _id: userId,
        avatarUrl: '/images/default-avatar.png',
        nickName: '未知用户'
      };
    }
    
    return {
      code: 0,
      data: {
        userInfo
      },
      message: '参与成功'
    }
  } catch (err) {
    console.error('参与任务失败：', err)
    return {
      code: 500,
      message: '参与任务失败：' + err.message
    }
  }
}

// 获取所有任务
async function getAllTasks() {
  try {
    // 获取所有任务，不分页
    const result = await db.collection('tasks')
      .orderBy('endTime', 'desc')
      .get()
    
    return {
      code: 0,
      data: result.data,
      message: '获取成功'
    }
  } catch (err) {
    console.error('获取所有任务失败：', err)
    return {
      code: 500,
      message: '获取所有任务失败：' + err.message
    }
  }
}

// 更新任务
async function updateTask(data, userId) {
  if (!data || !data._id) {
    return {
      code: 400,
      message: '参数不完整'
    }
  }

  try {
    // 检查任务是否存在
    const task = await db.collection('tasks').doc(data._id).get()
    
    if (!task.data) {
      return {
        code: 404,
        message: '任务不存在'
      }
    }
    
    // 检查是否有权限更新（只有创建者可以更新）
    if (task.data.createdBy !== userId) {
      return {
        code: 403,
        message: '无权限更新此任务'
      }
    }

    // 验证时间周期和打卡类型
    if (data.timePeriod && !isValidTimePeriod(data.timePeriod)) {
      return {
        code: 400,
        message: '无效的时间周期'
      }
    }

    if (data.checkinType && !isValidCheckinType(data.checkinType)) {
      return {
        code: 400,
        message: '无效的打卡类型'
      }
    }
    
    // 更新任务信息
    const taskId = data._id
    delete data._id // 移除_id字段，避免更新时出错
    
    await db.collection('tasks').doc(taskId).update({
      data: {
        ...data,
        updateTime: db.serverDate()
      }
    })
    
    return {
      code: 0,
      message: '更新成功'
    }
  } catch (err) {
    console.error('更新任务失败：', err)
    return {
      code: 500,
      message: '更新任务失败：' + err.message
    }
  }
}

// 删除任务
async function deleteTask(taskId, userId) {
  if (!taskId) {
    return {
      code: 400,
      message: '任务ID不能为空'
    }
  }

  try {
    // 检查任务是否存在
    const task = await db.collection('tasks').doc(taskId).get()
    
    if (!task.data) {
      return {
        code: 404,
        message: '任务不存在'
      }
    }
    
    // 检查用户是否是队长
    let isCaptain = false;
    try {
      const userInfo = await db.collection('users').doc(userId).get();
      isCaptain = userInfo.data && userInfo.data.isCaptain === true;
    } catch (err) {
      console.error('获取用户队长状态失败:', err);
      // 如果查询失败，默认不是队长
      isCaptain = false;
    }
    
    // 检查是否有权限删除（创建者或队长可以删除）
    if (task.data.createdBy !== userId && !isCaptain) {
      return {
        code: 403,
        message: '无权限删除此任务'
      }
    }
    
    // 删除任务
    await db.collection('tasks').doc(taskId).remove()
    
    // 记录活动日志
    await db.collection('activity_logs').add({
      data: {
        type: 'task_delete',
        userId: userId,
        taskId: taskId,
        taskTitle: task.data.title,
        timestamp: db.serverDate()
      }
    })
    
    return {
      code: 0,
      message: '删除成功'
    }
  } catch (err) {
    console.error('删除任务失败：', err)
    return {
      code: 500,
      message: '删除任务失败：' + err.message
    }
  }
}

// 验证时间周期是否有效
function isValidTimePeriod(timePeriod) {
  return TIME_PERIODS.WEEKS.includes(timePeriod) || TIME_PERIODS.TRAINING.includes(timePeriod)
}

// 验证打卡类型是否有效
function isValidCheckinType(checkinType) {
  return CHECKIN_TYPES.includes(checkinType)
} 