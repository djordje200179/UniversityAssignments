package podsistem1.entities;

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
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

@Entity
@XmlRootElement
@Table(name = "city")
@NamedQueries({
    @NamedQuery(name = "City.getAll", query = "SELECT c FROM City c"),
    @NamedQuery(name = "City.getByName", query = "SELECT c FROM City c WHERE c.name = :name")
})
public class City implements Serializable {
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
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "city")
    private List<User> users;

    public City() {}

    public City(String name) { 
        this.name = name;
    }

    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    @XmlTransient
    @JsonbTransient
    public List<User> getUsers() { return users;}
    public void setUsers(List<User> users) { this.users = users; }

    @Override
    public int hashCode() { return id != null ? id.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof City))
            return false;
        
        City other = (City) obj;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem1.entities.City[ id=" + id + " ]";
    }    
    
    public void clearReverseMappings() {
        users = null;
    }
}