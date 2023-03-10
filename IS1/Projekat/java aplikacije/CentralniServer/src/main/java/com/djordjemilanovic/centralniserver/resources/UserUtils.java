package com.djordjemilanovic.centralniserver.resources;

import java.nio.charset.StandardCharsets;
import java.util.Base64;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Queue;
import javax.jms.TextMessage;

class UserUtils {    
    static String getUsername(String authString) {
        String rawString = authString.substring("Basic".length()).trim();
        String decodedString = new String(Base64.getDecoder().decode(rawString), StandardCharsets.UTF_8);
        String[] credentials = decodedString.split(":", 2);
        
        String username = credentials[0];
        String password = credentials[1];
        
        return username;
    }
    
    static int getUserId(JMSContext context, JMSProducer producer, Queue subsystemRequests, Queue subsystemResponses, String username) throws JMSException {
        try(JMSConsumer consumer = context.createConsumer(subsystemResponses)) {            
            MapMessage requestMessage = context.createMapMessage();
            
            requestMessage.setStringProperty("resource", "user");
            requestMessage.setStringProperty("action", "getId");
            
            requestMessage.setString("username", username);
            
            producer.send(subsystemRequests, requestMessage);
            
            TextMessage message = (TextMessage)consumer.receive();
            String text = message.getText();
            if(text.startsWith("ERROR: "))
                return 0;
            
            int value = Integer.parseInt(text);
            return value;
        }
    }
}
