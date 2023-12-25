// generated with ast extension for cup
// version 0.8
// 25/11/2023 17:27:15


package rs.ac.bg.etf.pp1.ast;

public class SingularAccess extends ElemAccess {

    public SingularAccess () {
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
        buffer.append("SingularAccess(\n");

        buffer.append(tab);
        buffer.append(") [SingularAccess]");
        return buffer.toString();
    }
}
