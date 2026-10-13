package com.aiedumate.server.service.impl;

import com.aiedumate.server.dao.CourseSchedulesMapper;
import com.aiedumate.server.entity.TblCourseSchedules;
import com.aiedumate.server.entity.TblCourseSchedulesExample;
import com.aiedumate.server.service.CourseSchedulesService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.Date;
import java.util.List;

@Service
public class CourseSchedulesServiceImpl implements CourseSchedulesService {

	@Resource
    CourseSchedulesMapper courseSchedulesMapper;

	@Override
	public List<TblCourseSchedules> query(TblCourseSchedules entity) {
		TblCourseSchedulesExample example = new TblCourseSchedulesExample();
		TblCourseSchedulesExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(entity.getUsername());
		if(StringUtils.hasLength(entity.getCourse())){
			criteria.andCourseEqualTo(entity.getCourse());
		}
		if(StringUtils.hasLength(entity.getDayOfWeek())){
			criteria.andDayOfWeekEqualTo(entity.getDayOfWeek());
		}
		if(StringUtils.hasLength(entity.getTimeSlot())){
			criteria.andTimeSlotEqualTo(entity.getTimeSlot());
		}
		example.setOrderByClause("day_of_week,time_slot,start_time");

		return courseSchedulesMapper.selectByExample(example);
	}

	@Override
	public int insert(TblCourseSchedules entity){
		entity.setCreatedTime(new Date());
		entity.setUpdatedTime(entity.getCreatedTime());
		return courseSchedulesMapper.insert(entity);
	}
	@Override
	public int delete(TblCourseSchedules entity){
		TblCourseSchedulesExample example = new TblCourseSchedulesExample();
		TblCourseSchedulesExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(entity.getUsername());
		if(StringUtils.hasLength(entity.getCourse())){
			criteria.andCourseEqualTo(entity.getCourse());
		}
		if(StringUtils.hasLength(entity.getDayOfWeek())){
			criteria.andDayOfWeekEqualTo(entity.getDayOfWeek());
		}
		if(StringUtils.hasLength(entity.getTimeSlot())){
			criteria.andTimeSlotEqualTo(entity.getTimeSlot());
		}
		return courseSchedulesMapper.deleteByExample(example);
	}
}
