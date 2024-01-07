// generated with ast extension for cup
// version 0.8
// 7/0/2024 17:35:6


package rs.ac.bg.etf.pp1.ast;

public class FormParamOk extends FormParam {

    private Type Type;
    private String name;
    private VarQuantity VarQuantity;

    public FormParamOk (Type Type, String name, VarQuantity VarQuantity) {
        this.Type=Type;
        if(Type!=null) Type.setParent(this);
        this.name=name;
        this.VarQuantity=VarQuantity;
        if(VarQuantity!=null) VarQuantity.setParent(this);
    }

    public Type getType() {
        return Type;
    }

    public void setType(Type Type) {
        this.Type=Type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name=name;
    }

    public VarQuantity getVarQuantity() {
        return VarQuantity;
    }

    public void setVarQuantity(VarQuantity VarQuantity) {
        this.VarQuantity=VarQuantity;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(Type!=null) Type.accept(visitor);
        if(VarQuantity!=null) VarQuantity.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(Type!=null) Type.traverseTopDown(visitor);
        if(VarQuantity!=null) VarQuantity.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(Type!=null) Type.traverseBottomUp(visitor);
        if(VarQuantity!=null) VarQuantity.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("FormParamOk(\n");

        if(Type!=null)
            buffer.append(Type.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(" "+tab+name);
        buffer.append("\n");

        if(VarQuantity!=null)
            buffer.append(VarQuantity.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [FormParamOk]");
        return buffer.toString();
    }
}
