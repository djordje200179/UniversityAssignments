package podsistem3.entities;

import java.io.Serializable;
import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Embeddable;
import javax.validation.constraints.NotNull;

@Embeddable
public class OrderItemPK implements Serializable {
    @Basic(optional = false)
    @NotNull
    @Column(name = "order_id")
    private int orderId;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "item_id")
    private int itemId;

    public OrderItemPK() {}

    public OrderItemPK(int orderId, int itemId) {
        this.orderId = orderId;
        this.itemId = itemId;
    }

    public int getOrderId() { return orderId; }
    public void setOrderId(int orderId) { this.orderId = orderId; }

    public int getItemId() { return itemId; }
    public void setItemId(int itemId) { this.itemId = itemId; }

    @Override
    public int hashCode() { return orderId + itemId; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof OrderItemPK))
            return false;
        
        OrderItemPK other = (OrderItemPK) obj;
        if (this.orderId != other.orderId || this.itemId != other.itemId)
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem3.entities.OrderItemPK[ orderId=" + orderId + ", itemId=" + itemId + " ]";
    }
}