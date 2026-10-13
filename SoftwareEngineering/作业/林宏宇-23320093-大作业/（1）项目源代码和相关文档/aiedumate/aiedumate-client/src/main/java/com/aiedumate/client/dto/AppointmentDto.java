package com.aiedumate.client.dto;

import lombok.Data;

import java.util.Date;

@Data
public class AppointmentDto {
    private long id;
    private String title;
    private Date date;
    private String type;
}
