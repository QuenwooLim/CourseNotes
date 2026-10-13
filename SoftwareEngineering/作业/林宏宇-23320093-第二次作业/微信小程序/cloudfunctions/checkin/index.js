const cloud = require('wx-server-sdk')

cloud.init({
  env: cloud.DYNAMIC_CURRENT_ENV
})

const db = cloud.database()
const _ = db.command
const $ = db.command.aggregate

// 云函数入口函数
exports.main = async (event, context) => {
  const { type, data, rankingType = 'all', taskId, date, recordId, page = 1, pageSize = 10, userId, filters } = event
  const wxContext = cloud.getWXContext()

  switch (type) {
    case 'submit':
      return submitCheckin(data, userId)
    case 'ranking':
      return getRanking(rankingType, filters)
    case 'userStats':
      return getUserStats(userId)
    case 'checkStatus':
      return checkCheckinStatus(taskId, date, userId)
    case 'getRecord':
      return getCheckinRecord(recordId, userId)
    case 'userHistory':
      return getUserHistory(userId, page, pageSize)
    case 'getMemberCheckins':
      return getMemberCheckins(event)
    case 'getUserLastCheckin':
      return getUserLastCheckin(taskId, userId)
    default:
      return {
        code: 400,
        message: '未知的操作类型'
      }
  }
}

// 提交打卡
async function submitCheckin(data, userId) {
  try {
    // 使用传入的userId参数
    if (!userId) {
      return {
        code: 403,
        message: '用户未登录'
      }
    }

    // 检查是否已经打卡
    const existingCheckin = await db.collection('checkins')
      .where({
        userId: userId,
        date: data.date,
        taskId: data.taskId
      })
      .get()

    if (existingCheckin.data.length > 0) {
      return {
        code: 400,
        message: '今日已完成此任务打卡'
      }
    }

    // 获取打卡任务信息，用于判断打卡类型
    const taskInfo = await db.collection('tasks').doc(data.taskId).get()
    
    // 检查任务是否已经开始
    if (taskInfo.data && taskInfo.data.startDateTime) {
      const now = new Date()
      try {
        // 兼容不同日期格式
        const startTime = new Date(taskInfo.data.startDateTime.replace(/-/g, '/'))
        if (startTime > now) {
          return {
            code: 400,
            message: '任务尚未开始，不能打卡'
          }
        }
      } catch (e) {
        console.error('解析任务开始时间出错:', e)
        // 如果解析出错，退回到不检查开始时间的逻辑
      }
    }
    
    const isTrainingCheckin = taskInfo.data && taskInfo.data.checkinType && 
      (taskInfo.data.checkinType.includes('集训上午') || taskInfo.data.checkinType.includes('集训下午'))

    // 表单验证 - 根据打卡类型执行不同的验证
    if (isTrainingCheckin) {
      // 集训打卡验证
      if (!data.trainingContent && !data.content) {
      return {
        code: 400,
        message: '训练内容不能为空'
      }
    }
    } else {
      // 非集训打卡验证 - 检查是否有训练动作数据
      if (!data.exercises && !data.content) {
      return {
        code: 400,
          message: '训练内容不能为空'
        }
      }
    }

    // 创建打卡记录
    const checkinData = {
      taskId: data.taskId,
      userId: userId,
      // 兼容旧版字段
      content: data.content || '',
      training: data.training || '',
      // 新增字段
      trainingContent: data.trainingContent || data.content || '',
      exercises: data.exercises || [], // 训练动作数组
      remark: data.remark || '', // 设置默认值为空字符串，确保字段存在
      date: data.date,
      userInfo: data.userInfo,
      createTime: db.serverDate()
    }

    // 如果有图片信息，添加到记录中
    if (data.fileID) {
      checkinData.fileID = data.fileID
    }
    if (data.imageUrl) {
      checkinData.imageUrl = data.imageUrl
    }

    const result = await db.collection('checkins').add({
      data: checkinData
    })

    // 更新任务参与者计数和完成计数
    await db.collection('tasks').doc(data.taskId).update({
      data: {
        completedCount: _.inc(1),
        // 添加用户到参与者列表
        participants: _.addToSet(userId),
        updateTime: db.serverDate()
      }
    })
    
    // 记录活动日志 - 标记为实际完成参与
    await db.collection('activity_logs').add({
      data: {
        type: 'task_join_complete',
        userId: userId,
        taskId: data.taskId,
        checkinId: result._id,
        timestamp: db.serverDate()
      }
    })

    return {
      code: 0,
      data: {
        _id: result._id,
        ...checkinData
      },
      message: '打卡成功'
    }
  } catch (err) {
    console.error('打卡失败：', err)
    return {
      code: 500,
      message: '打卡失败：' + err.message
    }
  }
}

