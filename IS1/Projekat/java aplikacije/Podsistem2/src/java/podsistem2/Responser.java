package podsistem2;

import java.io.Serializable;
import java.util.List;
import javax.jms.JMSConsumer;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;
import javax.jms.MapMessage;
import javax.jms.Message;
import javax.jms.Queue;
import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.persistence.NoResultException;
import javax.persistence.RollbackException;
import podsistem2.entities.Cart;
import podsistem2.entities.CartItem;
import podsistem2.entities.CartItemPK;
import podsistem2.entities.Category;
import podsistem2.entities.Item;

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
    
    private Message handleItemRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "getAll": {
            List<Item> items = entityManager
                    .createNamedQuery("Item.getBySellerId", Item.class)
                    .setParameter("sellerId", request.getInt("sellerId"))
                    .getResultList();
            
            items.stream().forEach(item -> {
                entityManager.detach(item);
                item.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)items);
        }
        case "changePrice": {
            Item item = entityManager.find(Item.class, request.getInt("id"));
            
            if(item == null)
                return context.createTextMessage("ERROR: No entity");
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                item.setPrice(request.getInt("price"));
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "changeDiscount": {
            Item item = entityManager.find(Item.class, request.getInt("id"));
            
            if(item == null)
                return context.createTextMessage("ERROR: No entity");
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                item.setDiscount(request.getInt("discount"));
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "create": {
            Category category;
            
            try {
                category = entityManager
                    .createNamedQuery("Category.getByName", Category.class)
                    .setParameter("name", request.getString("categoryName"))
                    .getSingleResult();
            } catch(NoResultException e) {
                return context.createTextMessage("ERROR: No entity");
            }
            
            Item item = new Item(
                request.getString("name"), 
                request.getString("description"), 
                request.getInt("price"), 
                request.getInt("discount"), 
                request.getInt("sellerId"), 
                category
            );
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                entityManager.persist(item);
                
                transaction.commit();
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
    
    private Message handleCategoryRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "getAll": {
            List<Category> categories = entityManager
                    .createNamedQuery("Category.getAll", Category.class)
                    .getResultList();
            
            categories.stream().forEach(category -> {
                entityManager.detach(category);
                category.clearReverseMappings();
            });
            
            return context.createObjectMessage((Serializable)categories);
        }
        case "create": {
            Category supercategory = null;
            if (request.itemExists("supercategoryName")) {
                try {
                    supercategory = entityManager
                        .createNamedQuery("Category.getByName", Category.class)
                        .setParameter("name", request.getString("supercategoryName"))
                        .getSingleResult();
                } catch (NoResultException e) {
                    return context.createTextMessage("ERROR: No entity");
                }
            }
            
            Category category = new Category(
                request.getString("name"), 
                supercategory
            );
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                entityManager.persist(category);
                
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
    
    private Message handleCartRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
        case "addItem": {
            Cart cart = entityManager.find(Cart.class, request.getInt("userId"));
            Item item = entityManager.find(Item.class, request.getInt("itemId"));
            
            if(item == null)
                return context.createTextMessage("ERROR: No entity");
                
            int quantity = request.getInt("quantity");
            
            CartItem cartItem = entityManager.find(CartItem.class, new CartItemPK(request.getInt("itemId"), request.getInt("userId")));
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                if (cart == null) {
                    cart = new Cart(request.getInt("userId"));
                    entityManager.persist(cart);
                }
                
                if(cartItem != null)
                    cartItem.setQuantity(cartItem.getQuantity() + quantity);
                else {
                    cartItem = new CartItem(
                        item.getId(),
                        cart.getUserId(),
                        quantity
                    );
                    entityManager.persist(cartItem);
                }

                int deltaPrice = item.getPrice() * quantity;
                cart.setPrice(cart.getPrice() + deltaPrice);
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "removeItem": {            
            CartItem cartItem = entityManager.find(CartItem.class, new CartItemPK(
                request.getInt("itemId"), 
                request.getInt("userId")
            ));
            
            if(cartItem == null)
                return context.createTextMessage("ERROR: No entity");
            
            Cart cart = entityManager.find(Cart.class, request.getInt("userId"));
            Item item = entityManager.find(Item.class, request.getInt("itemId"));
            
            if(item == null || cart == null)
                return context.createTextMessage("ERROR: No entity");
            
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();
                
                int deltaPrice = item.getPrice() * cartItem.getQuantity();
                cart.setPrice(cart.getPrice() - deltaPrice);
                
                entityManager.remove(cartItem);
                
                transaction.commit();
            } finally {
                if(transaction.isActive())
                    transaction.rollback();
            }
            
            return context.createTextMessage("SUCCESS");
        }
        case "get": {
            Cart cart = entityManager.find(Cart.class, request.getInt("userId"));      
            
            if (cart == null) {
                EntityTransaction transaction = entityManager.getTransaction();
                try {
                    transaction.begin();

                    cart = new Cart(request.getInt("userId"));
                    entityManager.persist(cart);

                    transaction.commit();
                } finally {
                    if(transaction.isActive())
                        transaction.rollback();
                }
            }
            entityManager.refresh(cart); // mozda?
            entityManager.detach(cart);
            cart.clearReverseMappings();
            
            return context.createObjectMessage(cart);
        }
        case "getPrepared": {
            Cart cart = entityManager.find(Cart.class, request.getInt("userId")); 
                
            entityManager.refresh(cart); // mozda? 
            
            if (cart == null)
                return context.createObjectMessage(null);
            
            Object[][] response = cart
                .getItems().stream()
                .map(cartItem -> new Object[]{
                    cartItem.getItem().getId(),
                    cartItem.getItem().getPrice() * (100 - cartItem.getItem().getDiscount()) / 100,
                    cartItem.getQuantity()
                })
                .toArray(length -> new Object[length][]);
            
            return context.createObjectMessage(response);
        }
        case "empty": {
            Cart cart = entityManager.find(Cart.class, request.getInt("userId"));      
            
            if (cart == null)
                return context.createTextMessage("SUCCESS");
            
            EntityTransaction transaction = entityManager.getTransaction();
            try {
                transaction.begin();

                cart.setPrice(0);

                entityManager
                    .createQuery("DELETE FROM CartItem ci WHERE ci.cartItemPK.userId = :userId")
                    .setParameter("userId", cart.getUserId())
                    .executeUpdate();
                
                entityManager.flush();

                transaction.commit();
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
    
    private Message handleReviewRequests(String action, MapMessage request) throws JMSException {
        switch(action) {
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
            case "item":
                responseMessage = handleItemRequests(requestAction, requestMessage);
                break;
            case "category":
                responseMessage = handleCategoryRequests(requestAction, requestMessage);
                break;
            case "review":
                responseMessage = handleReviewRequests(requestAction, requestMessage);
                break;
            case "cart":
                responseMessage = handleCartRequests(requestAction, requestMessage);
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
