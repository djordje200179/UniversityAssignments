package podsistem2.entities;

import java.io.Serializable;
import java.util.List;
import javax.json.bind.annotation.JsonbTransient;
import javax.persistence.Basic;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

@Entity
@Table(name = "item")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Item.getAll", query = "SELECT i FROM Item i"),
    @NamedQuery(name = "Item.getBySellerId", query = "SELECT i FROM Item i WHERE i.sellerId = :sellerId")
})
public class Item implements Serializable {
    private static final long serialVersionUID = 1L;

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "id")
    private Integer id;
    
    @Basic(optional = false)
    @NotNull
    @Size(min = 1, max = 45)
    @Column(name = "name")
    private String name;
    
    @Basic(optional = false)
    @NotNull
    @Size(min = 1, max = 200)
    @Column(name = "description")
    private String description;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "price")
    private int price;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "discount")
    private int discount;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "seller_id")
    private int sellerId;
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "item")
    private List<Review> reviews;
    
    @JoinColumn(name = "category_id", referencedColumnName = "id")
    @ManyToOne(cascade = CascadeType.DETACH, optional = false)
    private Category category;

    public Item() {}

    public Item(String name, String description, int price, int discount, int sellerId, Category category) {
        this.name = name;
        this.description = description;
        this.price = price;
        this.discount = discount;
        this.sellerId = sellerId;
        this.category = category;
    }

    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public String getDescription() { return description; }
    public void setDescription(String description) { this.description = description; }

    public int getPrice() { return price; }
    public void setPrice(int price) { this.price = price; }
    
    public int getDiscount() { return discount; }
    public void setDiscount(int discount) { this.discount = discount; }
    
    public int getSellerId() { return sellerId; }
    public void setSellerId(int sellerId) { this.sellerId = sellerId; }

    public Category getCategory() { return category; }
    public void setCategory(Category category) { this.category = category; }

    @XmlTransient
    @JsonbTransient
    public List<Review> getReviews() { return reviews; }
    public void setReviews(List<Review> reviews) { this.reviews = reviews; }
    
    @Override
    public int hashCode() { return id != null ? id.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Item))
            return false;
            
        Item other = (Item) obj;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem2.entities.Item[ id=" + id + " ]";
    }
    
    public void clearReverseMappings() {
        reviews = null;
        
        category.clearReverseMappings();
    }
}