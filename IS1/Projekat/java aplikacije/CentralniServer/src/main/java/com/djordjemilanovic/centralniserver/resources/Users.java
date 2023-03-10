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
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import podsistem1.entities.User;

@Path("korisnici")
public class Users {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="userSubsystemRequests") private Queue subsystemRequests;
    @Resource(lookup="userSubsystemResponses") private Queue subsystemResponses;
    
    @POST
    public Response create(
        @FormParam("korisnickoIme") String username,
        @FormParam("sifra") String password,
        @FormParam("ime") String firstName,
        @FormParam("prezime") String lastName,
        @FormParam("adresa") String address,
        @FormParam("grad") String cityName
    ) throws JMSException {
        if (username == null || password == null || firstName == null || lastName == null || address == null || cityName == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "user");
            requestMessage.setStringProperty("action", "create");
            
            requestMessage.setString("username", username);
            requestMessage.setString("password", password);
            requestMessage.setString("firstName", firstName);
            requestMessage.setString("lastName", lastName);
            requestMessage.setString("address", address);
            requestMessage.setString("cityName", cityName);
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @PUT
    @Path("{username}/novac")
    public Response addMoney(
        @PathParam("username") String username,
        @FormParam("dodatniNovac") Integer deltaMoney
    ) throws JMSException {
        if (username == null || deltaMoney == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "user");
            requestMessage.setStringProperty("action", "addMoney");
            
            requestMessage.setString("username", username);
            requestMessage.setInt("deltaMoney", deltaMoney);
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.returnText(consumer);
        }
    }
    
    @PUT
    @Path("{username}/adresa")
    public Response changeAddress(
        @PathParam("username") String username,
        @FormParam("adresa") String address,
        @FormParam("grad") String cityName
    ) throws JMSException {
        if (username == null || address == null || cityName == null)
            return ReturnUtils.returnBadRequest("No parameters");
        
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "user");
            requestMessage.setStringProperty("action", "changeAddress");
            
            requestMessage.setString("username", username);
            requestMessage.setString("address", address);
            requestMessage.setString("cityName", cityName);
            
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
            
            requestMessage.setStringProperty("resource", "user");
            requestMessage.setStringProperty("action", "getAll");
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.<User>returnList(consumer);
        }
    }
}