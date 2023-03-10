package com.djordjemilanovic.centralniserver.resources;

import javax.annotation.Resource;
import javax.jms.ConnectionFactory;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Queue;
import javax.ws.rs.GET;
import javax.ws.rs.HeaderParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import podsistem3.entities.Order1;

@Path("narudzbine")
public class Orders {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="orderSubsystemRequests") private Queue orderSubsystemRequests;
    @Resource(lookup="orderSubsystemResponses") private Queue orderSubsystemResponses;
    
    @Resource(lookup="userSubsystemRequests") private Queue userSubsystemRequests;
    @Resource(lookup="userSubsystemResponses") private Queue userSubsystemResponses;
    
    @POST
    public Response create(
        @HeaderParam("Authorization") String authString
    ) throws JMSException {
        String username;
        try {
            username= UserUtils.getUsername(authString);
        } catch(Exception e) {
            return ReturnUtils.returnBadRequest("Not logged in");
        }
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(orderSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
             
            int userId = UserUtils.getUserId(context, producer, userSubsystemRequests, userSubsystemResponses, username);
            if (userId == 0)
                return ReturnUtils.returnBadRequest("Bad user");
            
            MapMessage orderRequestMessage = context.createMapMessage();
            
            orderRequestMessage.setStringProperty("resource", "order");
            orderRequestMessage.setStringProperty("action", "create");
            
            orderRequestMessage.setInt("userId", userId);
            orderRequestMessage.setString("username", username);
            
            producer.send(orderSubsystemRequests, orderRequestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAll() throws JMSException{
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(orderSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "order");
            requestMessage.setStringProperty("action", "getAll");
            
            producer.send(orderSubsystemRequests, requestMessage);
            
            return ReturnUtils.<Order1>returnList(consumer);
        }
    }
    
    @GET
    @Path("korisnik")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAllByCurrentUser(
        @HeaderParam("Authorization") String authString
    ) throws JMSException{
        String username;
        try {
            username= UserUtils.getUsername(authString);
        } catch(Exception e) {
            return ReturnUtils.returnBadRequest("Not logged in");
        }
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(orderSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            int userId = UserUtils.getUserId(context, producer, userSubsystemRequests, userSubsystemResponses, username);
            if (userId == 0)
                return ReturnUtils.returnBadRequest("Bad user");
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "order");
            requestMessage.setStringProperty("action", "getAllByUserID");
            
            requestMessage.setInt("userId", userId);
            
            producer.send(orderSubsystemRequests, requestMessage);
            
            return ReturnUtils.<Order1>returnList(consumer);
        }
    }
}