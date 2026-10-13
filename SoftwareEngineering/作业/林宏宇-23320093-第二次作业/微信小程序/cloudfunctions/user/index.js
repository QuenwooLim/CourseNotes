const cloud = require('wx-server-sdk')

cloud.init({
  env: cloud.DYNAMIC_CURRENT_ENV
})

const db = cloud.database()
const _ = db.command

// 获取下一个可用的用户ID
async function getNextId() {
  try {
    console.log('开始获取下一个用户ID');
    
    // 查询所有用户记录
    const usersRes = await db.collection('users').get();
    const users = usersRes.data || [];
    
    // 没有用户时返回初始ID "1"
    if (users.length === 0) {
      console.log('用户集合为空，返回初始ID: "1"');
      return "1";
    }
    
    // 1. 将所有用户ID转为整数类型
    let maxId = 0;
    users.forEach(user => {
      try {
        const userId = parseInt(user._id, 10);
        // 只处理有效的数字ID
        if (!isNaN(userId) && userId > maxId) {
          maxId = userId;
        }
      } catch (err) {
        console.warn('无法解析用户ID:', user._id, err);
      }
    });
    
    // 2. 找到最大值
    console.log('当前最大用户ID:', maxId);
    
    // 3. 最大值+1并转为字符串
    const nextId = maxId + 1;
    const nextIdStr = String(nextId);
    
    console.log('生成的下一个用户ID:', nextIdStr);
    return nextIdStr;
    
  } catch (err) {
    console.error('获取下一个用户ID时出错:', err);
    // 出错时返回一个较大的ID以避免冲突
    return String(Math.floor(100000 + Math.random() * 900000));
  }
}

// 云函数入口函数
exports.main = async (event, context) => {
  // 添加请求标识，便于日志跟踪
  const requestId = Date.now().toString(36) + Math.random().toString(36).substr(2);
  console.log(`[${requestId}] 用户云函数调用:`, event.type, event);
  
  try {
    let result;
    switch (event.type) {
      case 'create':
        result = await createUser(event.data);
        break;
      case 'get':
        result = await getUser(event.userId);
        break;
      case 'register':
        console.log(`[${requestId}] 开始用户注册流程`);
        result = await registerUser(event);
        console.log(`[${requestId}] 用户注册完成:`, result.code === 0 ? '成功' : '失败', result.message);
        break;
      case 'login':
        result = await loginUser(event);
        break;
      case 'update':
        result = await updateUserInfo(event);
        break;
      case 'verifyCaptain':
        result = await verifyCaptain(event);
        break;
      case 'getUserById':
        result = await getUserById(event);
        break;
      case 'checkCaptainStatus':
        result = await checkCaptainStatus(event);
        break;
      default:
        result = {
          code: 404,
          message: '未知操作类型'
        };
    }
    return result;
  } catch (e) {
    console.error(`[${requestId}] 云函数执行错误:`, e);
    // 记录更详细的错误信息
    const errorDetail = {
      message: e.message,
      stack: e.stack,
      name: e.name,
      code: e.code,
      event: event
    };
    console.error(`[${requestId}] 错误详情:`, JSON.stringify(errorDetail));
    
    return {
      code: 500,
      message: '云函数执行出错: ' + e.message,
      requestId
    };
  }
}

// 验证队长密码并更新状态
async function verifyCaptain(event) {
  try {
    const { userId, password } = event
    if (!userId || !password) {
      return {
        code: 400,
        message: '参数不完整'
      }
    }

    // 验证密码
    if (password !== 'SYSU') {
      return {
        code: 401,
        message: '密码错误'
      }
    }

    // 更新用户的队长状态
    await db.collection('users').doc(userId).update({
      data: {
        isCaptain: true,
        updateTime: db.serverDate()
      }
    })

    return {
      code: 0,
      message: '验证成功'
    }
  } catch (err) {
    console.error('验证队长失败：', err)
    return {
      code: 500,
      message: '验证失败：' + err.message
    }
  }
}

