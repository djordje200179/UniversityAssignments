// generated with ast extension for cup
// version 0.8
// 6/0/2024 19:34:18


package rs.ac.bg.etf.pp1.ast;

public class VarTypedDeclsDerived2 extends VarTypedDecls {

    public VarTypedDeclsDerived2 () {
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
        buffer.append("VarTypedDeclsDerived2(\n");

        buffer.append(tab);
        buffer.append(") [VarTypedDeclsDerived2]");
        return buffer.toString();
    }
}
