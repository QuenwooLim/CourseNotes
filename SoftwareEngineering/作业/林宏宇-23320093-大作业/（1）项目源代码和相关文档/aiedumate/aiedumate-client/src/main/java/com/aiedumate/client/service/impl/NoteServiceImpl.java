package com.aiedumate.client.service.impl;

import com.aiedumate.client.dao.NotesMapper;
import com.aiedumate.client.entity.TblNotes;
import com.aiedumate.client.entity.TblNotesExample;
import com.aiedumate.client.service.NoteService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.Date;
import java.util.List;

@Service
public class NoteServiceImpl implements NoteService {

	@Resource
    NotesMapper notesMapper;

	@Override
	public List<TblNotes> getAllNotes(String username, String course, String title) {
		TblNotesExample example = new TblNotesExample();
		TblNotesExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(username);
		if(StringUtils.hasLength(course)){
			criteria.andCourseLike("%" + course.trim() + "%");
		}
		if(StringUtils.hasLength(title)){
			criteria.andTitleLike("%" + title.trim() + "%");
		}
		example.setOrderByClause("created_time,id");

		return notesMapper.selectByExample(example);
	}
	@Override
	public TblNotes getNoteById(String username, int id) {
		TblNotes notes = notesMapper.selectByPrimaryKey(id);
		if(notes == null){
			return null;
		}
		if(notes.getUsername().equals(username)){
			return notes;
		} else {
			return null;
		}
	}

	@Override
	public List<String> getAllCategories(String username) {
		return notesMapper.getAllCourse(username);
	}

	@Override
	public TblNotes insert(TblNotes note) {
		int result = notesMapper.insert(note);
		if(result == 1){
			return note;
		} else {
			return null;
		}
	}
	@Override
	public TblNotes update(TblNotes note) {
		int result = notesMapper.updateByPrimaryKey(note);
		if(result == 1){
			return note;
		} else {
			return null;
		}
	}
	@Override
	public int delete(int id) {
		return notesMapper.deleteByPrimaryKey(id);
	}
}
