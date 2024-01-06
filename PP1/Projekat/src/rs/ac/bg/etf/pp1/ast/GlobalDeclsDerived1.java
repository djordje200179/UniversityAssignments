// generated with ast extension for cup
// version 0.8
// 6/0/2024 20:4:35


package rs.ac.bg.etf.pp1.ast;

public class GlobalDeclsDerived1 extends GlobalDecls {

    private GlobalDecls GlobalDecls;
    private GlobalDecl GlobalDecl;

    public GlobalDeclsDerived1 (GlobalDecls GlobalDecls, GlobalDecl GlobalDecl) {
        this.GlobalDecls=GlobalDecls;
        if(GlobalDecls!=null) GlobalDecls.setParent(this);
        this.GlobalDecl=GlobalDecl;
        if(GlobalDecl!=null) GlobalDecl.setParent(this);
    }

    public GlobalDecls getGlobalDecls() {
        return GlobalDecls;
    }

    public void setGlobalDecls(GlobalDecls GlobalDecls) {
        this.GlobalDecls=GlobalDecls;
    }

    public GlobalDecl getGlobalDecl() {
        return GlobalDecl;
    }

    public void setGlobalDecl(GlobalDecl GlobalDecl) {
        this.GlobalDecl=GlobalDecl;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(GlobalDecls!=null) GlobalDecls.accept(visitor);
        if(GlobalDecl!=null) GlobalDecl.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(GlobalDecls!=null) GlobalDecls.traverseTopDown(visitor);
        if(GlobalDecl!=null) GlobalDecl.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(GlobalDecls!=null) GlobalDecls.traverseBottomUp(visitor);
        if(GlobalDecl!=null) GlobalDecl.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("GlobalDeclsDerived1(\n");

        if(GlobalDecls!=null)
            buffer.append(GlobalDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(GlobalDecl!=null)
            buffer.append(GlobalDecl.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [GlobalDeclsDerived1]");
        return buffer.toString();
    }
}
