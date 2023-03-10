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
import podsistem1.entities.City;

@Path("gradovi")
public class Cities {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="userSubsystemRequests") private Queue subsystemRequests;
    @Resource(lookup="userSubsystemResponses") private Queue subsystemResponses;
   
    @POST
    public Response create(
        @FormParam("ime") String name
    ) throws JMSException {
        if (name == null)
           return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "city");
            requestMessage.setStringProperty("action", "create");
            
            requestMessage.setString("name", name);
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAll() throws JMSException{
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "city");
            requestMessage.setStringProperty("action", "getAll");
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.<City>returnList(consumer);
        }
    }
}