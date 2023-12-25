// generated with ast extension for cup
// version 0.8
// 25/11/2023 17:27:15


package rs.ac.bg.etf.pp1.ast;

public class NspDeclsDerived2 extends NspDecls {

    public NspDeclsDerived2 () {
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
        buffer.append("NspDeclsDerived2(\n");

        buffer.append(tab);
        buffer.append(") [NspDeclsDerived2]");
        return buffer.toString();
    }
}
