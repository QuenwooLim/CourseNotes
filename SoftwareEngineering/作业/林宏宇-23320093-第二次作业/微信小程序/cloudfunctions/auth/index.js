const cloud = require('wx-server-sdk')
cloud.init({
  env: 'cloud1-8g5yt7n443fd27f9'
})

const db = cloud.database()
const _ = db.command

// 获取下一个可用的ID
async function getNextId() {
  try {
    // 查询用户集合中的所有文档并按ID排序
    const result = await db.collection('users')
      .orderBy('_id', 'desc')
      .limit(1)
      .get();
    
    // 如果集合中没有文档，返回1作为第一个ID
    if (result.data.length === 0) {
      return 1;
    }
    
    // 尝试将最大的ID转换为数字并加1
    const lastId = result.data[0]._id;
    // 如果ID是数字字符串，解析为数字并加1
    if (!isNaN(lastId)) {
      return parseInt(lastId) + 1;
    } else {
      // 如果之前的ID不是数字格式，从1开始
      return 1;
    }
  } catch (err) {
    console.error('获取下一个ID失败:', err);
    // 出错时默认返回一个较大的数字，避免ID冲突
    return 10000;
  }
}

// 用户登录
exports.login = async (event, context) => {
  const { userInfo } = event
  try {
    // 查询用户是否存在 - 使用微信用户信息匹配
    const user = await db.collection('users').where({
      nickName: userInfo.nickName,
      avatarUrl: userInfo.avatarUrl
    }).get()
    
    if (user.data.length === 0) {
      // 获取下一个可用的ID
      const nextId = await getNextId();
      
      // 新用户，创建记录
      const newUser = {
        _id: nextId.toString(),  // 使用自增的数字ID，转为字符串
        nickName: userInfo.nickName,
        avatarUrl: userInfo.avatarUrl,
        gender: userInfo.gender,
        studentId: '',
        totalCheckins: 0,
        monthlyCheckins: 0,
        createdAt: db.serverDate(),
        userId: nextId  // 额外保存一个数字格式的ID，便于排序和查询
      }
      
      // 使用add方法创建用户
      await db.collection('users').add({
        data: newUser
      })
      
      return {
        code: 0,
        message: 'success',
        data: {
          userInfo: newUser
        }
      }
    } else {
      // 更新用户信息
      const existingUser = user.data[0];
      await db.collection('users').doc(existingUser._id).update({
        data: {
          nickName: userInfo.nickName,
          avatarUrl: userInfo.avatarUrl,
          gender: userInfo.gender
        }
      })
      
      return {
        code: 0,
        message: 'success',
        data: {
          userInfo: existingUser
        }
      }
    }
  } catch (err) {
    return {
      code: -1,
      message: err.message
    }
  }
}

// 获取用户信息
exports.getUserInfo = async (event, context) => {
  const { userId } = event
  try {
    const user = await db.collection('users').doc(userId).get()
    
    if (!user.data) {
      return {
        code: 1004,
        message: '用户不存在'
      }
    }
    
    return {
      code: 0,
      message: 'success',
      data: user.data
    }
  } catch (err) {
    return {
      code: -1,
      message: err.message
    }
  }
} 