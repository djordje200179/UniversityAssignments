// generated with ast extension for cup
// version 0.8
// 7/0/2024 20:31:52


package rs.ac.bg.etf.pp1.ast;

public class DesUnpackListDerived2 extends DesUnpackList {

    public DesUnpackListDerived2 () {
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
        buffer.append("DesUnpackListDerived2(\n");

        buffer.append(tab);
        buffer.append(") [DesUnpackListDerived2]");
        return buffer.toString();
    }
}
