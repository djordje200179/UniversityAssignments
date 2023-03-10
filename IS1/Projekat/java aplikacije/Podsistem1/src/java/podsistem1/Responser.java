package podsistem1;

import java.io.Serializable;
import java.util.List;
import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Message;
import javax.jms.Queue;
import javax.persistence.NoResultException;
import javax.persistence.RollbackException;
import podsistem1.entities.City;
import podsistem1.entities.User;

public class Responser {    
    private final EntityManager entityManager;
    
    private final JMSContext context;
    private final Queue requestsQueue, responsesQueue;
    
    public Responser(EntityManager entityManager, JMSContext context, Queue requestsQueue, Queue responsesQueue) {
        this.entityManager = entityManager;
        
        this.context = context;
        this.requestsQueue = requestsQueue;
        this.responsesQueue = responsesQueue;
    }
    
    private Message handleUserRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "getAll": {
            List<User> users = entityManager
                .createNamedQuery("User.getAll", User.class)
                .getResultList();
            
            users.stream().forEach(user -> {
                entityManager.detach(user);
                user.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)users);
        }
        case "changeAddress": {
            User user;
            try {
                user = entityManager
                    .createNamedQuery("User.getByUsername", User.class)
                    .setParameter("username", request.getString("username"))
                    .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            City city;
            try {
                city = entityManager
                    .createNamedQuery("City.getByName", City.class)
                    .setParameter("name", request.getString("cityName"))
                    .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                                
                user.setAddress(request.getString("address"));
                user.setCity(city);
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "addMoney": {
            User user;
            try {
                user = entityManager
                    .createNamedQuery("User.getByUsername", User.class)
                    .setParameter("username", request.getString("username"))
                    .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                user.setMoney(user.getMoney() + request.getInt("deltaMoney"));
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "removeMoney": {
            User user;
            try {
                user = entityManager
                    .createNamedQuery("User.getByUsername", User.class)
                    .setParameter("username", request.getString("username"))
                    .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            if(user.getMoney() < request.getInt("deltaMoney"))
                return context.createTextMessage("ERROR: Invalid state");
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                user.setMoney(user.getMoney() - request.getInt("deltaMoney"));
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "getShipmentInfo": {
            User user;
            try {
                user = entityManager
                    .createNamedQuery("User.getByUsername", User.class)
                    .setParameter("username", request.getString("username"))
                    .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            MapMessage responseMessage = context.createMapMessage();
            
            responseMessage.setString("address", user.getAddress());
            responseMessage.setInt("cityId", user.getCity().getId());
            
            return responseMessage;
        }
        case "getId": {
            User user;
            try {
                user = entityManager
                    .createNamedQuery("User.getByUsername", User.class)
                    .setParameter("username", request.getString("username"))
                    .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            return context.createTextMessage(user.getId().toString());
        }
        case "create": {
            City city;
            try {
                city = entityManager
                .createNamedQuery("City.getByName", City.class)
                .setParameter("name", request.getString("cityName"))
                .getSingleResult();
            } catch (NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            User user = new User(
                request.getString("username"),
                request.getString("password"),
                request.getString("firstName"),
                request.getString("lastName"),
                request.getString("address"), 
                city
            );
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                entityManager.persist(user);
                
                transaction.commit();
            } catch(RollbackException e) {
                return context.createTextMessage("ERROR: Duplicate entity");
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        default:
            return null;
        }
    }
    
    private Message handleCityRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "getAll": {
            List<City> cities = entityManager
                .createNamedQuery("City.getAll", City.class)
                .getResultList();
            
            cities.stream().forEach(city -> {
                entityManager.detach(city);
                city.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)cities);
        }
        case "create": {
            City city = new City(request.getString("name"));
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                entityManager.persist(city);
                
                transaction.commit();
            } catch(RollbackException e) {
                return context.createTextMessage("ERROR: Duplicate entity");
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        default:
            return null;
        }
    }
    
    public void handle() throws JMSException {
        JMSConsumer consumer = context.createConsumer(requestsQueue);
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
            case "user":
                responseMessage = handleUserRequests(requestAction, requestMessage);
                break;
            case "city":
                responseMessage = handleCityRequests(requestAction, requestMessage);
                break;
            default:
                System.out.println("Invalid request with action: " + requestAction);
                continue;
            }
            
            System.out.println("Response: " + responseMessage);
            
            producer.send(responsesQueue, responseMessage);
        }
    }    
}