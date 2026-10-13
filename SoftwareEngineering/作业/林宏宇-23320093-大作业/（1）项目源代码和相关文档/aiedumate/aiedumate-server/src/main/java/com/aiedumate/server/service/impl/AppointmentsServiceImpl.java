package com.aiedumate.server.service.impl;

import com.aiedumate.server.dao.AppointmentsMapper;
import com.aiedumate.server.entity.TblAppointments;
import com.aiedumate.server.entity.TblAppointmentsExample;
import com.aiedumate.server.service.AppointmentsService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.Date;
import java.util.List;

@Service
public class AppointmentsServiceImpl implements AppointmentsService {

	@Resource
    AppointmentsMapper appointmentsMapper;

	@Override
	public List<TblAppointments> query(TblAppointments entity) {
		TblAppointmentsExample example = new TblAppointmentsExample();
		TblAppointmentsExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(entity.getUsername());
		if(StringUtils.hasLength(entity.getTitle())){
			criteria.andTitleLike("%" + entity.getTitle() + "%");
		}
		if(entity.getStartTime() != null){
			criteria.andStartTimeGreaterThanOrEqualTo(entity.getStartTime());
		}
		if(entity.getEndTime() != null){
			criteria.andEndTimeLessThanOrEqualTo(entity.getEndTime());
		}
		if(StringUtils.hasLength(entity.getLocation())){
			criteria.andLocationLike("%" + entity.getLocation()+ "%");
		}
		example.setOrderByClause("start_time,id");

		return appointmentsMapper.selectByExampleWithBLOBs(example);
	}

	@Override
	public int insert(TblAppointments entity){
		entity.setCreatedTime(new Date());
		entity.setUpdatedTime(entity.getCreatedTime());
		return appointmentsMapper.insert(entity);
	}
	@Override
	public int delete(TblAppointments entity){
		TblAppointmentsExample example = new TblAppointmentsExample();
		TblAppointmentsExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(entity.getUsername());
		if(StringUtils.hasLength(entity.getTitle())){
			criteria.andTitleLike("%" + entity.getTitle() + "%");
		}
		if(entity.getStartTime() != null){
			criteria.andStartTimeGreaterThanOrEqualTo(entity.getStartTime());
		}
		if(entity.getEndTime() != null){
			criteria.andEndTimeLessThanOrEqualTo(entity.getEndTime());
		}
		if(StringUtils.hasLength(entity.getLocation())){
			criteria.andLocationLike("%" +entity.getLocation()+ "%");
		}
		return appointmentsMapper.deleteByExample(example);
	}
}
