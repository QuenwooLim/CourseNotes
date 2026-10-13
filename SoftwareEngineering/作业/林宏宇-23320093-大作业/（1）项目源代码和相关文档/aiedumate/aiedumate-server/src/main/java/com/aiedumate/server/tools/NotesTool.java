package com.aiedumate.server.tools;

import com.aiedumate.server.entity.TblNotes;
import com.aiedumate.server.handler.NotesHandler;
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
public class NotesTool {
	@Autowired
	private NotesHandler notesHandler;
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
				"title" : {  
					"type": "string",  
					"description": "笔记标题"  
				},
				"content" : {  
					"type": "string",  
					"description": "笔记内容"  
				}
			},  
			"required":[  
				"oprType", "username"
			],  
			"additionalProperties":false  
		}  
		""";
        // 方法的描述
        McpSchema.Tool tool = new McpSchema.Tool("notesManagement", "笔记管理", jsonSchema);
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
		String title = String.valueOf(arguments.get("title"));
		String content = String.valueOf(arguments.get("content"));

		//构建实体对象
		TblNotes notes = new TblNotes();
		notes.setUsername(username);

		if(!"null".equals(course)) {
			notes.setCourse(course);
		}
		if(!"null".equals(title)) {
			notes.setTitle(title);
		}
		if(!"null".equals(content)) {
			notes.setContent(content);
		}

		//根据操作类型进行对应处理
		String resultText;
		if("insert".equals(oprType)){
			if(!StringUtils.hasLength(title)){
				log.error("预约标题为空");
				return new McpSchema.CallToolResult("预约标题不能为空", false);
			}
			if(!StringUtils.hasLength(content)){
				log.error("预约详情为空");
				return new McpSchema.CallToolResult("预约详情不能为空", false);
			}
			//调用处理器
			resultText = notesHandler.createNotes(notes);
		} else if("delete".equals(oprType)){
			//调用处理器
			resultText = notesHandler.deleteNotes(notes);
		} else if("query".equals(oprType)){
			//支持查询所有
			//调用处理器
			resultText = notesHandler.queryNotes(notes);
		} else {
			resultText = "操作类型不正确，必须是insert,delete或者query";
		}

		//返回处理结果
		log.info("resultText:{}", resultText);
		return new McpSchema.CallToolResult(resultText, false);
	};
}
