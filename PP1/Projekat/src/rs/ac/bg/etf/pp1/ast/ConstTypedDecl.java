// generated with ast extension for cup
// version 0.8
// 25/11/2023 10:26:33


package rs.ac.bg.etf.pp1.ast;

public class ConstTypedDecl implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private String type;
    private ConstDecls ConstDecls;

    public ConstTypedDecl (String type, ConstDecls ConstDecls) {
        this.type=type;
        this.ConstDecls=ConstDecls;
        if(ConstDecls!=null) ConstDecls.setParent(this);
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type=type;
    }

    public ConstDecls getConstDecls() {
        return ConstDecls;
    }

    public void setConstDecls(ConstDecls ConstDecls) {
        this.ConstDecls=ConstDecls;
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
        if(ConstDecls!=null) ConstDecls.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ConstDecls!=null) ConstDecls.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ConstDecls!=null) ConstDecls.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ConstTypedDecl(\n");

        buffer.append(" "+tab+type);
        buffer.append("\n");

        if(ConstDecls!=null)
            buffer.append(ConstDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ConstTypedDecl]");
        return buffer.toString();
    }
}
