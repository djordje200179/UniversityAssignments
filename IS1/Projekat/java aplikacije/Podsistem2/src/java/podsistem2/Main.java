package podsistem2;

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
    
    @Resource(lookup="itemSubsystemRequests")
    private static Queue subsystemRequests;
    
    @Resource(lookup="itemSubsystemResponses")
    private static Queue subsystemResponses;
    
    public static void main(String[] args) throws JMSException {
        JMSContext context = connectionFactory.createContext();
        
        EntityManagerFactory emFactory = Persistence.createEntityManagerFactory("Podsistem2PU");
        try {
            EntityManager em = emFactory.createEntityManager();
            
            Responser responser = new Responser(em, context, subsystemRequests, subsystemResponses);
            responser.handle();
        } finally {
            emFactory.close();
        }
    }
}