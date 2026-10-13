package com.aiedumate.server.service;

import com.aiedumate.server.entity.TblCourseSchedules;

import java.util.List;

public interface CourseSchedulesService {

	List<TblCourseSchedules> query(TblCourseSchedules entity);

	int insert(TblCourseSchedules entity);

	int delete(TblCourseSchedules entity);
}
