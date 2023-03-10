package com.djordjemilanovic.centralniserver.resources;

import java.util.List;
import javax.jms.JMSConsumer;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.ObjectMessage;
import javax.jms.TextMessage;
import javax.ws.rs.core.GenericEntity;
import javax.ws.rs.core.Response;

class ReturnUtils {    
    static <T> Response returnEntity(JMSConsumer consumer) throws JMSException {
        Message message;
        while (!((message = consumer.receive()) instanceof ObjectMessage));
        ObjectMessage objectMessage = (ObjectMessage)message;
        T entity = (T)objectMessage.getObject();
        
        return Response.ok(entity).build();
    }
    
    static <T> Response returnList(JMSConsumer consumer) throws JMSException {
        Message message;
        while (!((message = consumer.receive()) instanceof ObjectMessage));
        ObjectMessage objectMessage = (ObjectMessage)message;
        List<T> list = (List<T>)objectMessage.getObject();
        GenericEntity<List<T>> entities = new GenericEntity<List<T>>(list){};
        
        return Response.ok(entities).build();
    }
    
    static <T> Response returnText(JMSConsumer consumer) throws JMSException {
        Message message;
        while (!((message = consumer.receive()) instanceof TextMessage));
        TextMessage textMessage = (TextMessage)message;
        String text = textMessage.getText();
        
        return Response.ok(text).build();
    }
    
    static Response returnBadRequest(String text) {
        String fullText = text.startsWith("ERROR:") ? text : "ERROR: " + text;
        
        return Response
                .status(Response.Status.BAD_REQUEST)
                .entity(fullText)
                .build();
    }
}
