// generated with ast extension for cup
// version 0.8
// 13/0/2024 13:23:46


package rs.ac.bg.etf.pp1.ast;

public class StmtListDerived2 extends StmtList {

    public StmtListDerived2 () {
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
        buffer.append("StmtListDerived2(\n");

        buffer.append(tab);
        buffer.append(") [StmtListDerived2]");
        return buffer.toString();
    }
}
