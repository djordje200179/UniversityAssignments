// generated with ast extension for cup
// version 0.8
// 13/0/2024 13:23:46


package rs.ac.bg.etf.pp1.ast;

public class VarTypedDecl implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private Type Type;
    private VarDecls VarDecls;

    public VarTypedDecl (Type Type, VarDecls VarDecls) {
        this.Type=Type;
        if(Type!=null) Type.setParent(this);
        this.VarDecls=VarDecls;
        if(VarDecls!=null) VarDecls.setParent(this);
    }

    public Type getType() {
        return Type;
    }

    public void setType(Type Type) {
        this.Type=Type;
    }

    public VarDecls getVarDecls() {
        return VarDecls;
    }

    public void setVarDecls(VarDecls VarDecls) {
        this.VarDecls=VarDecls;
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
        if(Type!=null) Type.accept(visitor);
        if(VarDecls!=null) VarDecls.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(Type!=null) Type.traverseTopDown(visitor);
        if(VarDecls!=null) VarDecls.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(Type!=null) Type.traverseBottomUp(visitor);
        if(VarDecls!=null) VarDecls.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("VarTypedDecl(\n");

        if(Type!=null)
            buffer.append(Type.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(VarDecls!=null)
            buffer.append(VarDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [VarTypedDecl]");
        return buffer.toString();
    }
}