// 创建或更新用户
async function createUser(userData) {
  try {
    console.log('创建/更新用户:', userData)
    
    if (!userData._id) {
      console.error('用户ID为空');
      return {
        code: 400,
        message: '用户ID不能为空'
      }
    }
    
    // 检查用户是否已存在
    let existingUser;
    try {
      existingUser = await db.collection('users').doc(userData._id).get()
    } catch (err) {
      console.error('查询用户失败:', err);
      // 继续执行，假设用户不存在
      existingUser = { data: null };
    }

    // 准备用户数据，确保包含所有需要的字段
    const user = {
      // 基本信息
      nickName: userData.nickName || '未命名用户',
      avatarUrl: userData.avatarUrl || '',
      studentId: userData.studentId || '',
      gender: userData.gender || 'male',
      college: userData.college || '',  // 学院
      grade: userData.grade || '',      // 年级
      password: userData.password || '',
      
      // 队员信息
      teamStatus: userData.teamStatus || '在训',  // 状态：在训/退役/退队
      paddleSide: userData.paddleSide || '',      // 左右桨
      competitions: userData.competitions || [],   // 参赛记录
      
      // 个人详细信息
      birthday: userData.birthday || '',
      joinDate: userData.joinDate || '',  // 入队时间
      weight: userData.weight || '',      // 体重(kg)
      height: userData.height || '',      // 身高(cm)
      testLevel: userData.testLevel || '', // 测试水平
      phone: userData.phone || '',         // 手机号
      
      // 系统信息
      isCaptain: userData.isCaptain || false,
      updateTime: db.serverDate()
    }
    
    // 保留原有数据中可能存在的其他字段
    if (userData.city) user.city = userData.city
    if (userData.province) user.province = userData.province
    if (userData.country) user.country = userData.country
    if (userData.userId) user.userId = userData.userId

    // 如果用户不存在，创建新用户
    if (!existingUser.data) {
      // 获取下一个可用ID - 已是字符串
      const nextIdStr = await getNextId();
      console.log('创建新用户，分配ID:', nextIdStr);
      
      // 设置ID字段
      user._id = nextIdStr;
      user.userId = parseInt(nextIdStr, 10); // 转为数字类型
      user.createTime = db.serverDate();
      
      // 创建用户
      await db.collection('users').add({
        data: user
      })
    } else {
      // 如果用户存在，更新信息
      await db.collection('users').doc(userData._id).update({
        data: user
      })
    }

    return {
      code: 0,
      data: user,
      message: '操作成功'
    }
  } catch (err) {
    console.error('创建/更新用户失败：', err)
    return {
      code: 500,
      message: '操作失败：' + err.message
    }
  }
}

// 获取用户信息
async function getUser(userId) {
  try {
    console.log('获取用户信息, userId:', userId)
    
    if (!userId) {
      return {
        code: 404,
        message: '用户ID不能为空'
      }
    }
    
    // 根据ID直接获取
    try {
      const user = await db.collection('users').doc(userId).get()
      if (user && user.data) {
        return {
          code: 0,
          data: user.data,
          message: '获取用户信息成功'
        }
      }
    } catch (err) {
      console.log('查询用户失败', err)
      
      // 尝试使用userId字段查询
      try {
        const userQuery = await db.collection('users').where({
          userId: parseInt(userId)
        }).get()
        
        if (userQuery.data.length > 0) {
          return {
            code: 0,
            data: userQuery.data[0],
            message: '获取用户信息成功'
          }
        }
      } catch (innerErr) {
        console.log('使用userId字段查询用户失败', innerErr)
      }
      
      return {
        code: 404,
        message: '用户不存在'
      }
    }

    return {
      code: 404,
      message: '用户不存在'
    }
  } catch (err) {
    console.error('获取用户信息失败：', err)
    return {
      code: 500,
      message: '获取用户信息失败：' + err.message
    }
  }
}

