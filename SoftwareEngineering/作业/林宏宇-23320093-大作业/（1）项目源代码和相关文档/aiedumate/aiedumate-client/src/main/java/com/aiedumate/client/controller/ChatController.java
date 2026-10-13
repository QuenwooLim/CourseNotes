package com.aiedumate.client.controller;

import com.aiedumate.client.dto.ApiResponse;
import com.aiedumate.client.dto.Message;
import com.aiedumate.client.util.JwtTokenUtil;
import jakarta.servlet.http.HttpServletRequest;
import lombok.extern.slf4j.Slf4j;
import org.springframework.ai.chat.client.ChatClient;
import org.springframework.ai.chat.memory.ChatMemory;
import org.springframework.ai.chat.messages.UserMessage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@Slf4j
public class ChatController {

    @Autowired
    private ChatClient chatClient;
    @Autowired
    private ChatMemory chatMemory;
    @Autowired
    private SimpMessagingTemplate messagingTemplate;
    @Autowired
    private JwtTokenUtil jwtTokenUtil;

    private String getUserName(Message message){
        String token = message.getAuthorization();
        if (token == null || !token.startsWith("Bearer ")) {
            return null;
        }
        token = token.substring(7); // 去掉"Bearer "
        String username = jwtTokenUtil.getUsernameFromToken(token);
        if (username == null || !jwtTokenUtil.validateToken(token, username)) {
            return null;
        }
        return username;
    }

    @MessageMapping("/send.message")
    public void handleChatMessage(Message message) {
        log.info("Received conversationId:{}, content: {}", message.getConversationId(), message.getContent());

        try {

            String botResponse = null;
            String username = getUserName(message);
            if(username == null) {
                botResponse = "未登录，请先登录在使用";
            } else {//已经登录
                List<org.springframework.ai.chat.messages.Message> memoryMessageList = chatMemory.get(message.getConversationId());
                if(memoryMessageList == null || memoryMessageList.isEmpty()){//新会话，在会话中绑定用户
                    UserMessage userMessage = new UserMessage("username is " + username);
                    chatMemory.add(message.getConversationId(), userMessage);
                }
                botResponse = chatClient.prompt()
                        .user(message.getContent())
                        .advisors(a -> a.param(ChatMemory.CONVERSATION_ID, message.getConversationId()))
                        .call()
                        .content();
            }

            log.info("conversationId:{}, memory:{}", message.getConversationId(), chatMemory.get(message.getConversationId()));

            log.info("Generated response: {}", botResponse);
            messagingTemplate.convertAndSend(
                    "/topic/private.messages." + message.getConversationId(),
                    new Message(message.getConversationId(),botResponse, false));
        } catch (Exception e) {
            log.error("Error processing message", e);
            messagingTemplate.convertAndSend(
                    "/topic/private.messages." + message.getConversationId(),
                    new Message(message.getConversationId(),"处理请求时出错: " + e.getMessage(), false));
        }
    }


    @PostMapping("/api/resetconversation")
    public ResponseEntity<?> resetConversation(@RequestBody Message message, HttpServletRequest request) {
        try {
            String username = (String)request.getAttribute("username");

            //在会话中绑定用户
            //UserMessage userMessage = new UserMessage("username is " + username);
            //chatMemory.add(message.getConversationId(), userMessage);

            List<org.springframework.ai.chat.messages.Message> memoryMessageList = chatMemory.get(message.getOldConversationId());

            String botResponse = null;
            StringBuilder messageHis = new StringBuilder();
            for (org.springframework.ai.chat.messages.Message memoryMessage : memoryMessageList) {
                messageHis.append("MessageType:")
                        .append(memoryMessage.getMessageType())
                        .append(",text:")
                        .append(memoryMessage.getText())
                        .append("\n\r");
                if (botResponse == null && !memoryMessage.getText().startsWith("username is")){
                    botResponse = memoryMessage.getText();
                }
            }
            log.info("Message History:{}", messageHis.toString());
            return ResponseEntity.ok(new ApiResponse(true, botResponse));
        } catch (RuntimeException e) {
            log.error(e.getMessage(),e);
            return ResponseEntity.badRequest().body(new ApiResponse(false, "处理异常，请查看后台日志"));
        }
    }

}