// 获取排行榜
async function getRanking(rankingType, filters = {}) {
  try {
    console.log('获取排行榜, rankingType:', rankingType, '筛选条件:', filters);
    
    // 初始化查询条件
    let timeQuery = {};
    
    // 根据类型筛选
    if (rankingType === 'today') {
      const today = new Date();
      today.setHours(0, 0, 0, 0);
      timeQuery.createTime = _.gte(today);
    } else if (rankingType === 'week') {
      const weekAgo = new Date();
      weekAgo.setDate(weekAgo.getDate() - 7);
      timeQuery.createTime = _.gte(weekAgo);
    }
    
    // 存储匹配的taskIds
    let matchedTaskIds = null;
    
    // 处理自定义筛选条件 - 通过先查询tasks集合获取符合条件的任务ID
    if (filters && (filters.timePeriod || filters.checkinType)) {
      console.log('应用筛选条件:', filters);
      
      let tasksQuery = {};
      
      if (filters.timePeriod) {
        console.log('应用时间周期筛选:', filters.timePeriod);
        tasksQuery.timePeriod = filters.timePeriod;
      }
      
      if (filters.checkinType) {
        console.log('应用打卡类型筛选:', filters.checkinType);
        tasksQuery.checkinType = filters.checkinType;
      }
      
      // 查询符合条件的任务ID
      const matchedTasks = await db.collection('tasks')
        .where(tasksQuery)
        .field({
          _id: true
        })
        .get();
      
      if (matchedTasks.data && matchedTasks.data.length > 0) {
        matchedTaskIds = matchedTasks.data.map(task => task._id);
        console.log('找到符合条件的任务数:', matchedTaskIds.length);
      } else {
        console.log('没有找到符合条件的任务');
        // 如果没有找到任何匹配的任务，返回空结果
        return {
          code: 0,
          data: {
            list: []
          },
          message: '没有符合筛选条件的数据'
        };
      }
    }
    
    // 构建最终查询
    let finalQuery = timeQuery;
    
    // 如果有匹配的任务ID，添加到查询条件
    if (matchedTaskIds !== null) {
      finalQuery.taskId = _.in(matchedTaskIds);
    }
    
    // 执行聚合查询
    console.log('执行聚合查询, 条件:', finalQuery);
    const result = await db.collection('checkins')
      .aggregate()
      .match(finalQuery)
      .group({
        _id: '$userId',
        count: $.sum(1)
      })
      .sort({
        count: -1
      })
      .limit(100)
      .end();
    
    // 如果没有数据，直接返回空数组
    if (!result.list || result.list.length === 0) {
      console.log('查询结果为空');
      return {
        code: 0,
        data: {
          list: []
        },
        message: '暂无数据'
      };
    }

    // 获取用户信息
    const userIds = result.list.map(item => item._id);
    
    // 使用_id查询用户
    const users = await db.collection('users')
      .where({
        _id: _.in(userIds)
      })
      .get();

    // 合并用户信息
    const rankingList = result.list.map(item => {
      const user = users.data.find(u => u._id === item._id);
      return {
        userId: item._id,
        _id: item._id, // 添加_id字段用于前端匹配当前用户
        nickName: user ? user.nickName : '未知用户',
        avatarUrl: user ? user.avatarUrl : '',
        checkInCount: item.count
      };
    });

    console.log('排行榜数据处理完成', rankingList.length, '条记录');

    return {
      code: 0,
      data: {
        list: rankingList
      },
      message: '获取成功'
    };
  } catch (err) {
    console.error('获取排行榜失败：', err);
    // 返回详细错误信息以便调试
    return {
      code: 500,
      message: '获取排行榜失败：' + err.message,
      stack: err.stack
    };
  }
}

