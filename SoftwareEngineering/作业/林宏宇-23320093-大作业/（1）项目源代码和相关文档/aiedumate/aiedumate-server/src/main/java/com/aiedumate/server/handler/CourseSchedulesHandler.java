package com.aiedumate.server.handler;

import com.aiedumate.server.entity.TblCourseSchedules;
import com.aiedumate.server.service.CourseSchedulesService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
@Slf4j
public class CourseSchedulesHandler {
    @Autowired
    private CourseSchedulesService courseSchedulesService;
    private String printCourse(TblCourseSchedules courseSchedules) {
        String result = String.format("username:%s,course:%s,dayOfWeek:%s,timeSlot:%s,startTime:%s,endTime:%s,teacher:%s,classroom:%s,remark:%s",
                courseSchedules.getUsername(),
                courseSchedules.getCourse(),
                courseSchedules.getDayOfWeek(),
                courseSchedules.getTimeSlot(),
                courseSchedules.getStartTime(),
                courseSchedules.getEndTime(),
                courseSchedules.getTeacher(),
                courseSchedules.getClassroom(),
                courseSchedules.getRemark());
        return result;
    }

    public String createCourse(TblCourseSchedules courseSchedules) {
        log.info(printCourse(courseSchedules));
        int result = courseSchedulesService.insert(courseSchedules);
        log.info("insert result:{}", result);

        String resultText;
        if(result == 1){
            resultText = "添加课程成功！";
        } else {
            resultText = "添加课程失败，请重试！";
        }
        return resultText;
    }

    public String deleteCourse(TblCourseSchedules courseSchedules) {
        log.info(printCourse(courseSchedules));
        int result = courseSchedulesService.delete(courseSchedules);
        log.info("delete result:{}", result);

        String resultText;
        if(result >= 0){
            resultText = "删除课程成功，数量：" +  result;
        } else {
            resultText = "删除课程失败，请重试！";
        }
        return resultText;
    }

    public String queryCourse(TblCourseSchedules courseSchedules) {
        log.info(printCourse(courseSchedules));
        List<TblCourseSchedules> courseSchedulesList = courseSchedulesService.query(courseSchedules);
        log.info("query result:{}", courseSchedulesList.size());

        StringBuilder  resultText = new StringBuilder();
        for(TblCourseSchedules entity : courseSchedulesList) {
            String line = String.format("星期：%s, 时间段: %s, 时间：%s, 课程：%s\\n"
                    , entity.getDayOfWeek()
                    , entity.getTimeSlot()
                    , entity.getStartTime() + "-" + entity.getEndTime()
                    , entity.getCourse());
            resultText.append(line);
        }

        return resultText.toString();
    }
}
