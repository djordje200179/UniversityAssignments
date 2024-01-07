// generated with ast extension for cup
// version 0.8
// 7/0/2024 17:35:6


package rs.ac.bg.etf.pp1.ast;

public class EqualityRelop extends Relop {

    public EqualityRelop () {
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
        buffer.append("EqualityRelop(\n");

        buffer.append(tab);
        buffer.append(") [EqualityRelop]");
        return buffer.toString();
    }
}
