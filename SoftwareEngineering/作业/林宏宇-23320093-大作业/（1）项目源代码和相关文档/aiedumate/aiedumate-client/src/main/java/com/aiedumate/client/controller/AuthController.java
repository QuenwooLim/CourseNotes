package com.aiedumate.client.controller;

import com.aiedumate.client.dto.LoginRequest;
import com.aiedumate.client.dto.ApiResponse;
import com.aiedumate.client.dto.LoginResponse;
import com.aiedumate.client.dto.Message;
import com.aiedumate.client.service.AuthService;
import jakarta.servlet.http.HttpServletRequest;
import lombok.extern.slf4j.Slf4j;
import org.springframework.ai.chat.memory.ChatMemory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

@RestController
@Slf4j
public class AuthController {
    @Autowired
    private AuthService authService;
    @Autowired
    private ChatMemory chatMemory;

    //登录验证API
    @PostMapping("/api/login")
    public ResponseEntity<?> authenticateUser(@RequestBody LoginRequest loginRequest, HttpServletRequest request) {
        try {
            log.info("username:{},password:{}", loginRequest.getUsername(), loginRequest.getPassword());

            String ipAddress = request.getRemoteAddr();
            String userAgent = request.getHeader("User-Agent");

            LoginResponse loginResponse = authService.authenticateUser(loginRequest, ipAddress, userAgent);

            return ResponseEntity.ok(Map.of(
                    "code", 200,
                    "message", "登录成功",
                    "data", Map.of(
                            "token", loginResponse.getToken(),
                            "username", loginResponse.getUsername(),
                            "email", loginResponse.getEmail()
                    )
            ));
//            return ResponseEntity.ok(loginResponse);
        } catch (RuntimeException e) {
            log.error(e.getMessage(),e);
//            Map<String, Object> errorResponse = new HashMap<>();
//            errorResponse.put("code", 500);
//            errorResponse.put("message",e.getMessage());
//            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(errorResponse);
            return ResponseEntity.badRequest().body(new ApiResponse(false, "处理异常，请查看后台日志"));
        }
    }

    //登出验证API
    @PostMapping("/api/logout")
    public ResponseEntity<?> logoutUser(@RequestHeader("Authorization") String authHeader) {
        try {
            String token = authHeader.substring(7); // 去掉 "Bearer " 前缀
            authService.logout(token);
            return ResponseEntity.ok(new ApiResponse(true, "登出成功"));
        } catch (RuntimeException e) {
            log.error(e.getMessage(),e);
            return ResponseEntity.badRequest().body(new ApiResponse(false, "处理异常，请查看后台日志"));
        }
    }

    //token验证API
    @GetMapping("/api/validatetoken")
    public ResponseEntity<?> validateToken(@RequestHeader("Authorization") String authHeader) {
        try {
            String token = authHeader.substring(7); // 去掉 "Bearer " 前缀
            LoginResponse loginResponse = authService.validateToken(token);
            return ResponseEntity.ok(loginResponse);
        } catch (RuntimeException e) {
            log.error(e.getMessage(),e);
            return ResponseEntity.badRequest().body(new ApiResponse(false, "处理异常，请查看后台日志"));
        }
    }

}