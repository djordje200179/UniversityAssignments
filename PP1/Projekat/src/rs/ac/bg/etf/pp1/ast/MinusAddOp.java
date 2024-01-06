// generated with ast extension for cup
// version 0.8
// 6/0/2024 23:1:17


package rs.ac.bg.etf.pp1.ast;

public class MinusAddOp extends Addop {

    public MinusAddOp () {
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
        buffer.append("MinusAddOp(\n");

        buffer.append(tab);
        buffer.append(") [MinusAddOp]");
        return buffer.toString();
    }
}
