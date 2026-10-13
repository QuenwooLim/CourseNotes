package com.aiedumate.client.controller;

import com.aiedumate.client.dto.CategoryDto;
import com.aiedumate.client.dto.NoteRequestDto;
import com.aiedumate.client.dto.NoteResponseDto;
import com.aiedumate.client.entity.TblNotes;
import com.aiedumate.client.service.NoteService;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

@RestController
@RequiredArgsConstructor
public class NoteController {
    @Autowired
    private NoteService noteService;

    private NoteResponseDto convertToDto(TblNotes notes){
        NoteResponseDto dto = new NoteResponseDto();
        dto.setId(notes.getId().toString());
        dto.setTitle(notes.getTitle());
        dto.setCategory(notes.getCourse());
        dto.setContent(notes.getContent());
        dto.setLastUpdated(notes.getUpdatedTime());
        return dto;
    }
    private TblNotes convertToEntity(NoteRequestDto dto) {
        TblNotes note = new TblNotes();
        note.setTitle(dto.getTitle());
        note.setCourse(dto.getCategory());
        note.setContent(dto.getContent());
        return note;
    }

    @GetMapping("/api/notes/list")
    public ResponseEntity<List<NoteResponseDto>> getAllNotes(HttpServletRequest request) {
        String username = (String)request.getAttribute("username");
        List<TblNotes> notesList = noteService.getAllNotes(username, null, null);
        List<NoteResponseDto> dtoList = new ArrayList<>();
        for(TblNotes notes : notesList){
            dtoList.add(convertToDto(notes));
        }
        return ResponseEntity.ok(dtoList);
    }

    @GetMapping("/api/notes/content")
    public ResponseEntity<NoteResponseDto> getNoteById(@RequestParam("id") int id, HttpServletRequest request) {
        String username = (String)request.getAttribute("username");
        TblNotes notes = noteService.getNoteById(username, id);
        return ResponseEntity.ok(convertToDto(notes));
    }
    @GetMapping("/api/notes/categories")
    public ResponseEntity<List<CategoryDto>> getAllCategories(HttpServletRequest request) {
        String username = (String)request.getAttribute("username");
        List<String> courseList = noteService.getAllCategories(username);
        List<CategoryDto> dtoList = new ArrayList<>();
//        Integer id = 1;
        for(String course : courseList){
            CategoryDto dto = new CategoryDto();
            dto.setId(course);
            dto.setName(course);
            dtoList.add(dto);
//            id++;
        }
        return ResponseEntity.ok(dtoList);
    }
    @PostMapping("/api/notes/add")
    public ResponseEntity<NoteResponseDto> addNotes(@Valid @RequestBody NoteRequestDto noteDto, HttpServletRequest request) {
        String username = (String)request.getAttribute("username");

        TblNotes notes = convertToEntity(noteDto);
        notes.setUsername(username);
        notes.setCreatedTime(new Date());
        notes.setUpdatedTime(notes.getCreatedTime());
        TblNotes respNotes = noteService.insert(notes);

        return ResponseEntity.ok(convertToDto(respNotes));
    }
    @PutMapping("/api/notes/update")
    public ResponseEntity<NoteResponseDto> updateNotes(@Valid @RequestBody NoteRequestDto noteDto, HttpServletRequest request) {
        String username = (String)request.getAttribute("username");
        TblNotes notes = convertToEntity(noteDto);
        notes.setUpdatedTime(new Date());
        TblNotes respNotes = noteService.update(notes);
        return ResponseEntity.ok(convertToDto(respNotes));
    }
    @DeleteMapping("/api/notes/delete")
    public ResponseEntity<Void> deleteNotes(@RequestParam("id") int id, HttpServletRequest request) {
        String username = (String)request.getAttribute("username");
        noteService.delete(id);
        return ResponseEntity.noContent().build();
    }
}
