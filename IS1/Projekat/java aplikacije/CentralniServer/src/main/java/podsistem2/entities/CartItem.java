package podsistem2.entities;

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
@Table(name = "cart_item")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "CartItem.getAll", query = "SELECT c FROM CartItem c")
})
public class CartItem implements Serializable {
    private static final long serialVersionUID = 1L;
    
    @EmbeddedId
    protected CartItemPK cartItemPK;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "quantity")
    private int quantity;
    
    @JoinColumn(name = "user_id", referencedColumnName = "user_id", insertable = false, updatable = false)
    @ManyToOne(optional = false)
    private Cart cart;
    
    @JoinColumn(name = "item_id", referencedColumnName = "id", insertable = false, updatable = false)
    @ManyToOne(optional = false)
    private Item item;

    public CartItem() {}

    public CartItem(int itemId, int userId, int quantity) {
        this.cartItemPK = new CartItemPK(itemId, userId);
        this.quantity = quantity;
    }

    public int getQuantity() { return quantity; }
    public void setQuantity(int quantity) { this.quantity = quantity; }

    @XmlTransient
    @JsonbTransient
    public Cart getCart() { return cart; }
    public void setCart(Cart cart) { this.cart = cart; }

    public Item getItem() { return item; }
    public void setItem(Item item) { this.item = item; }

    @Override
    public int hashCode() { return cartItemPK != null ? cartItemPK.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof CartItem))
            return false;
        
        CartItem other = (CartItem) obj;
        if ((this.cartItemPK == null && other.cartItemPK != null) || (this.cartItemPK != null && !this.cartItemPK.equals(other.cartItemPK)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem2.entities.CartItem[ cartItemPK=" + cartItemPK + " ]";
    }
    
    public void clearReverseMappings() {
        cart = null;
        
        item.clearReverseMappings();
    }    
}