// 获取用户统计数据
async function getUserStats(userId) {
  try {
    // 如果userId为空，直接返回默认数据
    if (!userId) {
      console.log('用户ID为空，返回默认数据')
      return {
        code: 0,
        data: {
          totalCheckins: 0,
          rank: '未上榜',
          recentCheckins: []
        },
        message: '获取成功(默认数据)'
      }
    }
    
    console.log('获取用户统计数据, userId:', userId)
    
    // 获取用户打卡总数
    let totalCount = { total: 0 }
    try {
      totalCount = await db.collection('checkins')
        .where({ userId })
        .count()
    } catch (countErr) {
      console.error('获取打卡总数失败:', countErr)
    }
    
    // 获取用户排名
    let rank = '未上榜'
    try {
      // 修改聚合查询方式
      const rankResult = await db.collection('checkins')
        .aggregate()
        .group({
          _id: '$userId',
          count: $.sum(1)
        })
        .sort({
          count: -1
        })
        .end()
      
      // 计算排名
      rankResult.list.forEach((item, index) => {
        if (item._id === userId) {
          rank = index + 1
        }
      })
    } catch (rankErr) {
      console.error('获取排名失败:', rankErr)
    }
    
    // 获取最近打卡记录
    let recentCheckins = []
    try {
      const recentResult = await db.collection('checkins')
        .where({ userId })
        .orderBy('createTime', 'desc')
        .limit(5)
        .get()
      
      recentCheckins = recentResult.data || []
    } catch (recentErr) {
      console.error('获取最近打卡记录失败:', recentErr)
    }
    
    return {
      code: 0,
      data: {
        totalCheckins: totalCount.total || 0,
        rank: rank,
        recentCheckins: recentCheckins
      },
      message: '获取成功'
    }
  } catch (err) {
    console.error('获取用户统计数据失败：', err)
    return {
      code: 0,
      data: {
        totalCheckins: 0,
        rank: '未上榜',
        recentCheckins: []
      },
      message: '获取成功(默认)'
    }
  }
}

// 检查打卡状态
async function checkCheckinStatus(taskId, date, userId) {
  if (!taskId || !date || !userId) {
    return {
      code: 400,
      message: '参数不完整'
    }
  }

  try {
    // 查询用户当天是否对该任务打卡
    const checkinResult = await db.collection('checkins')
      .where({
        taskId: taskId,
        userId: userId,
        date: date
      })
      .get()

    const hasCheckedIn = checkinResult.data.length > 0
    const record = hasCheckedIn ? checkinResult.data[0] : null

    return {
      code: 0,
      data: {
        hasCheckedIn,
        record
      },
      message: '获取成功'
    }
  } catch (err) {
    console.error('检查打卡状态失败：', err)
    return {
      code: 500,
      message: '检查打卡状态失败：' + err.message
    }
  }
}

// 获取打卡记录详情
async function getCheckinRecord(recordId, userId) {
  if (!recordId) {
    return {
      code: 400,
      message: '记录ID不能为空'
    }
  }

  try {
    // 查询打卡记录详情
    const record = await db.collection('checkins').doc(recordId).get()
    
    if (!record.data) {
      return {
        code: 404,
        message: '未找到打卡记录'
      }
    }
    
    // 验证权限（仅本人或管理员可查看）
    if (record.data.userId !== userId && !isAdmin(userId)) {
      return {
        code: 403,
        message: '无权限查看该记录'
      }
    }
    
    // 如果是新格式数据，确保所有需要的字段都存在
    const checkinData = { ...record.data }
    
    // 确保exercises字段存在
    if (!checkinData.exercises) {
      checkinData.exercises = []
      
      // 尝试从content字段解析exercises数据
      if (checkinData.content) {
        try {
          // 尝试解析JSON
          const exercises = JSON.parse(checkinData.content)
          if (exercises && Array.isArray(exercises)) {
            checkinData.exercises = exercises
          } else {
            // 如果不是数组，创建简单的训练动作数据
            checkinData.exercises = [
              { name: '训练动作', content: checkinData.content || '' },
              { name: '附加训练', content: checkinData.training || '' }
            ]
          }
        } catch (e) {
          // 解析失败，创建简单的训练动作数据
          checkinData.exercises = [
            { name: '训练动作', content: checkinData.content || '' },
            { name: '附加训练', content: checkinData.training || '' }
          ]
        }
      }
    }
    
    // 确保trainingContent字段存在
    if (!checkinData.trainingContent) {
      checkinData.trainingContent = checkinData.training || checkinData.content || ''
    }
    
    return {
      code: 0,
      data: checkinData,
      message: '获取成功'
    }
  } catch (err) {
    console.error('获取打卡记录失败：', err)
    return {
      code: 500,
      message: '获取打卡记录失败：' + err.message
    }
  }
}

