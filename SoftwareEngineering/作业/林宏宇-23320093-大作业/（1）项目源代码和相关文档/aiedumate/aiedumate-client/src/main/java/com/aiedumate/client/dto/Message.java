package com.aiedumate.client.dto;


public class Message {
    private String authorization;
    private String oldConversationId;
    private String conversationId;
    private String content;
    private boolean isUser;

    // 必须有默认构造函数
    public Message() {}

    public Message(String conversationId, String content, boolean isUser) {
        this.conversationId = conversationId;
        this.content = content;
        this.isUser = isUser;
    }

    public String getAuthorization() {
        return authorization;
    }

    public void setAuthorization(String authorization) {
        this.authorization = authorization;
    }

    public String getConversationId() {
        return conversationId;
    }

    public void setConversationId(String conversationId) {
        this.conversationId = conversationId;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public boolean isUser() {
        return isUser;
    }

    public void setUser(boolean user) {
        isUser = user;
    }

    public String getOldConversationId() {
        return oldConversationId;
    }

    public void setOldConversationId(String oldConversationId) {
        this.oldConversationId = oldConversationId;
    }
}