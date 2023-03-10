package podsistem2.entities;

import java.io.Serializable;
import java.util.List;
import javax.persistence.Basic;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;

@Entity
@Table(name = "cart")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Cart.getAll", query = "SELECT c FROM Cart c")
})
public class Cart implements Serializable {
    private static final long serialVersionUID = 1L;
    
    @Id
    @Basic(optional = false)
    @NotNull
    @Column(name = "user_id")
    private Integer userId;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "price")
    private int price;
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "cart")
    private List<CartItem> items;

    public Cart() { }
    
    public Cart(Integer userId) {
        this.userId = userId;
    }

    public Integer getUserId() { return userId; }
    public void setUserId(Integer userId) { this.userId = userId; }

    public int getPrice() { return price; }
    public void setPrice(int price) { this.price = price; }

    public List<CartItem> getItems() { return items; }
    public void setItems(List<CartItem> items) { this.items = items; }

    @Override
    public int hashCode() { return userId != null ? userId.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Cart))
            return false;
        
        Cart other = (Cart) obj;
        if ((this.userId == null && other.userId != null) || (this.userId != null && !this.userId.equals(other.userId)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem2.entities.Cart[ id=" + userId + " ]";
    }
    
    public void clearReverseMappings() {
        items.forEach(CartItem::clearReverseMappings);
    }
}