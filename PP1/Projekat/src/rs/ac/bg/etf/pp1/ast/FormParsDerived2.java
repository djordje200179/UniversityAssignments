// generated with ast extension for cup
// version 0.8
// 25/11/2023 18:10:25


package rs.ac.bg.etf.pp1.ast;

public class FormParsDerived2 extends FormPars {

    public FormParsDerived2 () {
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
        buffer.append("FormParsDerived2(\n");

        buffer.append(tab);
        buffer.append(") [FormParsDerived2]");
        return buffer.toString();
    }
}
