package com.aiedumate.client.dto;

import lombok.Data;
import java.time.LocalDate;
import java.util.Date;
import java.util.List;

@Data
public class NoteResponseDto {
    private String id;
    private String title;
    private String category;
    private Date lastUpdated;
    private List<String> tags;
    private String content;
}