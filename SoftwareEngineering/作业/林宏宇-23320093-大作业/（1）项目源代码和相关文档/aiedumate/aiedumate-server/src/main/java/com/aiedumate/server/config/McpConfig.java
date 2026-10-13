package com.aiedumate.server.config;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.aiedumate.server.tools.AppointmentsTool;
import com.aiedumate.server.tools.CourseSchedulesTool;
import com.aiedumate.server.tools.NotesTool;
import io.modelcontextprotocol.server.McpServer;
import io.modelcontextprotocol.server.McpServerFeatures;
import io.modelcontextprotocol.server.McpSyncServer;
import io.modelcontextprotocol.server.transport.WebFluxSseServerTransportProvider;
import io.modelcontextprotocol.spec.McpSchema;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.reactive.config.EnableWebFlux;
import org.springframework.web.reactive.function.server.RouterFunction;

@EnableWebFlux
@Configuration
@Slf4j
class McpConfig {
    @Autowired
    private CourseSchedulesTool courseSchedulesTool;
    @Autowired
    private AppointmentsTool appointmentsTool;
    @Autowired
    private NotesTool notesTool;
    @Bean
    WebFluxSseServerTransportProvider webFluxSseServerTransportProvider(ObjectMapper mapper) {
        return new WebFluxSseServerTransportProvider(mapper, "/mcp/message");
    }

    @Bean
    RouterFunction<?> mcpRouterFunction(WebFluxSseServerTransportProvider transportProvider) {
        return transportProvider.getRouterFunction();
    }

    @Bean(destroyMethod = "close")
    public McpSyncServer mcpSyncServer(WebFluxSseServerTransportProvider transportProvider) {
        McpSyncServer mcpSyncServer = McpServer.sync(transportProvider)
                .serverInfo("mcp-server", "1.0.0")
                .capabilities(McpSchema.ServerCapabilities.builder()
                        .tools(true)
                        .build())
                .build();
        // 添加工具
        McpServerFeatures.SyncToolSpecification toolHandler = new McpServerFeatures.SyncToolSpecification(courseSchedulesTool.getTool(), courseSchedulesTool.call);
        mcpSyncServer.addTool(toolHandler);

        toolHandler = new McpServerFeatures.SyncToolSpecification(appointmentsTool.getTool(), appointmentsTool.call);
        mcpSyncServer.addTool(toolHandler);

        toolHandler = new McpServerFeatures.SyncToolSpecification(notesTool.getTool(), notesTool.call);
        mcpSyncServer.addTool(toolHandler);

        log.info("MCP Server initilized successfully");

        return mcpSyncServer;
    }
}

