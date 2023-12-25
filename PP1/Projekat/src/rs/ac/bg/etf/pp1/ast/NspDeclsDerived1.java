// generated with ast extension for cup
// version 0.8
// 25/11/2023 10:26:33


package rs.ac.bg.etf.pp1.ast;

public class NspDeclsDerived1 extends NspDecls {

    private NspDecls NspDecls;
    private Namespace Namespace;

    public NspDeclsDerived1 (NspDecls NspDecls, Namespace Namespace) {
        this.NspDecls=NspDecls;
        if(NspDecls!=null) NspDecls.setParent(this);
        this.Namespace=Namespace;
        if(Namespace!=null) Namespace.setParent(this);
    }

    public NspDecls getNspDecls() {
        return NspDecls;
    }

    public void setNspDecls(NspDecls NspDecls) {
        this.NspDecls=NspDecls;
    }

    public Namespace getNamespace() {
        return Namespace;
    }

    public void setNamespace(Namespace Namespace) {
        this.Namespace=Namespace;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(NspDecls!=null) NspDecls.accept(visitor);
        if(Namespace!=null) Namespace.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(NspDecls!=null) NspDecls.traverseTopDown(visitor);
        if(Namespace!=null) Namespace.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(NspDecls!=null) NspDecls.traverseBottomUp(visitor);
        if(Namespace!=null) Namespace.traverseBottomUp(visitor);
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

        if(Namespace!=null)
            buffer.append(Namespace.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [NspDeclsDerived1]");
        return buffer.toString();
    }
}
