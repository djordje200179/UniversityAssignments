// generated with ast extension for cup
// version 0.8
// 25/11/2023 23:15:59


package rs.ac.bg.etf.pp1.ast;

public class Designator implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    public rs.etf.pp1.symboltable.concepts.Obj obj = null;

    private VarRef VarRef;
    private ElemAccess ElemAccess;

    public Designator (VarRef VarRef, ElemAccess ElemAccess) {
        this.VarRef=VarRef;
        if(VarRef!=null) VarRef.setParent(this);
        this.ElemAccess=ElemAccess;
        if(ElemAccess!=null) ElemAccess.setParent(this);
    }

    public VarRef getVarRef() {
        return VarRef;
    }

    public void setVarRef(VarRef VarRef) {
        this.VarRef=VarRef;
    }

    public ElemAccess getElemAccess() {
        return ElemAccess;
    }

    public void setElemAccess(ElemAccess ElemAccess) {
        this.ElemAccess=ElemAccess;
    }

    public SyntaxNode getParent() {
        return parent;
    }

    public void setParent(SyntaxNode parent) {
        this.parent=parent;
    }

    public int getLine() {
        return line;
    }

    public void setLine(int line) {
        this.line=line;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(VarRef!=null) VarRef.accept(visitor);
        if(ElemAccess!=null) ElemAccess.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarRef!=null) VarRef.traverseTopDown(visitor);
        if(ElemAccess!=null) ElemAccess.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarRef!=null) VarRef.traverseBottomUp(visitor);
        if(ElemAccess!=null) ElemAccess.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("Designator(\n");

        if(VarRef!=null)
            buffer.append(VarRef.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ElemAccess!=null)
            buffer.append(ElemAccess.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [Designator]");
        return buffer.toString();
    }
}
