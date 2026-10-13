// 云函数入口文件
const cloud = require('wx-server-sdk')

cloud.init({ env: cloud.DYNAMIC_CURRENT_ENV })
const db = cloud.database()
const _ = db.command

// 云函数入口函数
exports.main = async (event, context) => {
  console.log('Cloud function team triggered:', event.action, 'by userId:', event.userId)

  // 处理不同的操作请求
  switch (event.action) {
    case 'createTeam':
      return await createTeam(event)
    case 'joinTeam':
      return await joinTeam(event)
    case 'getTeamInfo':
      return await getTeamInfo(event)
    case 'getTeamMembers':
      return await getTeamMembers(event.userId)
    default:
      return {
        code: -1,
        message: '未知操作'
      }
  }
}

// 创建团队
async function createTeam(event) {
  // ... 现有代码保持不变 ...
  const { userId } = event;
  // 使用userId代替openid
}

// 加入团队
async function joinTeam(event) {
  // ... 现有代码保持不变 ...
  const { userId } = event;
  // 使用userId代替openid
}

// 获取团队信息
async function getTeamInfo(event) {
  // ... 现有代码保持不变 ...
  const { userId } = event;
  // 使用userId代替openid
}

// 获取团队所有成员（所有人都可以调用）
async function getTeamMembers(userId) {
  try {
    console.log('getTeamMembers called with userId:', userId)
    
    // 直接查询所有用户
    const members = await db.collection('users')
      .get()
    
    console.log('All users count:', members.data.length)

    // 返回用户信息
    return {
      code: 0,
      data: members.data,
      message: '获取用户列表成功'
    }
  } catch (err) {
    console.error('获取用户列表失败', err)
    return {
      code: -1,
      message: '获取用户列表失败: ' + err.message
    }
  }
} 