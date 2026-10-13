package com.aiedumate.server.tools;

import com.aiedumate.server.entity.TblCourseSchedules;
import com.aiedumate.server.handler.CourseSchedulesHandler;
import io.modelcontextprotocol.server.McpSyncServerExchange;
import io.modelcontextprotocol.spec.McpSchema;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import java.util.Map;
import java.util.function.BiFunction;

@Component
@Slf4j
public class CourseSchedulesTool {
	@Autowired
	private CourseSchedulesHandler courseSchedulesHandler;
    public McpSchema.Tool getTool(){
        String jsonSchema = """  
		{  
			"type":"object",  
			"properties":{  
				"oprType" : {  
					"type": "string",  
					"enum": ["insert", "delete", "query"],
					"description": "操作类型"  
				},
				"username" : {  
					"type": "string",  
					"description": "用户名"  
				},				
				"course" : {  
					"type": "string",  
					"description": "课程"  
				},
				"teacher" : {  
					"type": "string",  
					"description": "教师"  
				},
				"classroom" : {  
					"type": "string",  
					"description": "教室"  
				},
				"dayOfWeek" : {  
					"type": "string",  
					"enum": ["星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"],
					"description": "星期"  
				},
				"timeSlot" : {  
					"type": "string",  
					"enum": ["上午", "下午", "晚上"],
					"description": "时间段"  
				},
				"startTime" : {  
					"type": "string",  
					"description": "开始时间"  
				},
				"endTime" : {  
					"type": "string",  
					"description": "结束时间"  
				},
				"remark" : {  
					"type": "string",  
					"description": "备注"  
				}  
			},  
			"required":[  
				"oprType", "username"
			],  
			"additionalProperties":false  
		}  
		""";
        // 方法的描述
        McpSchema.Tool tool = new McpSchema.Tool("courseManagement", "课程管理", jsonSchema);
		return tool;
    }

	public BiFunction<McpSyncServerExchange, Map<String, Object>, McpSchema.CallToolResult> call = (exchange, arguments) -> {
		//从接口中获取数据
		String oprType = String.valueOf(arguments.get("oprType"));
		String username = String.valueOf(arguments.get("username"));
		if(!StringUtils.hasLength(oprType)){
			log.error("操作类型为空");
			return new McpSchema.CallToolResult("操作类型不能为空", false);
		}
		if (!StringUtils.hasLength(username)) {
			log.error("用户名为空");
			return new McpSchema.CallToolResult("用户名不能为空", false);
		}
		String course = String.valueOf(arguments.get("course"));
		String teacher = String.valueOf(arguments.get("teacher"));
		String classroom = String.valueOf(arguments.get("classroom"));
		String dayOfWeek = String.valueOf(arguments.get("dayOfWeek"));
		String timeSlot = String.valueOf(arguments.get("timeSlot"));
		String startTime = String.valueOf(arguments.get("startTime"));
		String endTime = String.valueOf(arguments.get("endTime"));
		String remark = String.valueOf(arguments.get("remark"));

		//构建实体对象
		TblCourseSchedules courseSchedules = new TblCourseSchedules();
		courseSchedules.setUsername(username);

		if(!"null".equals(course)) {
			courseSchedules.setCourse(course);
		}
		if(!"null".equals(teacher)) {
			courseSchedules.setTeacher(teacher);
		}
		if(!"null".equals(classroom)) {
			courseSchedules.setClassroom(classroom);
		}
		if(!"null".equals(dayOfWeek)) {
			courseSchedules.setDayOfWeek(dayOfWeek);
		}
		if(!"null".equals(timeSlot)) {
			courseSchedules.setTimeSlot(timeSlot);
		}
		if(!"null".equals(startTime)) {
			courseSchedules.setStartTime(startTime);
		}
		if(!"null".equals(endTime)) {
			courseSchedules.setEndTime(endTime);
		}
		if(!"null".equals(remark)) {
			courseSchedules.setRemark(remark);
		}

		//根据操作类型进行对应处理
		String resultText;
		if("insert".equals(oprType)){
			if(!StringUtils.hasLength(course)){
				log.error("课程为空");
				return new McpSchema.CallToolResult("课程不能为空", false);
			}
			if(!StringUtils.hasLength(dayOfWeek)){
				log.error("星期为空");
				return new McpSchema.CallToolResult("星期不能为空", false);
			}
			if(!StringUtils.hasLength(timeSlot)){
				log.error("时间段为空");
				return new McpSchema.CallToolResult("时间段不能为空", false);
			}
			if(!StringUtils.hasLength(startTime)){
				log.error("开始时间为空");
				return new McpSchema.CallToolResult("开始时间不能为空", false);
			}
			if(!StringUtils.hasLength(endTime)){
				log.error("结束时间为空");
				return new McpSchema.CallToolResult("结束时间不能为空", false);
			}
			//调用处理器
			resultText = courseSchedulesHandler.createCourse(courseSchedules);
		} else if("delete".equals(oprType)){
			//调用处理器
			resultText = courseSchedulesHandler.deleteCourse(courseSchedules);
		} else if("query".equals(oprType)){
			//支持查询所有
			//调用处理器
			resultText = courseSchedulesHandler.queryCourse(courseSchedules);
		} else {
			resultText = "操作类型不正确，必须是insert,delete或者query";
		}

		//返回处理结果
		log.info("resultText:{}", resultText);
		return new McpSchema.CallToolResult(resultText, false);
	};
}
