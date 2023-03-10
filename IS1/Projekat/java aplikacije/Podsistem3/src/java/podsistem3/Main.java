package podsistem3;

import javax.persistence.EntityManagerFactory;
import javax.persistence.EntityManager;
import javax.persistence.Persistence;
import javax.annotation.Resource;
import javax.jms.ConnectionFactory;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.Queue;

public class Main {
    @Resource(lookup="jms/__defaultConnectionFactory")
    private static ConnectionFactory connectionFactory;
    
    @Resource(lookup="userSubsystemRequests") private static Queue userSubsystemRequests;
    @Resource(lookup="userSubsystemResponses") private static Queue userSubsystemResponses;
    
    @Resource(lookup="itemSubsystemRequests") private static Queue itemSubsystemRequests;
    @Resource(lookup="itemSubsystemResponses") private static Queue itemSubsystemResponses;
    
    @Resource(lookup="orderSubsystemRequests") private static Queue orderSubsystemRequests;
    @Resource(lookup="orderSubsystemResponses") private static Queue orderSubsystemResponses;
    
    public static void main(String[] args) throws JMSException {
        JMSContext context = connectionFactory.createContext();
        
        EntityManagerFactory emFactory = Persistence.createEntityManagerFactory("Podsistem3PU");
        try {
            EntityManager em = emFactory.createEntityManager();
            
            Responser responser = new Responser(
                em, context, 
                userSubsystemRequests, userSubsystemResponses,
                itemSubsystemRequests, itemSubsystemResponses,
                orderSubsystemRequests, orderSubsystemResponses
            );
            
            responser.handle();
        } finally {
            emFactory.close();
        }
    }
}