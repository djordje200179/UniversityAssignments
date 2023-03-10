package podsistem3.entities;

import java.io.Serializable;
import javax.json.bind.annotation.JsonbTransient;
import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.EmbeddedId;
import javax.persistence.Entity;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.Table;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

@Entity
@Table(name = "order_item")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "OrderItem.getAll", query = "SELECT o FROM OrderItem o")
})
public class OrderItem implements Serializable {
    private static final long serialVersionUID = 1L;
    
    @EmbeddedId
    protected OrderItemPK orderItemPK;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "quantity")
    private int quantity;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "price")
    private int price;
    
    @JoinColumn(name = "order_id", referencedColumnName = "id", insertable = false, updatable = false)
    @ManyToOne(optional = false)
    private Order1 order;

    public OrderItem() {}

    public OrderItem(int orderId, int itemId, int quantity, int price) {
        this.orderItemPK = new OrderItemPK(orderId, itemId);
        this.quantity = quantity;
        this.price = price;
    }

    public OrderItemPK getOrderItemPK() { return orderItemPK; }
    public void setOrderItemPK(OrderItemPK orderItemPK) { this.orderItemPK = orderItemPK; }

    public int getQuantity() { return quantity; }
    public void setQuantity(int quantity) { this.quantity = quantity; }
    
    public int getPrice() { return price; }
    public void setPrice(int price) { this.price = price; }

    @XmlTransient
    @JsonbTransient
    public Order1 getOrder() { return order; }
    public void setOrder(Order1 order) { this.order = order; }

    @Override
    public int hashCode() { return orderItemPK != null ? orderItemPK.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof OrderItem))
            return false;
        
        OrderItem other = (OrderItem) obj;
        if ((this.orderItemPK == null && other.orderItemPK != null) || (this.orderItemPK != null && !this.orderItemPK.equals(other.orderItemPK)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem3.entities.OrderItem[ orderItemPK=" + orderItemPK + " ]";
    }
    
    public void clearReverseMappings() {
        order = null;
    }
}