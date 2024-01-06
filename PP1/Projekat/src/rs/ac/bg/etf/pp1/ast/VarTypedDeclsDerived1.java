// generated with ast extension for cup
// version 0.8
// 6/0/2024 20:4:36


package rs.ac.bg.etf.pp1.ast;

public class VarTypedDeclsDerived1 extends VarTypedDecls {

    private VarTypedDecls VarTypedDecls;
    private VarTypedDecl VarTypedDecl;

    public VarTypedDeclsDerived1 (VarTypedDecls VarTypedDecls, VarTypedDecl VarTypedDecl) {
        this.VarTypedDecls=VarTypedDecls;
        if(VarTypedDecls!=null) VarTypedDecls.setParent(this);
        this.VarTypedDecl=VarTypedDecl;
        if(VarTypedDecl!=null) VarTypedDecl.setParent(this);
    }

    public VarTypedDecls getVarTypedDecls() {
        return VarTypedDecls;
    }

    public void setVarTypedDecls(VarTypedDecls VarTypedDecls) {
        this.VarTypedDecls=VarTypedDecls;
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
        if(VarTypedDecls!=null) VarTypedDecls.accept(visitor);
        if(VarTypedDecl!=null) VarTypedDecl.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarTypedDecls!=null) VarTypedDecls.traverseTopDown(visitor);
        if(VarTypedDecl!=null) VarTypedDecl.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarTypedDecls!=null) VarTypedDecls.traverseBottomUp(visitor);
        if(VarTypedDecl!=null) VarTypedDecl.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("VarTypedDeclsDerived1(\n");

        if(VarTypedDecls!=null)
            buffer.append(VarTypedDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(VarTypedDecl!=null)
            buffer.append(VarTypedDecl.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [VarTypedDeclsDerived1]");
        return buffer.toString();
    }
}
