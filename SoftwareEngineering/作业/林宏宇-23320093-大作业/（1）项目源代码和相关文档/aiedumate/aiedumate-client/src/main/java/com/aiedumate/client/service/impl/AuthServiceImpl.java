package com.aiedumate.client.service.impl;

import com.aiedumate.client.dto.LoginRequest;
import com.aiedumate.client.dto.LoginResponse;
import com.aiedumate.client.entity.TblUserSessions;
import com.aiedumate.client.entity.TblUsers;
import com.aiedumate.client.service.AuthService;
import com.aiedumate.client.util.JwtTokenUtil;
import com.aiedumate.client.dao.UserSessionsMapper;
import com.aiedumate.client.dao.UsersMapper;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.codec.digest.DigestUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.util.Date;

@Service
@Slf4j
public class AuthServiceImpl implements AuthService {

    @Autowired
    private JwtTokenUtil jwtTokenUtil;
    @Resource
    UsersMapper usersMapper;
    @Resource
    UserSessionsMapper userSessionsMapper;
    @Value("${jwt.expiration}")
    private Long expiration;
    @Override
    public LoginResponse authenticateUser(LoginRequest loginRequest, String ipAddress, String userAgent) {
        TblUsers user = usersMapper.selectByPrimaryKey(loginRequest.getUsername());
        if(user == null){
            throw new RuntimeException("用户不存在");
        }
        if (!user.getIsActive()) {
            throw new RuntimeException("用户已被禁用");
        }

        // 加密密码
        String encryptedPassword = DigestUtils.md5Hex(loginRequest.getUsername() + loginRequest.getPassword() + user.getSalt());

        log.info("username:{},password:{},salt:{},encryptpwd:{}", loginRequest.getUsername(),loginRequest.getPassword(), user.getSalt(), encryptedPassword);

        if(!user.getPassword().equals(encryptedPassword)){
            throw new RuntimeException("密码错误");
        }

        // 更新最后登录时间
        user.setLastLoginTime(new Date());
        usersMapper.updateByPrimaryKey(user);

        // 使之前的会话失效
        userSessionsMapper.invalidateAllSessions(user.getUsername());

        // 创建新会话
        String token = jwtTokenUtil.generateToken(user.getUsername());
        Date expiresTime = new Date((new Date()).getTime() + expiration * 1000);

        TblUserSessions session = new TblUserSessions();
        session.setSessionId(token);
        session.setUsername(user.getUsername());
        session.setCreatedTime(new Date());
        session.setExpiresTime(expiresTime);
        session.setIsExpired(false);
        session.setIpAddress(ipAddress);
        session.setUserAgent(userAgent);
        userSessionsMapper.insert(session);

        return new LoginResponse(token, token, user.getUsername(), user.getEmail(), expiresTime);
    }

    @Override
    public void logout(String sessionId) {
        TblUserSessions session = userSessionsMapper.selectByPrimaryKey(sessionId);
        if(session == null){
            throw new RuntimeException("无效的会话");
        }
        session.setIsExpired(true);
        userSessionsMapper.updateByPrimaryKey(session);
    }

    @Override
    public LoginResponse validateToken(String token) {
        if (!jwtTokenUtil.validateToken(token)) {
            throw new RuntimeException("无效的令牌");
        }

        String loginName = jwtTokenUtil.getUsernameFromToken(token);

//        TblUserSessions session = userSessionsMapper.selectByPrimaryKey(sessionId);
//        if(session == null){
//            throw new RuntimeException("无效的会话");
//        }
//        if(session.getExpiresTime().compareTo(new Date()) <= 0){
//            throw new RuntimeException("会话已过期");
//        }

        TblUsers user = usersMapper.selectByPrimaryKey(loginName);
        if(!user.getIsActive()){
            throw new RuntimeException("用户已注销");
        }
        if(user == null){
            throw new RuntimeException("用户不存在");
        }
        return new LoginResponse(token,token, user.getUsername(), user.getEmail(), null);
    }
}