// 用户信息更新
async function updateUserInfo(event) {
  try {
    console.log('更新用户信息:', event.data, '队长编辑模式:', event.isCaptainEdit)
    
    // 获取更新数据
    const userData = event.data
    const isCaptainEdit = event.isCaptainEdit || false
    
    // 验证关键字段存在
    if (!userData._id) {
      return {
        code: 400,
        message: '用户ID不能为空'
      }
    }
    
    // 检查用户是否存在 - 使用用户提供的_id进行查询
    let userId = userData._id
    let existingUser
    
    try {
      // 使用_id直接查询
      existingUser = await db.collection('users').doc(userId).get()
    } catch (err) {
      console.error('使用_id查询用户失败:', err)
      return {
        code: 404,
        message: '用户不存在'
      }
    }
    
    if (!existingUser || !existingUser.data) {
      return {
        code: 404,
        message: '用户信息不存在'
      }
    }

    // 如果是队长编辑模式，需要验证调用者是否为队长
    if (isCaptainEdit) {
      // 获取当前登录用户的ID（从event中获取）
      const callerId = event.callerId
      
      if (!callerId) {
        return {
          code: 403,
          message: '无法验证队长身份，请重新登录'
        }
      }
      
      console.log('验证队长权限，callerId:', callerId)
      
      // 获取调用者信息
      let callerUser
      try {
        // 先尝试使用_id直接查询
        callerUser = await db.collection('users').doc(callerId).get()
      } catch (err) {
        console.error('查询用户失败:', err)
        return {
          code: 403,
          message: '无法验证队长身份，用户不存在'
        }
      }
      
      // 验证调用者是否为队长
      if (!callerUser.data || !callerUser.data.isCaptain) {
        return {
          code: 403,
          message: '无权进行此操作，仅队长可编辑其他队员信息'
        }
      }
      
      console.log('队长编辑模式验证通过')
    }
    
    // 准备更新数据
    // 常规模式下只允许更新有限的字段
    const updateData = isCaptainEdit ? {
      // 队长模式下允许更新所有字段
      avatarUrl: userData.avatarUrl,
      nickName: userData.nickName,
      studentId: userData.studentId,
      gender: userData.gender,
      college: userData.college,
      grade: userData.grade,
      teamStatus: userData.teamStatus,
      testLevel: userData.testLevel,
      phone: userData.phone,
      birthday: userData.birthday,
      height: userData.height,
      weight: userData.weight,
      paddleSide: userData.paddleSide,
      joinDate: userData.joinDate,
      updateTime: db.serverDate()
    } : {
      // 常规模式下的有限字段
      avatarUrl: userData.avatarUrl,
      phone: userData.phone,
      birthday: userData.birthday,
      height: userData.height,
      weight: userData.weight,
      paddleSide: userData.paddleSide,
      joinDate: userData.joinDate,
      updateTime: db.serverDate()
    }
    
    // 如果有提供密码，增加密码更新
    if (userData.password) {
      updateData.password = userData.password
    }
    
    // 执行更新操作 - 使用确认的用户ID
    await db.collection('users').doc(userId).update({
      data: updateData
    })
    
    // 获取更新后的完整用户数据
    const updatedUser = await db.collection('users').doc(userId).get()
    
    // 返回完整的用户信息
    return {
      code: 0,
      data: updatedUser.data,
      message: '更新成功'
    }
  } catch (err) {
    console.error('更新用户信息失败:', err)
    return {
      code: 500,
      message: '操作失败: ' + err.message
    }
  }
}

// 根据用户ID获取用户信息
async function getUserById(event) {
  try {
    const { userId } = event
    if (!userId) {
      return {
        code: -1,
        message: '缺少必要参数'
      }
    }

    // 查询指定用户信息
    try {
      const user = await db.collection('users').doc(userId).get()
      
      if (!user.data) {
        return {
          code: -1,
          message: '用户不存在'
        }
      }
      
      return {
        code: 0,
        data: user.data,
        message: '获取用户信息成功'
      }
    } catch (err) {
      return {
        code: -1,
        message: '用户不存在'
      }
    }
  } catch (err) {
    console.error('获取用户信息失败', err)
    return {
      code: -1,
      message: '获取用户信息失败: ' + err.message
    }
  }
}

