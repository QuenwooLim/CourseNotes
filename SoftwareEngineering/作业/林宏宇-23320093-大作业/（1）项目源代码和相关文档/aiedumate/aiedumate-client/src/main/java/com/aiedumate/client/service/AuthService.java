package com.aiedumate.client.service;


import com.aiedumate.client.dto.LoginRequest;
import com.aiedumate.client.dto.LoginResponse;

public interface AuthService {

    public LoginResponse authenticateUser(LoginRequest loginRequest, String ipAddress, String userAgent);

    public void logout(String sessionId);

    public LoginResponse validateToken(String sessionId);
}
