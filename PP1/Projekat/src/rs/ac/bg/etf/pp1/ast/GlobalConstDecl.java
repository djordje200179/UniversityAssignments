// generated with ast extension for cup
// version 0.8
// 25/11/2023 17:27:15


package rs.ac.bg.etf.pp1.ast;

public class GlobalConstDecl extends GlobalDecl {

    private ConstTypedDecl ConstTypedDecl;

    public GlobalConstDecl (ConstTypedDecl ConstTypedDecl) {
        this.ConstTypedDecl=ConstTypedDecl;
        if(ConstTypedDecl!=null) ConstTypedDecl.setParent(this);
    }

    public ConstTypedDecl getConstTypedDecl() {
        return ConstTypedDecl;
    }

    public void setConstTypedDecl(ConstTypedDecl ConstTypedDecl) {
        this.ConstTypedDecl=ConstTypedDecl;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(ConstTypedDecl!=null) ConstTypedDecl.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ConstTypedDecl!=null) ConstTypedDecl.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ConstTypedDecl!=null) ConstTypedDecl.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("GlobalConstDecl(\n");

        if(ConstTypedDecl!=null)
            buffer.append(ConstTypedDecl.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [GlobalConstDecl]");
        return buffer.toString();
    }
}
