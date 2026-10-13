package com.aiedumate.server.service;

import com.aiedumate.server.entity.TblAppointments;

import java.util.List;

public interface AppointmentsService {

	List<TblAppointments> query(TblAppointments entity);

	int insert(TblAppointments entity);

	int delete(TblAppointments entity);
}
