// generated with ast extension for cup
// version 0.8
// 13/0/2024 13:23:46


package rs.ac.bg.etf.pp1.ast;

public class VarDeclOk extends VarDecl {

    private String name;
    private VarQuantity VarQuantity;

    public VarDeclOk (String name, VarQuantity VarQuantity) {
        this.name=name;
        this.VarQuantity=VarQuantity;
        if(VarQuantity!=null) VarQuantity.setParent(this);
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
        if(VarQuantity!=null) VarQuantity.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarQuantity!=null) VarQuantity.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarQuantity!=null) VarQuantity.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("VarDeclOk(\n");

        buffer.append(" "+tab+name);
        buffer.append("\n");

        if(VarQuantity!=null)
            buffer.append(VarQuantity.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [VarDeclOk]");
        return buffer.toString();
    }
}
