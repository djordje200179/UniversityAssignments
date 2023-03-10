package podsistem2.entities;

import java.io.Serializable;
import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Embeddable;
import javax.validation.constraints.NotNull;

@Embeddable
public class CartItemPK implements Serializable {
    @Basic(optional = false)
    @NotNull
    @Column(name = "item_id")
    private int itemId;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "user_id")
    private int userId;

    public CartItemPK() { }

    public CartItemPK(int itemId, int userId) {
        this.itemId = itemId;
        this.userId = userId;
    }

    public int getItemId() { return itemId; }
    public void setItemId(int itemId) { this.itemId = itemId; }

    public int getUserId() { return userId; }
    public void setUserId(int userId) { this.userId = userId; }

    @Override
    public int hashCode() { return itemId + userId; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof CartItemPK))
            return false;
        
        CartItemPK other = (CartItemPK) obj;
        if (this.itemId != other.itemId || this.userId != other.userId)
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem2.entities.CartItemPK[ itemId=" + itemId + ", userId=" + userId + " ]";
    } 
}