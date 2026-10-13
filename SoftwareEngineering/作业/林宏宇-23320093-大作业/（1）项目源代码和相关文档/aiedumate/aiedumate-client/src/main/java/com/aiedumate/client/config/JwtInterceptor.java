package com.aiedumate.client.config;

import com.aiedumate.client.util.JwtTokenUtil;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.HttpMethod;
import org.springframework.web.servlet.HandlerInterceptor;

@Slf4j
public class JwtInterceptor implements HandlerInterceptor {

    private final JwtTokenUtil jwtTokenUtil;

    public JwtInterceptor(JwtTokenUtil jwtTokenUtil) {
        this.jwtTokenUtil = jwtTokenUtil;
    }

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        // 如果是OPTIONS请求，直接放行
        if (HttpMethod.OPTIONS.toString().equals(request.getMethod())) {
            return true;
        }
        String requestUri = request.getRequestURI();

        String token = request.getHeader("Authorization");

        if (token == null || !token.startsWith("Bearer ")) {
            log.info("Token不存在，拦截器处理路径: {}", requestUri);
            //response.sendError(HttpServletResponse.SC_UNAUTHORIZED, "未提供有效的token");
            return false;
        }

        token = token.substring(7); // 去掉"Bearer "
        String username = jwtTokenUtil.getUsernameFromToken(token);

        if (username == null || !jwtTokenUtil.validateToken(token, username)) {
            log.info("Token不过期，拦截器处理路径: {}", requestUri);
            //response.sendError(HttpServletResponse.SC_UNAUTHORIZED, "无效的token");
            return false;
        }

        // 验证通过，将用户名存入request属性中
        request.setAttribute("username", username);
        return true;
    }
}
