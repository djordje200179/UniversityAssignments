package com.djordjemilanovic.centralniserver.resources;

import javax.annotation.Resource;
import javax.jms.ConnectionFactory;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Queue;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import podsistem2.entities.Category;

@Path("kategorije")
public class Categories {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="itemSubsystemRequests") private Queue subsystemRequests;
    @Resource(lookup="itemSubsystemResponses") private Queue subsystemResponses;
    
    @POST
    public Response create(
        @FormParam("ime") String name,
        @FormParam("natkategorija") String supercategoryName
    ) throws JMSException {
        if (name == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "category");
            requestMessage.setStringProperty("action", "create");
            
            requestMessage.setString("name", name);
            if(supercategoryName != null)
                requestMessage.setString("supercategoryName", supercategoryName);
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAll() throws JMSException {
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "category");
            requestMessage.setStringProperty("action", "getAll");
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.<Category>returnList(consumer);
        }
    }
}
