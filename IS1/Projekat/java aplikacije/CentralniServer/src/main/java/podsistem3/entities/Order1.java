package podsistem3.entities;

import java.io.Serializable;
import java.util.Date;
import java.util.List;
import javax.json.bind.annotation.JsonbTransient;
import javax.persistence.Basic;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

@Entity
@Table(name = "order1")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Order1.getAll", query = "SELECT o FROM Order1 o"),
    @NamedQuery(name = "Order1.getAllByUserId", query = "SELECT o FROM Order1 o WHERE o.userId = :userId")
})
public class Order1 implements Serializable {
    private static final long serialVersionUID = 1L;
    
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Basic(optional = false)
    @Column(name = "id")
    private Integer id;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "user_id")
    private int userId;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "time_stamp")
    //@Temporal(TemporalType.TIMESTAMP)
    //private Date timestamp;
    private String timestamp;
    
    @Basic(optional = false)
    @NotNull
    @Size(min = 1, max = 45)
    @Column(name = "address")
    private String address;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "city_id")
    private int cityId;
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "order")
    private List<OrderItem> items;
    
    @OneToOne(cascade = CascadeType.ALL, mappedBy = "order")
    private Transaction1 transaction;

    public Order1() {}

    public Order1(int userId, String address, int cityId) {
        this.userId = userId;
        this.address = address;
        this.cityId = cityId;
        
        this.timestamp = new Date().toString();
    }

    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }
    
    public String getTimestamp() { return timestamp; }
    public void setTimestamp(Date timestamp) { this.timestamp = timestamp.toString(); }

    public String getAddress() { return address; }
    public void setAddress(String address) { this.address = address; }

    public int getUserId() { return userId; }
    public void setUserId(int userId) { this.userId = userId; }

    public int getCityId() { return cityId; }
    public void setCityId(int cityId) { this.cityId = cityId; }

    public List<OrderItem> getItems() { return items; }
    public void setItems(List<OrderItem> items) { this.items = items; }

    @XmlTransient
    @JsonbTransient
    public Transaction1 getTransaction() { return transaction; }
    public void setTransaction(Transaction1 transaction) { this.transaction = transaction; }

    @Override
    public int hashCode() { return id != null ? id.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Order1))
            return false;
        
        Order1 other = (Order1) obj;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem3.entities.Order[ id=" + id + " ]";
    }
    
    public void clearReverseMappings() {
        transaction = null;
        
        items.forEach(OrderItem::clearReverseMappings);
    }
}