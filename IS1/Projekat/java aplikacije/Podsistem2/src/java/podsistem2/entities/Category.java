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
@Table(name = "category")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Category.getAll", query = "SELECT c FROM Category c"),
    @NamedQuery(name = "Category.getByName", query = "SELECT c FROM Category c WHERE c.name = :name")
})
public class Category implements Serializable {
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
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "category")
    private List<Item> items;
    
    @OneToMany(mappedBy = "supercategory")
    private List<Category> subcategories;
    
    @JoinColumn(name = "supercategory_id", referencedColumnName = "id")
    @ManyToOne(cascade = CascadeType.DETACH)
    private Category supercategory;

    public Category() {}

    public Category(String name, Category supercategory) {
        this.name = name;
        this.supercategory = supercategory;
    }

    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    @XmlTransient
    @JsonbTransient
    public List<Item> getItems() { return items; }
    public void setItems(List<Item> items) { this.items = items; }

    @XmlTransient
    @JsonbTransient
    public List<Category> getSubcategories() { return subcategories; }
    public void setSubcategories(List<Category> subcategories) { this.subcategories = subcategories; }

    public Category getSupercategory() { return supercategory; }
    public void setSupercategory(Category supercategory) { this.supercategory = supercategory; }

    @Override
    public int hashCode() { return id != null ? id.hashCode() : 0; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Category))
            return false;
            
        Category other = (Category) obj;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id)))
            return false;
        
        return true;
    }

    @Override
    public String toString() {
        return "podsistem2.entities.Category[ id=" + id + " ]";
    }
    
    public void clearReverseMappings() {
        subcategories = null;
        items = null;
        
        if(supercategory != null)
            supercategory.clearReverseMappings();
    }
}