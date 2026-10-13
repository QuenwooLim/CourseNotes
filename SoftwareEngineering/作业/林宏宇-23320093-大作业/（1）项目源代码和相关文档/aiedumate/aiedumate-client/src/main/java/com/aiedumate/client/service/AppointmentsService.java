package com.aiedumate.client.service;

import com.aiedumate.client.entity.TblAppointments;

import java.util.List;

public interface AppointmentsService {
	List<TblAppointments> query(String username, int year, int month);
}
