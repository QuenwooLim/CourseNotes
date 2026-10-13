package com.aiedumate.client.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.util.List;

@Data
public class NoteRequestDto {
    @NotBlank(message = "Title is required")
    private String title;

    private String category;
    private List<String> tags;
    private String content;
}