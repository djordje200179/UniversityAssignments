// generated with ast extension for cup
// version 0.8
// 7/0/2024 17:35:6


package rs.ac.bg.etf.pp1.ast;

public class GlobalVarDecl extends GlobalDecl {

    private VarTypedDecl VarTypedDecl;

    public GlobalVarDecl (VarTypedDecl VarTypedDecl) {
        this.VarTypedDecl=VarTypedDecl;
        if(VarTypedDecl!=null) VarTypedDecl.setParent(this);
    }

    public VarTypedDecl getVarTypedDecl() {
        return VarTypedDecl;
    }

    public void setVarTypedDecl(VarTypedDecl VarTypedDecl) {
        this.VarTypedDecl=VarTypedDecl;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(VarTypedDecl!=null) VarTypedDecl.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarTypedDecl!=null) VarTypedDecl.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarTypedDecl!=null) VarTypedDecl.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("GlobalVarDecl(\n");

        if(VarTypedDecl!=null)
            buffer.append(VarTypedDecl.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [GlobalVarDecl]");
        return buffer.toString();
    }
}
