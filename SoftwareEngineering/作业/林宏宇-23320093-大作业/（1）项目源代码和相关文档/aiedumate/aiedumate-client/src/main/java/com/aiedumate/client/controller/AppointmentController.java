package com.aiedumate.client.controller;

import com.aiedumate.client.dto.AppointmentDto;
import com.aiedumate.client.entity.TblAppointments;
import com.aiedumate.client.service.AppointmentsService;
import jakarta.servlet.http.HttpServletRequest;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.ArrayList;
import java.util.List;

@Controller
@Slf4j
public class AppointmentController {
    @Autowired
    private AppointmentsService appointmentsService;

    //获取课程表数据
    @GetMapping("/api/appointment")
    public ResponseEntity<?> getAppointment(@RequestParam("year") int year,
                                            @RequestParam("month") int month,
                                            HttpServletRequest request) {
        try {
            String username = (String)request.getAttribute("username");

            List<TblAppointments> appointments = appointmentsService.query(username, year, month);

            List<AppointmentDto> dtoList = new ArrayList<>();
            for (TblAppointments entity : appointments) {
                AppointmentDto dto = new AppointmentDto();
                dto.setId(entity.getId());
                dto.setTitle(entity.getTitle());
                dto.setDate(entity.getStartTime());
                dto.setType("meeting");

                dtoList.add(dto);
            }

            return ResponseEntity.ok().body(dtoList);
        } catch (Exception e) {
            log.error(e.getMessage(),e);
            return ResponseEntity.internalServerError()
                    .body("获取预约行程失败: " + e.getMessage());
        }
    }

}