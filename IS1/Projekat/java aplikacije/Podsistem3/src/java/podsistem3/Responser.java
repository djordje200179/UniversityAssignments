package podsistem3;

import java.io.Serializable;
import java.util.List;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Message;
import javax.jms.ObjectMessage;
import javax.jms.Queue;
import javax.jms.TextMessage;
import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import podsistem3.entities.Order1;
import podsistem3.entities.OrderItem;
import podsistem3.entities.Transaction1;

public class Responser {
    private final EntityManager entityManager;
    
    private final JMSContext context;
    private final Queue userRequestsQueue, userResponsesQueue;
    private final Queue itemRequestsQueue, itemResponsesQueue;
    private final Queue orderRequestsQueue, orderResponsesQueue;
    
    public Responser(
        EntityManager entityManager, JMSContext context, 
        Queue userRequestsQueue, Queue userResponsesQueue,
        Queue itemRequestsQueue, Queue itemResponsesQueue,
        Queue orderRequsetsQueue, Queue orderResponsesQueue
    ) {
        this.entityManager = entityManager;
        
        this.context = context;
        this.userRequestsQueue = userRequestsQueue;
        this.userResponsesQueue = userResponsesQueue;
        this.itemRequestsQueue = itemRequestsQueue;
        this.itemResponsesQueue = itemResponsesQueue;
        this.orderRequestsQueue = orderRequsetsQueue;
        this.orderResponsesQueue = orderResponsesQueue;
    }
    
    private Message handleOrderRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "create": {
            int userId = request.getInt("userId");
            String username = request.getString("username");
            
            Object[][] cartItems;
                
            try(JMSConsumer consumer = context.createConsumer(itemResponsesQueue)) {
                JMSProducer producer = context.createProducer();

                MapMessage requestMessage = context.createMapMessage();

                requestMessage.setStringProperty("resource", "cart");
                requestMessage.setStringProperty("action", "getPrepared");

                requestMessage.setInt("userId", userId);

                producer.send(itemRequestsQueue, requestMessage);

                ObjectMessage responseMessage = (ObjectMessage)consumer.receive();

                if(responseMessage.getObject() == null)
                    cartItems = null;
                else {
                    Object[][] response = (Object[][])responseMessage.getObject(); 
                    cartItems = response;
                }
            }

            if (cartItems == null)
                return context.createTextMessage("ERROR: No action");
            
            int totalPrice = 0;
            for (Object[] cartItem : cartItems) {
                int price = (Integer)cartItem[1];
                int quantity = (Integer)cartItem[2];

                totalPrice = price * quantity;
            }
            
            try(JMSConsumer consumer = context.createConsumer(userResponsesQueue)) {
                JMSProducer producer = context.createProducer();

                MapMessage requestMessage = context.createMapMessage();

                requestMessage.setStringProperty("resource", "user");
                requestMessage.setStringProperty("action", "removeMoney");

                requestMessage.setString("username", username);
                requestMessage.setInt("deltaMoney", totalPrice);

                producer.send(userRequestsQueue, requestMessage);
                TextMessage responseMessage = (TextMessage)consumer.receive();
                String responseText = responseMessage.getText();

                if(responseText.startsWith("ERROR:"))
                    return context.createTextMessage(responseText);
            }

            try(JMSConsumer consumer = context.createConsumer(itemResponsesQueue)) {
                JMSProducer producer = context.createProducer();

                MapMessage requestMessage = context.createMapMessage();

                requestMessage.setStringProperty("resource", "cart");
                requestMessage.setStringProperty("action", "empty");

                requestMessage.setInt("userId", userId);

                producer.send(itemRequestsQueue, requestMessage);
                consumer.receive();
            }
            
            String shipmentAddress;
            int shipmentCityId;

            try(JMSConsumer consumer = context.createConsumer(userResponsesQueue)) {
                JMSProducer producer = context.createProducer();

                MapMessage requestMessage = context.createMapMessage();

                requestMessage.setStringProperty("resource", "user");
                requestMessage.setStringProperty("action", "getShipmentInfo");

                requestMessage.setString("username", username);

                producer.send(userRequestsQueue, requestMessage);
                MapMessage responseMessage = (MapMessage)consumer.receive();

                shipmentAddress = responseMessage.getString("address");
                shipmentCityId = responseMessage.getInt("cityId");
            }
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                Order1 order = new Order1(
                    userId, shipmentAddress, shipmentCityId
                );
                
                entityManager.persist(order);
                entityManager.flush();
                
                for (Object[] cartItem : cartItems) {
                    int itemId = (Integer)cartItem[0];
                    int price = (Integer)cartItem[1];
                    int quantity = (Integer)cartItem[2];
                    
                    OrderItem orderItem = new OrderItem(
                        order.getId(),
                        itemId,
                        quantity,
                        price
                    );
                    
                    entityManager.persist(orderItem);
                }
            
                Transaction1 orderTransaction = new Transaction1(totalPrice, order);
                entityManager.persist(orderTransaction);
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "getAll": {
            List<Order1> orders = entityManager
                .createNamedQuery("Order1.getAll", Order1.class)
                .getResultList();

            orders.forEach(order -> {
                entityManager.refresh(order); // mozda?
                entityManager.detach(order);
                order.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)orders);
        }
        case "getAllByUserID": {
            List<Order1> orders = entityManager
                .createNamedQuery("Order1.getAllByUserId", Order1.class)
                .setParameter("userId", request.getInt("userId"))
                .getResultList();
            
            orders.forEach(order -> {
                entityManager.refresh(order); // mozda?
                entityManager.detach(order);
                order.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)orders);
        }
        default:
            return null;
        }
    }
    
    private Message handleTransactionRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "getAll": {
            List<Transaction1> transactions = entityManager
                .createNamedQuery("Transaction1.getAll", Transaction1.class)
                .getResultList();
            
            transactions.forEach(transaction -> {
                entityManager.refresh(transaction); // mozda?
                entityManager.detach(transaction);
                transaction.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)transactions);
        }
        default:
            return null;
        }
    }
    
    public void handle() throws JMSException {
        JMSConsumer consumer = context.createConsumer(orderRequestsQueue);
        JMSProducer producer = context.createProducer();
        
        while(true) {
            System.out.println("Waiting for request...");
            
            MapMessage requestMessage = (MapMessage)consumer.receive();
            
            String requestResource = requestMessage.getStringProperty("resource");
            String requestAction = requestMessage.getStringProperty("action");
            
            System.out.println("Got new message!");
            System.out.println("Resource: " + requestResource);
            System.out.println("Action: " + requestAction);            
            
            Message responseMessage;
            switch(requestResource) {
            case "order":
                responseMessage = handleOrderRequests(requestAction, requestMessage);
                break;
            case "transaction":
                responseMessage = handleTransactionRequests(requestAction, requestMessage);
                break;
            default:
                System.out.println("Invalid request with action: " + requestAction);
                continue;
            }
            
            System.out.println("Response: " + responseMessage);
            
            producer.send(orderResponsesQueue, responseMessage);
        }
    }    
}
