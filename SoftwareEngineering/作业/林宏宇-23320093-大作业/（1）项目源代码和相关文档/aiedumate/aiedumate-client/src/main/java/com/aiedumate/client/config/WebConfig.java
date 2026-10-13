package com.aiedumate.client.config;

import com.aiedumate.client.util.JwtTokenUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

@Configuration
public class WebConfig implements WebMvcConfigurer {
    @Autowired
    private JwtTokenUtil jwtTokenUtil;
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(new JwtInterceptor(jwtTokenUtil))
                .addPathPatterns("/**") // 拦截所有路径
                .excludePathPatterns(
                        "/",
                        "/video",
                        "/login",
                        "/register",
                        "/chat",
                        "/schedule",
                        "/appointment",
                        "/note",
                        "/api/login",
                        "/api/register",
                        "/api/logout",
                        "/api/validatetoken",
                        "/css/**",
                        "/js/**",
                        "/favicon.ico",
                        "/images/**"); // 排除静态资源和登录注册接口
    }
}