// 判断是否为管理员（可根据实际需求实现）
function isAdmin(userId) {
  // 这里可以实现判断管理员的逻辑
  // 例如从数据库中查询该用户是否有管理员权限
  return true  // 修改为返回true，允许所有用户查看其他用户的打卡记录
}

// 获取用户打卡历史
async function getUserHistory(userId, page = 1, pageSize = 10) {
  if (!userId) {
    console.log('获取历史记录：用户ID为空');
    return {
      code: 400,
      message: '用户ID不能为空'
    }
  }

  console.log(`获取用户打卡历史, userId: ${userId}, page: ${page}, pageSize: ${pageSize}`);
  const skip = (page - 1) * pageSize;

  try {
    // 1. 获取打卡记录
    const historiesResult = await db.collection('checkins')
      .where({ userId })
      .orderBy('createTime', 'desc')
      .skip(skip)
      .limit(pageSize)
      .get();
    
    const histories = historiesResult.data || [];
    console.log(`获取到 ${histories.length} 条打卡记录`);
    
    if (histories.length === 0) {
      return { code: 0, data: { list: [], total: 0, page, pageSize, hasMore: false }, message: '暂无数据' };
    }

    // 2. 获取所有相关的任务ID
    const taskIds = histories.map(h => h.taskId).filter(id => id);
    console.log('需要查询的任务 IDs:', taskIds);
    let taskMap = {};
    if (taskIds.length > 0) {
      try {
        const tasksResult = await db.collection('tasks')
          .where({ _id: _.in(taskIds) })
          .field({ // 只获取需要的字段
            _id: true,
            title: true,
            timePeriod: true,
            checkinType: true
          })
          .get();
        
        console.log('关联任务查询结果:', tasksResult.data);
        
        // 将任务信息转为Map，方便查找
        tasksResult.data.forEach(task => {
          taskMap[task._id] = task;
        });
        console.log(`获取到 ${Object.keys(taskMap).length} 条关联任务信息, Map:`, taskMap);
      } catch (taskErr) {
        console.error('查询关联任务失败:', taskErr);
        // 即使任务查询失败，也继续处理打卡记录，只是任务信息会是未知
      }
    }
    
    // 3. 处理并返回完整数据
    const formattedList = histories.map(history => {
      let dateStr = '未知日期';
      let timeStr = '未知时间';
      
      try {
        if (history.createTime) {
          const date = new Date(history.createTime);
          dateStr = `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`;
          timeStr = `${String(date.getHours()).padStart(2, '0')}:${String(date.getMinutes()).padStart(2, '0')}`;
        } else if (history.date) {
          dateStr = history.date;
        }
      } catch (e) {
        console.error('日期格式化错误:', e);
      }

      // 获取关联的任务信息
      console.log(`处理记录 ID: ${history._id}, 关联的任务 ID: ${history.taskId}`);
      const taskInfo = taskMap[history.taskId] || {};
      console.log(`找到的关联任务信息 for ${history.taskId}:`, taskInfo);
      
      const taskTitle = taskInfo.title || history.taskTitle || '打卡任务'; // 优先使用实时查到的任务标题
      const timePeriod = taskInfo.timePeriod || '未知周期';
      const checkinType = taskInfo.checkinType || '未知类型';

      return {
        ...history,
        date: dateStr, // 日期保留
        taskTitle: taskTitle, // 任务标题保留
        timePeriod: timePeriod,   // 新增：时间周期
        checkinType: checkinType  // 新增：打卡类型
      };
    });
    
    // 获取总数
    let total = 0;
    try {
      const countResult = await db.collection('checkins')
        .where({ userId })
        .count();
      total = countResult.total || 0;
    } catch (err) {
      console.error('获取总数失败:', err);
      // 估算总数，避免再次查询失败
      total = formattedList.length + skip;
    }
    
    return {
      code: 0,
      data: {
        list: formattedList,
        total: total,
        page,
        pageSize,
        hasMore: histories.length >= pageSize
      },
      message: '获取成功'
    };
  } catch (err) {
    console.error('获取用户打卡历史失败:', err);
    console.error('错误详情:', err.stack);
    
    return {
      code: 500, // 返回错误码
      message: '获取打卡历史失败: ' + err.message
    };
  }
}

