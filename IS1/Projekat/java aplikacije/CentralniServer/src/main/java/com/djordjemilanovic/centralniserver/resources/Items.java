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
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.HeaderParam;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import podsistem2.entities.Item;

@Path("artikli")
public class Items {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="itemSubsystemRequests") private Queue itemSubsystemRequests;
    @Resource(lookup="itemSubsystemResponses") private Queue itemSubsystemResponses;
    
    @Resource(lookup="userSubsystemRequests") private Queue userSubsystemRequests;
    @Resource(lookup="userSubsystemResponses") private Queue userSubsystemResponses;
    
    @POST
    public Response create(
        @FormParam("ime") String name,
        @FormParam("opis") String description,
        @FormParam("cena") Integer price,
        @FormParam("popust") Integer discount,
        @FormParam("prodavac") String sellerUsername,
        @FormParam("kategorija") String categoryName
    ) throws JMSException {
        if (name == null || description == null || price == null || sellerUsername == null || categoryName == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        if (discount == null)
            discount = 0;
            
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(itemSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            int sellerId = UserUtils.getUserId(context, producer, userSubsystemRequests, userSubsystemResponses, sellerUsername);
            if (sellerId == 0)
                return ReturnUtils.returnBadRequest("Bad user");
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "item");
            requestMessage.setStringProperty("action", "create");
            
            requestMessage.setString("name", name);
            requestMessage.setString("description", description);
            requestMessage.setInt("price", price);
            requestMessage.setInt("discount", discount);
            requestMessage.setInt("sellerId", sellerId);
            requestMessage.setString("categoryName", categoryName);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @PUT
    @Path("{id}/cijena")
    public Response changePrice(
        @PathParam("id") Integer id,
        @FormParam("cijena") Integer price
    ) throws JMSException {
        if (id == null || price == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(itemSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "item");
            requestMessage.setStringProperty("action", "changePrice");
            
            requestMessage.setInt("id", id);
            requestMessage.setInt("price", price);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            TextMessage responseMessage = (TextMessage)consumer.receive();
            
            return Response
                .ok(responseMessage.getText())
                .build();
        }
    }
    
    @PUT
    @Path("{id}/popust")
    public Response changeDiscount(
        @PathParam("id") Integer id,
        @FormParam("popust") Integer discount
    ) throws JMSException {
        if (id == null || discount == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
         try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(itemSubsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "item");
            requestMessage.setStringProperty("action", "changeDiscount");
            
            requestMessage.setInt("id", id);
            requestMessage.setInt("discount", discount);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            TextMessage responseMessage = (TextMessage)consumer.receive();
            
            return Response
                .ok(responseMessage.getText())
                .build();
        }
    }
    
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAll(
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
            
            requestMessage.setStringProperty("resource", "item");
            requestMessage.setStringProperty("action", "getAll");
            
            requestMessage.setInt("sellerId", userId);
            
            producer.send(itemSubsystemRequests, requestMessage);
            
            return ReturnUtils.<Item>returnList(consumer);
        }
    }
}