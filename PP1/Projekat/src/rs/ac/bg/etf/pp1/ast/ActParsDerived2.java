// generated with ast extension for cup
// version 0.8
// 25/11/2023 23:15:59


package rs.ac.bg.etf.pp1.ast;

public class ActParsDerived2 extends ActPars {

    public ActParsDerived2 () {
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
        buffer.append("ActParsDerived2(\n");

        buffer.append(tab);
        buffer.append(") [ActParsDerived2]");
        return buffer.toString();
    }
}
