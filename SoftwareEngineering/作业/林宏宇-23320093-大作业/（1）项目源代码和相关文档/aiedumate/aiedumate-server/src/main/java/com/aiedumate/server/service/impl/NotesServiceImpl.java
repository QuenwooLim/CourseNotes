package com.aiedumate.server.service.impl;

import com.aiedumate.server.dao.NotesMapper;
import com.aiedumate.server.entity.TblNotes;
import com.aiedumate.server.entity.TblNotesExample;
import com.aiedumate.server.service.NotesService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.Date;
import java.util.List;

@Service
public class NotesServiceImpl implements NotesService {

	@Resource
	NotesMapper notesMapper;

	@Override
	public List<TblNotes> query(TblNotes entity) {
		TblNotesExample example = new TblNotesExample();
		TblNotesExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(entity.getUsername());
		if(StringUtils.hasLength(entity.getCourse())){
			criteria.andCourseLike("%" + entity.getCourse() + "%");
		}
		if(StringUtils.hasLength(entity.getTitle())){
			criteria.andTitleLike("%" + entity.getTitle() + "%");
		}
		example.setOrderByClause("created_time,id");

		return notesMapper.selectByExampleWithBLOBs(example);
	}

	@Override
	public int insert(TblNotes entity){
		entity.setCreatedTime(new Date());
		entity.setUpdatedTime(entity.getCreatedTime());
		return notesMapper.insert(entity);
	}

	@Override
	public int delete(TblNotes entity){
		TblNotesExample example = new TblNotesExample();
		TblNotesExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(entity.getUsername());
		if(StringUtils.hasLength(entity.getCourse())){
			criteria.andCourseLike("%" + entity.getCourse() + "%");
		}
		if(StringUtils.hasLength(entity.getTitle())){
			criteria.andTitleLike("%" + entity.getTitle() + "%");
		}
		return notesMapper.deleteByExample(example);
	}
}
