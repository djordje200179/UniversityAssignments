package com.djordjemilanovic.centralniserver.resources;

import javax.annotation.Resource;
import javax.jms.ConnectionFactory;
import javax.jms.Queue;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.core.Response;

@Path("recenzije")
public class Reviews {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="itemSubsystemRequests") private Queue subsystemRequests;
    @Resource(lookup="itemSubsystemResponses") private Queue subsystemResponses;
    
    @GET
    public Response ping(){
        return Response
            .ok("pong")
            .build();
    }
}
