// generated with ast extension for cup
// version 0.8
// 5/0/2024 12:37:54


package rs.ac.bg.etf.pp1.ast;

public class PlusAddOp extends Addop {

    public PlusAddOp () {
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
        buffer.append("PlusAddOp(\n");

        buffer.append(tab);
        buffer.append(") [PlusAddOp]");
        return buffer.toString();
    }
}
