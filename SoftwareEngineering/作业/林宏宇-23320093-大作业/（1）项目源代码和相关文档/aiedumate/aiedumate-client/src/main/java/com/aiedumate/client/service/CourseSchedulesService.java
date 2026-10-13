package com.aiedumate.client.service;

import com.aiedumate.client.entity.TblCourseSchedules;

import java.util.List;

public interface CourseSchedulesService {
	List<TblCourseSchedules> query(String username);
}
