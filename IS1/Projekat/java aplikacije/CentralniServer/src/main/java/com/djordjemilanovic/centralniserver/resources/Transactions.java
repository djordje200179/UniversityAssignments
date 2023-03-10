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
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import podsistem3.entities.Transaction1;

@Path("transakcije")
public class Transactions {
    @Resource(lookup="jms/__defaultConnectionFactory") private ConnectionFactory connectionFactory;
    
    @Resource(lookup="orderSubsystemRequests") private Queue subsystemRequests;
    @Resource(lookup="orderSubsystemResponses") private Queue subsystemResponses;
    
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAll() throws JMSException {
        try(JMSContext context = connectionFactory.createContext();
            JMSConsumer consumer = context.createConsumer(subsystemResponses)) {
            JMSProducer producer = context.createProducer();
            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "transaction");
            requestMessage.setStringProperty("action", "getAll");
            
            producer.send(subsystemRequests, requestMessage);
            
            return ReturnUtils.<Transaction1>returnList(consumer);
        }
    }
}