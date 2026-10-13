package com.aiedumate.client.dto;

import lombok.Data;

import java.util.Date;

@Data
public class CourseSchedulesDto {
    private String course;

    private String teacher;

    private String classroom;

    private String dayOfWeek;

    private String timeSlot;

    private String startTime;

    private String endTime;

    private String remark;
}