package com.aiedumate.client.service.impl;

import com.aiedumate.client.dao.CourseSchedulesMapper;
import com.aiedumate.client.entity.TblCourseSchedulesExample;
import com.aiedumate.client.entity.TblCourseSchedules;
import com.aiedumate.client.service.CourseSchedulesService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class CourseSchedulesServiceImpl implements CourseSchedulesService {

	@Resource
    CourseSchedulesMapper courseSchedulesMapper;

	@Override
	public List<TblCourseSchedules> query(String username) {
		TblCourseSchedulesExample example = new TblCourseSchedulesExample();
		TblCourseSchedulesExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(username);

		example.setOrderByClause("day_of_week,time_slot,start_time");

		return courseSchedulesMapper.selectByExample(example);
	}

}
