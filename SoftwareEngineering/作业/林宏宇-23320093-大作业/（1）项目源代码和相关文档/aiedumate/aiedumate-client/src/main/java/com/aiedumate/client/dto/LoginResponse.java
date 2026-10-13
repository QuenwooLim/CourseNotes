package com.aiedumate.client.dto;

import lombok.Data;

import java.time.LocalDateTime;
import java.util.Date;

@Data
public class LoginResponse {
    private String token;
    private String sessionId;
    private String username;
    private String email;
    private Date expiresTime;

    public LoginResponse(String token,String sessionId, String username, String email, Date expiresTime) {
        this.token = token;
        this.sessionId = sessionId;
        this.username = username;
        this.email = email;
        this.expiresTime = expiresTime;
    }
}
