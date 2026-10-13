package com.aiedumate.client.service.impl;

//import org.springframework.security.crypto.password.PasswordEncoder;
import com.aiedumate.client.dto.RegisterRequest;
import com.aiedumate.client.service.RegisterService;
import com.aiedumate.client.dao.UsersMapper;
import com.aiedumate.client.entity.TblUsers;
import com.aiedumate.client.util.SaltUtil;
import jakarta.annotation.Resource;
import org.apache.commons.codec.digest.DigestUtils;
import org.springframework.stereotype.Service;

import java.util.Date;

@Service
public class RegisterServiceImpl implements RegisterService {
    @Resource
    UsersMapper usersMapper;

    @Override
    public void registerUser(RegisterRequest registerRequest) {
        TblUsers user = new TblUsers();
        user.setUsername(registerRequest.getUsername());
        user.setEmail(registerRequest.getEmail());
        user.setPhone(registerRequest.getPhone());
        // 生成盐值
        String salt = SaltUtil.generateSalt();
        user.setSalt(salt);
        // 加密密码
        String encryptedPassword = DigestUtils.md5Hex(registerRequest.getUsername() + registerRequest.getPassword() + user.getSalt());
        user.setPassword(encryptedPassword);
        user.setIsActive(true);
        user.setCreatedTime(new Date());
        user.setUpdatedTime(user.getCreatedTime());
        usersMapper.insert(user);
    }
}