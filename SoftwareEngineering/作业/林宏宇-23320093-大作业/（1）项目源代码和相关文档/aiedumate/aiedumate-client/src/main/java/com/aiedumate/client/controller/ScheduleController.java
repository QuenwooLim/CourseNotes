package com.aiedumate.client.controller;

import com.aiedumate.client.dto.CourseSchedulesDto;
import com.aiedumate.client.entity.TblCourseSchedules;
import com.aiedumate.client.service.CourseSchedulesService;
import jakarta.servlet.http.HttpServletRequest;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

import java.util.ArrayList;
import java.util.List;

@Controller
@Slf4j
public class ScheduleController {
    @Autowired
    private CourseSchedulesService courseSchedulesService;

    //获取课程表数据
    @GetMapping("/api/schedule")
    public ResponseEntity<?> getSchedule(HttpServletRequest request) {
        try {
            String username = (String)request.getAttribute("username");
            List<TblCourseSchedules> schedules = courseSchedulesService.query(username);

            List<CourseSchedulesDto> schedulesDto = new ArrayList<>();
            for (TblCourseSchedules entity : schedules) {
                CourseSchedulesDto dto = new CourseSchedulesDto();
                BeanUtils.copyProperties(entity, dto);

                schedulesDto.add(dto);
            }

            return ResponseEntity.ok().body(schedulesDto);
        } catch (Exception e) {
            log.error(e.getMessage(),e);
            return ResponseEntity.internalServerError()
                    .body("获取课程表失败: " + e.getMessage());
        }
    }

}