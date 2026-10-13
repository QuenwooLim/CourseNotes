package com.aiedumate.server.handler;

import com.aiedumate.server.entity.TblAppointments;
import com.aiedumate.server.service.AppointmentsService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
@Slf4j
public class AppointmentsHandler {
    @Autowired
    private AppointmentsService appointmentsService;
    private String printAppointments(TblAppointments appointments) {
        String result = String.format("username:%s,title:%s,startTime:%s,endTime:%s,location:%s,attendees:%s,content:%s",
                appointments.getUsername(),
                appointments.getTitle(),
                appointments.getStartTime(),
                appointments.getEndTime(),
                appointments.getLocation(),
                appointments.getAttendees(),
                appointments.getContent());

        return result;
    }

    public String createAppointments(TblAppointments appointments) {
        log.info(printAppointments(appointments));
        int result = appointmentsService.insert(appointments);
        log.info("insert result:{}", result);

        String resultText;
        if(result == 1){
            resultText = "添加预约成功！";
        } else {
            resultText = "添加预约失败，请重试！";
        }
        return resultText;
    }

    public String deleteAppointments(TblAppointments appointments) {
        log.info(printAppointments(appointments));
        int result = appointmentsService.delete(appointments);
        log.info("delete result:{}", result);

        String resultText;
        if(result >= 0){
            resultText = "删除预约成功，数量：" +  result;
        } else {
            resultText = "删除预约失败，请重试！";
        }
        return resultText;
    }

    public String queryAppointments(TblAppointments appointments) {
        log.info(printAppointments(appointments));
        List<TblAppointments> appointmentsList = appointmentsService.query(appointments);
        log.info("query result:{}", appointmentsList.size());

        StringBuilder  resultText = new StringBuilder();
        for(TblAppointments entity : appointmentsList) {
            String line = String.format("预约标题：%s, 预约详情: %s, 时间：%s, 预约地点：%s, 参与者：%s\\n"
                    , entity.getTitle()
                    , entity.getContent()
                    , entity.getStartTime() + "-" + entity.getEndTime()
                    , entity.getLocation()
                    , entity.getAttendees());
            resultText.append(line);
        }

        return resultText.toString();
    }
}
