package com.aiedumate.client.controller;

import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
@Slf4j
public class StaticPageController {
    //获取登录静态页面
    @GetMapping("/login")
    public String login(Model model) {
        return "login";
    }

    //获取注册静态页面
    @GetMapping("/register")
    public String register(Model model) {
        return "register";
    }

    @GetMapping("/")
    public String root() {
        return "index";
    }

    @GetMapping("/home")
    public String home() {
        return "index";
    }

    @GetMapping("/video")
    public String video(Model model) {
        return "video";
    }

    @GetMapping("/chat")
    public String chat(Model model) {
        return "chat";
    }

    @GetMapping("/chat2")
    public String chat2(Model model) {
        return "chat2";
    }
    @GetMapping("/chat3")
    public String chat3(Model model) {
        return "chat3";
    }

    //获取课程表静态页面
    @GetMapping("/schedule")
    public String schedule(Model model) {
        return "schedule";
    }

    //获取预约行程静态页面
    @GetMapping("/appointment")
    public String appointment(Model model) {
        return "appointment";
    }

    //获取笔记本静态页面
    @GetMapping("/note")
    public String note(Model model) {
        return "note";
    }
}