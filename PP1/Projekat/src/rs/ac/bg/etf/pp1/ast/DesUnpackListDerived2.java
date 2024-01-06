// generated with ast extension for cup
// version 0.8
// 6/0/2024 20:25:7


package rs.ac.bg.etf.pp1.ast;

public class DesUnpackListDerived2 extends DesUnpackList {

    private DesOpt DesOpt;

    public DesUnpackListDerived2 (DesOpt DesOpt) {
        this.DesOpt=DesOpt;
        if(DesOpt!=null) DesOpt.setParent(this);
    }

    public DesOpt getDesOpt() {
        return DesOpt;
    }

    public void setDesOpt(DesOpt DesOpt) {
        this.DesOpt=DesOpt;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(DesOpt!=null) DesOpt.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(DesOpt!=null) DesOpt.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(DesOpt!=null) DesOpt.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("DesUnpackListDerived2(\n");

        if(DesOpt!=null)
            buffer.append(DesOpt.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [DesUnpackListDerived2]");
        return buffer.toString();
    }
}
