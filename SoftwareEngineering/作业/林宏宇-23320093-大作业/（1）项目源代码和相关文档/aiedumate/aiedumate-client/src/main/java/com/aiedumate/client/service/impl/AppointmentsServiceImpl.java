package com.aiedumate.client.service.impl;

import com.aiedumate.client.dao.AppointmentsMapper;
import com.aiedumate.client.entity.TblAppointments;
import com.aiedumate.client.entity.TblAppointmentsExample;
import com.aiedumate.client.service.AppointmentsService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.Date;
import java.util.List;

@Service
public class AppointmentsServiceImpl implements AppointmentsService {

	@Resource
	AppointmentsMapper appointmentsMapper;

	@Override
	public List<TblAppointments> query(String username, int year, int month) {
//		Date start = new Date(year, month, 1, 0, 0);
//		Date end = new Date(year, month+1, 1, 0, 0);

		LocalDateTime  localStart = LocalDateTime .of(year, month, 1, 0,0);
		LocalDateTime  localEnd = LocalDateTime .of(year, month+1, 1, 0,0);
		Date start = Date.from(localStart.atZone( ZoneId.systemDefault()).toInstant());
		Date end = Date.from(localEnd.atZone( ZoneId.systemDefault()).toInstant());

		TblAppointmentsExample example = new TblAppointmentsExample();
		TblAppointmentsExample.Criteria criteria = example.createCriteria();
		criteria.andUsernameEqualTo(username);

		criteria.andStartTimeGreaterThanOrEqualTo(start);
		criteria.andStartTimeLessThan(end);

		example.setOrderByClause("start_time,id");

		return appointmentsMapper.selectByExampleWithBLOBs(example);
	}

}
