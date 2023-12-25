// generated with ast extension for cup
// version 0.8
// 25/11/2023 17:27:15


package rs.ac.bg.etf.pp1.ast;

public class VoidReturnStmt extends Statement {

    public VoidReturnStmt () {
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
        buffer.append("VoidReturnStmt(\n");

        buffer.append(tab);
        buffer.append(") [VoidReturnStmt]");
        return buffer.toString();
    }
}
