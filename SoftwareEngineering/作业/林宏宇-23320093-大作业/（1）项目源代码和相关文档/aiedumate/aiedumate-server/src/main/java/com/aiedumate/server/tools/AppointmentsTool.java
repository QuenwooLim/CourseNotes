package com.aiedumate.server.tools;

import com.aiedumate.server.entity.TblAppointments;
import com.aiedumate.server.handler.AppointmentsHandler;
import io.modelcontextprotocol.server.McpSyncServerExchange;
import io.modelcontextprotocol.spec.McpSchema;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import java.text.SimpleDateFormat;
import java.util.Map;
import java.util.function.BiFunction;

@Component
@Slf4j
public class AppointmentsTool {
	@Autowired
	private AppointmentsHandler appointmentsHandler;
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
				"title" : {  
					"type": "string",  
					"description": "预约标题"  
				},
				"content" : {  
					"type": "string",  
					"description": "预约详情"  
				},
				"startTime" : {  
					"type": "string",  
					"format": "date-time",
					"description": "开始时间"  
				},
				"endTime" : {  
					"type": "string",  
					"format": "date-time",
					"description": "结束时间"  
				},
				"location" : {
					"type": "string",  
					"description": "预约地点"  
				},
				"attendees" : {  
					"type": "string",  
					"description": "参与者"  
				}  
			},  
			"required":[  
				"oprType", "username"
			],  
			"additionalProperties":false  
		}  
		""";
        // 方法的描述
        McpSchema.Tool tool = new McpSchema.Tool("appointmentsManagement", "预约管理", jsonSchema);
		return tool;
    }

	public BiFunction<McpSyncServerExchange, Map<String, Object>, McpSchema.CallToolResult> call = (exchange, arguments) -> {
		//从接口中获取数据
		try {
			String oprType = String.valueOf(arguments.get("oprType"));
			String username = String.valueOf(arguments.get("username"));
			if (!StringUtils.hasLength(oprType)) {
				log.error("操作类型为空");
				return new McpSchema.CallToolResult("操作类型不能为空", false);
			}
			if (!StringUtils.hasLength(username)) {
				log.error("用户名为空");
				return new McpSchema.CallToolResult("用户名不能为空", false);
			}
			String title = String.valueOf(arguments.get("title"));
			String content = String.valueOf(arguments.get("content"));
			String startTime = String.valueOf(arguments.get("startTime"));
			String endTime = String.valueOf(arguments.get("endTime"));
			String location = String.valueOf(arguments.get("location"));
			String attendees = String.valueOf(arguments.get("attendees"));

			//构建实体对象
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
			TblAppointments appointments = new TblAppointments();
			appointments.setUsername(username);

			if (!"null".equals(title)) {
				appointments.setTitle(title);
			}
			if (!"null".equals(content)) {
				appointments.setContent(content);
			}
			if (!"null".equals(startTime)) {
				appointments.setStartTime(sdf.parse(startTime));
			}
			if (!"null".equals(endTime)) {
				appointments.setEndTime(sdf.parse(endTime));
			}
			if (!"null".equals(location)) {
				appointments.setLocation(location);
			}
			if (!"null".equals(attendees)) {
				appointments.setAttendees(attendees);
			}

			//根据操作类型进行对应处理
			String resultText;
			if ("insert".equals(oprType)) {
				if (!StringUtils.hasLength(title)) {
					log.error("预约标题为空");
					return new McpSchema.CallToolResult("预约标题不能为空", false);
				}
				if (!StringUtils.hasLength(content)) {
					log.error("预约详情为空");
					return new McpSchema.CallToolResult("预约详情不能为空", false);
				}
				//调用处理器
				resultText = appointmentsHandler.createAppointments(appointments);
			} else if ("delete".equals(oprType)) {
				//调用处理器
				resultText = appointmentsHandler.deleteAppointments(appointments);
			} else if ("query".equals(oprType)) {
				//支持查询所有
				//调用处理器
				resultText = appointmentsHandler.queryAppointments(appointments);
			} else {
				resultText = "操作类型不正确，必须是insert,delete或者query";
			}

			//返回处理结果
			log.info("resultText:{}", resultText);
			return new McpSchema.CallToolResult(resultText, false);
		} catch (Exception ex) {
			log.error(ex.getMessage(),ex);
			return new McpSchema.CallToolResult("处理异常", false);
		}
	};
}
