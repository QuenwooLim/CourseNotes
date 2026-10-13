package com.aiedumate.client.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.messaging.converter.MappingJackson2MessageConverter;
import org.springframework.messaging.converter.MessageConverter;
import org.springframework.messaging.simp.config.MessageBrokerRegistry;
import org.springframework.web.socket.config.annotation.EnableWebSocketMessageBroker;
import org.springframework.web.socket.config.annotation.StompEndpointRegistry;
import org.springframework.web.socket.config.annotation.WebSocketMessageBrokerConfigurer;

import java.util.List;

@Configuration
@EnableWebSocketMessageBroker
public class WebSocketConfig implements WebSocketMessageBrokerConfigurer {

    @Override
    public void registerStompEndpoints(StompEndpointRegistry registry) {
        // 注册 WebSocket 端点，前端将连接到此端点
        registry.addEndpoint("/ws-chat")
                .setAllowedOriginPatterns("*") // 允许所有来源（生产环境应限制）
                .withSockJS(); // 支持 SockJS
    }

    @Override
    public void configureMessageBroker(MessageBrokerRegistry registry) {
        // 设置应用程序目标前缀（前端发送消息的地址前缀）
        registry.setApplicationDestinationPrefixes("/app");

        // 设置代理目的地前缀（前端订阅的地址前缀）
        registry.enableSimpleBroker("/topic");

        // 确保消息顺序
        registry.setPreservePublishOrder(true);
    }

    @Override
    public boolean configureMessageConverters(List<MessageConverter> messageConverters) {
        // 添加 JSON 消息转换器
        messageConverters.add(new MappingJackson2MessageConverter());
        return true;
    }
}

