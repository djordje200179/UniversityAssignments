// generated with ast extension for cup
// version 0.8
// 7/0/2024 20:31:52


package rs.ac.bg.etf.pp1.ast;

public class NspDeclsDerived1 extends NspDecls {

    private NspDecls NspDecls;
    private NspDecl NspDecl;

    public NspDeclsDerived1 (NspDecls NspDecls, NspDecl NspDecl) {
        this.NspDecls=NspDecls;
        if(NspDecls!=null) NspDecls.setParent(this);
        this.NspDecl=NspDecl;
        if(NspDecl!=null) NspDecl.setParent(this);
    }

    public NspDecls getNspDecls() {
        return NspDecls;
    }

    public void setNspDecls(NspDecls NspDecls) {
        this.NspDecls=NspDecls;
    }

    public NspDecl getNspDecl() {
        return NspDecl;
    }

    public void setNspDecl(NspDecl NspDecl) {
        this.NspDecl=NspDecl;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(NspDecls!=null) NspDecls.accept(visitor);
        if(NspDecl!=null) NspDecl.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(NspDecls!=null) NspDecls.traverseTopDown(visitor);
        if(NspDecl!=null) NspDecl.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(NspDecls!=null) NspDecls.traverseBottomUp(visitor);
        if(NspDecl!=null) NspDecl.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("NspDeclsDerived1(\n");

        if(NspDecls!=null)
            buffer.append(NspDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(NspDecl!=null)
            buffer.append(NspDecl.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [NspDeclsDerived1]");
        return buffer.toString();
    }
}