// 注册新用户
async function registerUser(event) {
  try {
    const { 
      studentId, 
      password, 
      nickName,
      gender,
      college,
      grade,
      teamStatus
    } = event.data
    
    if (!studentId || !password) {
      return {
        code: 400,
        message: '学号和密码不能为空'
      }
    }
    
    // 检查学号是否已存在
    const existingUsers = await db.collection('users').where({
      studentId
    }).get()
    
    if (existingUsers.data.length > 0) {
      return {
        code: 400,
        message: '该学号已被注册'
      }
    }
    
    // 获取下一个可用ID - 已经是字符串类型
    console.log('正在获取下一个用户ID');
    const nextIdStr = await getNextId();
    console.log('获取到的下一个用户ID:', nextIdStr);
    
    // 将ID转换为数字格式用于userId字段
    const nextIdNum = parseInt(nextIdStr, 10);
    
    // 创建新用户对象
    const userData = {
      _id: nextIdStr,
      nickName: nickName || '用户' + studentId,
      studentId,
      password,
      avatarUrl: '',
      gender: gender || 'male',
      college: college || '',
      grade: grade || '',
      teamStatus: teamStatus || '在训',
      paddleSide: '',
      competitions: [],
      birthday: '',
      joinDate: '',
      weight: '',
      height: '',
      testLevel: '',
      phone: '',
      isCaptain: false,
      userId: nextIdNum, // 数值类型的用户ID
      createTime: db.serverDate(),
      updateTime: db.serverDate()
    }
    
    console.log('准备创建用户:', JSON.stringify({
      _id: userData._id,
      studentId: userData.studentId,
      userId: userData.userId
    }));
    
    // 创建用户
    const result = await db.collection('users').add({
      data: userData
    });
    
    console.log('用户创建成功:', result);
    
    return {
      code: 0,
      data: {
        ...userData,
        _id: result._id
      },
      message: '注册成功'
    }
  } catch (err) {
    console.error('用户注册失败:', err);
    const errorMsg = err.message || err.errMsg || '未知错误';
    return {
      code: 500,
      message: '注册失败：' + errorMsg
    }
  }
}

// 用户登录
async function loginUser(event) {
  try {
    const { studentId, password } = event.data
    
    if (!studentId || !password) {
      return {
        code: 400,
        message: '学号和密码不能为空'
      }
    }
    
    // 查询用户
    const users = await db.collection('users').where({
      studentId,
      password
    }).get()
    
    if (users.data.length === 0) {
      return {
        code: 401,
        message: '学号或密码错误'
      }
    }
    
    const user = users.data[0]
    
    // 移除密码字段再返回
    const { password: _, ...userWithoutPassword } = user
    
    return {
      code: 0,
      data: userWithoutPassword,
      message: '登录成功'
    }
  } catch (err) {
    console.error('用户登录失败：', err)
    return {
      code: 500,
      message: '登录失败：' + err.message
    }
  }
}

// 检查用户的队长状态
async function checkCaptainStatus(event) {
  try {
    const { userId } = event
    
    if (!userId) {
      return {
        code: 400,
        message: '用户ID不能为空'
      }
    }
    
    // 查询用户
    let user
    try {
      user = await db.collection('users').doc(userId).get()
    } catch (err) {
      console.error('查询用户失败:', err)
      return {
        code: 404,
        message: '用户不存在'
      }
    }
    
    if (!user || !user.data) {
      return {
        code: 404,
        message: '用户不存在'
      }
    }
    
    // 返回用户的队长状态
    return {
      code: 0,
      data: {
        userId: userId,
        isCaptain: user.data.isCaptain || false
      },
      message: '获取队长状态成功'
    }
  } catch (err) {
    console.error('检查队长状态失败:', err)
    return {
      code: 500,
      message: '检查队长状态失败: ' + err.message
    }
  }
} 