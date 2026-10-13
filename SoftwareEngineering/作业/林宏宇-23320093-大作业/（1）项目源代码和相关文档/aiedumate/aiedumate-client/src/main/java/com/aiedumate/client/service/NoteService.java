package com.aiedumate.client.service;

import com.aiedumate.client.entity.TblNotes;

import java.util.List;

public interface NoteService {

	List<TblNotes> getAllNotes(String username, String course, String title);

	TblNotes getNoteById(String username, int id);

	List<String> getAllCategories(String username);

	TblNotes insert(TblNotes note);

	TblNotes update(TblNotes note);
	int delete(int id);
}
