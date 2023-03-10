package com.djordjemilanovic.centralniserver.resources;

import javax.annotation.Resource;
import javax.jms.ConnectionFactory;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Queue;
import javax.jms.TextMessage;
import javax.ws.rs.DELETE;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.HeaderParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("korpa")
public class Cart {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="itemSubsystemRequests") private Queue itemSubsystemRequests;
    @Resource(lookup="itemSubsystemResponses") private Queue itemSubsystemResponses;
    
    @Resource(lookup="userSubsystemRequests") private Queue userSubsystemRequests;
    @Resource(lookup="userSubsystemResponses") private Queue userSubsystemResponses;
    
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response getCart(
        @HeaderParam("Authorization") String authString
    ) throws JMSException {        
        String username;
        try {
            username= UserUtils.getUsername(authString);
        } catch(Exception e) {
            return ReturnUtils.returnBadRequest("Not logged in");
        }
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(itemSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            int userId = UserUtils.getUserId(context, producer, userSubsystemRequests, userSubsystemResponses, username);
            if (userId == 0)
                return ReturnUtils.returnBadRequest("Bad user");
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "cart");
            requestMessage.setStringProperty("action", "get");
            
            requestMessage.setInt("userId", userId);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            return ReturnUtils.<Cart>returnEntity(consumer);
        }
    }
    
    @POST
    public Response addItem(
        @HeaderParam("Authorization") String authString,
        @FormParam("artikal") Integer itemId,
        @FormParam("kolicina") Integer quantity
    ) throws JMSException {
        String username;
        try {
            username= UserUtils.getUsername(authString);
        } catch(Exception e) {
            return ReturnUtils.returnBadRequest("Not logged in");
        }
        
        if (itemId == null || quantity == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(itemSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            int userId = UserUtils.getUserId(context, producer, userSubsystemRequests, userSubsystemResponses, username);
            if (userId == 0)
                return ReturnUtils.returnBadRequest("Bad user");
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "cart");
            requestMessage.setStringProperty("action", "addItem");
            
            requestMessage.setInt("userId", userId);
            requestMessage.setInt("itemId", itemId);
            requestMessage.setInt("quantity", quantity);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @DELETE
    @Path("{itemId}")
    public Response removeItem(
        @HeaderParam("Authorization") String authString,
        @PathParam("itemId") Integer itemId
    ) throws JMSException {
        String username;
        try {
            username= UserUtils.getUsername(authString);
        } catch(Exception e) {
            return ReturnUtils.returnBadRequest("Not logged in");
        }
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(itemSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            int userId = UserUtils.getUserId(context, producer, userSubsystemRequests, userSubsystemResponses, username);
            if (userId == 0)
                return ReturnUtils.returnBadRequest("Bad user");
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "cart");
            requestMessage.setStringProperty("action", "removeItem");
            
            requestMessage.setInt("userId", userId);
            requestMessage.setInt("itemId", itemId);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
}