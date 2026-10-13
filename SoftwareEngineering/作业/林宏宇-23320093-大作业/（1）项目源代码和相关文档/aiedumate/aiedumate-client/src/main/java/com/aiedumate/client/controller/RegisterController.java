package com.aiedumate.client.controller;

import com.aiedumate.client.dto.RegisterRequest;
import com.aiedumate.client.dto.ResponseResult;
import com.aiedumate.client.service.RegisterService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@Slf4j
public class RegisterController {
    @Autowired
    private RegisterService registerService;

    @PostMapping("/api/register")
    public ResponseResult<Void> register(@RequestBody RegisterRequest registerRequest) {
        try {
            registerService.registerUser(registerRequest);
            return ResponseResult.success();
        } catch (Exception e) {
            log.error(e.getMessage(),e);
            return ResponseResult.fail("处理异常，请查看后台日志");
        }
    }
}