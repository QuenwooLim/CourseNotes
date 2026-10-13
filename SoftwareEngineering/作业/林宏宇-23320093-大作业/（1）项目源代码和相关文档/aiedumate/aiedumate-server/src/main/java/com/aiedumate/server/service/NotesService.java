package com.aiedumate.server.service;

import com.aiedumate.server.entity.TblNotes;

import java.util.List;

public interface NotesService {

	List<TblNotes> query(TblNotes entity);

	int insert(TblNotes entity);

	int delete(TblNotes entity);
}