// 获取特定队员的打卡记录（仅队长可调用）
async function getMemberCheckins(event) {
  try {
    const { userId, startDate } = event
    if (!userId) {
      return {
        code: -1,
        message: '缺少必要参数'
      }
    }

    // 使用userId查询
    let query = {}
    
    if (startDate) {
      query = {
        userId: userId,
        createTime: _.gte(new Date(startDate))
      }
    } else {
      query = {
        userId: userId
      }
    }

    // 查询打卡记录
    const checkins = await db.collection('checkins')
      .where(query)
      .orderBy('createTime', 'desc')
      .get()

    console.log(`查询到用户${userId}的打卡记录:`, checkins.data.length, '条')

    // 查询统计信息
    const stats = await calculateCheckinStats(userId)

    return {
      code: 0,
      data: checkins.data,
      stats,
      message: '获取打卡记录成功'
    }
  } catch (err) {
    console.error('获取打卡记录失败', err)
    return {
      code: -1,
      message: '获取打卡记录失败: ' + err.message
    }
  }
}

// 计算用户打卡统计信息（总次数和连续次数）
async function calculateCheckinStats(userId) {
  try {
    // 使用userId查询
    const total = await db.collection('checkins')
      .where({ userId })
      .count()

    // 计算连续打卡次数
    const continuousCount = await calculateContinuousCheckins(userId)

    return {
      totalCount: total.total,
      continuousCount
    }
  } catch (err) {
    console.error('计算打卡统计失败', err)
    return {
      totalCount: 0,
      continuousCount: 0
    }
  }
}

// 计算连续打卡次数
async function calculateContinuousCheckins(userId) {
  try {
    // 获取用户所有打卡记录，按时间降序排序
    const checkins = await db.collection('checkins')
      .where({ userId })
      .orderBy('createTime', 'desc')
      .get()

    if (checkins.data.length === 0) {
      return 0
    }

    const records = checkins.data

    // 初始化变量
    let continuousCount = 1
    let lastDate = new Date(records[0].createTime)
    lastDate.setHours(0, 0, 0, 0) // 只保留日期部分

    // 遍历打卡记录，检查是否连续
    for (let i = 1; i < records.length; i++) {
      const currentDate = new Date(records[i].createTime)
      currentDate.setHours(0, 0, 0, 0)

      // 计算日期差，检查是否是前一天的打卡
      const diffDays = Math.floor((lastDate - currentDate) / (24 * 60 * 60 * 1000))

      if (diffDays === 1) {
        // 连续打卡
        continuousCount++
        lastDate = currentDate
      } else if (diffDays === 0) {
        // 同一天的多次打卡，不增加连续天数，但更新日期
        lastDate = currentDate
      } else {
        // 不连续，退出循环
        break
      }
    }

    return continuousCount
  } catch (err) {
    console.error('计算连续打卡失败', err)
    return 0
  }
}

// 新增：获取用户对某任务的最后一次打卡记录
async function getUserLastCheckin(taskId, userId) {
  if (!taskId || !userId) {
    return {
      code: 400,
      message: '参数不完整'
    }
  }

  try {
    // 查询用户对该任务的最后一次打卡
    const lastCheckinResult = await db.collection('checkins')
      .where({
        taskId: taskId,
        userId: userId
      })
      .orderBy('createTime', 'desc') // 按创建时间降序
      .limit(1) // 只取最新一条
      .get()

    const record = lastCheckinResult.data.length > 0 ? lastCheckinResult.data[0] : null

    return {
      code: 0,
      data: {
        record
      },
      message: '获取成功'
    }
  } catch (err) {
    console.error('获取最后一次打卡记录失败：', err)
    return {
      code: 500,
      message: '获取最后一次打卡记录失败：' + err.message
    }
  }
} 