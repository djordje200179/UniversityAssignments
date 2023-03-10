package podsistem3.entities;

import java.io.Serializable;
import java.util.Date;
import javax.persistence.Basic;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;

@Entity
@Table(name = "transaction1")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Transaction1.getAll", query = "SELECT t FROM Transaction1 t")
})
public class Transaction1 implements Serializable {
    private static final long serialVersionUID = 1L;
    
    @Id
    @Basic(optional = false)
    @NotNull
    @Column(name = "order_id")
    private Integer orderId;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "price")
    private int price;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "time_stamp")
    //@Temporal(TemporalType.TIMESTAMP)
    //private Date timestamp;
    private String timestamp;
    
    @JoinColumn(name = "order_id", referencedColumnName = "id", insertable = false, updatable = false)
    @OneToOne(cascade = CascadeType.DETACH, optional = false)
    private Order1 order;

    public Transaction1() { }

    public Transaction1(int price, Order1 order) {
        this.orderId = order.getId();
        this.price = price;
        this.order = order;
        
        timestamp = new Date().toString();
    }

    public Integer getOrderId() { return orderId; }
    public void setOrderId(Integer orderId) { this.orderId = orderId; }

    public int getPrice() { return price; }
    public void setPrice(int price) { this.price = price; }

    public String getTimestamp() { return timestamp; }
    public void setTimestamp(Date timestamp) { this.timestamp = timestamp.toString(); }

    public Order1 getOrder() { return order; }
    public void setOrder(Order1 order) { this.order = order; }

    @Override
    public int hashCode() { return orderId != null ? orderId.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Transaction1))
            return false;
        
        Transaction1 other = (Transaction1) obj;
        if ((this.orderId == null && other.orderId != null) || (this.orderId != null && !this.orderId.equals(other.orderId)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem3.entities.Transaction[ id=" + orderId + " ]";
    }

    public void clearReverseMappings() {
        order.clearReverseMappings();
    }
}