// generated with ast extension for cup
// version 0.8
// 13/0/2024 13:23:46


package rs.ac.bg.etf.pp1.ast;

public class CondOrJoiner implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    public CondOrJoiner () {
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
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("CondOrJoiner(\n");

        buffer.append(tab);
        buffer.append(") [CondOrJoiner]");
        return buffer.toString();
    }
}