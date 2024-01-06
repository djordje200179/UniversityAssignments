// generated with ast extension for cup
// version 0.8
// 6/0/2024 19:34:18


package rs.ac.bg.etf.pp1.ast;

public class InfiniteFor extends ForCondition {

    public InfiniteFor () {
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
        buffer.append("InfiniteFor(\n");

        buffer.append(tab);
        buffer.append(") [InfiniteFor]");
        return buffer.toString();
    }
}
