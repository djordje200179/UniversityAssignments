// generated with ast extension for cup
// version 0.8
// 7/0/2024 20:31:52


package rs.ac.bg.etf.pp1.ast;

public class DesUnpackListDerived1 extends DesUnpackList {

    private DesUnpackList DesUnpackList;
    private DesOpt DesOpt;

    public DesUnpackListDerived1 (DesUnpackList DesUnpackList, DesOpt DesOpt) {
        this.DesUnpackList=DesUnpackList;
        if(DesUnpackList!=null) DesUnpackList.setParent(this);
        this.DesOpt=DesOpt;
        if(DesOpt!=null) DesOpt.setParent(this);
    }

    public DesUnpackList getDesUnpackList() {
        return DesUnpackList;
    }

    public void setDesUnpackList(DesUnpackList DesUnpackList) {
        this.DesUnpackList=DesUnpackList;
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
        if(DesUnpackList!=null) DesUnpackList.accept(visitor);
        if(DesOpt!=null) DesOpt.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(DesUnpackList!=null) DesUnpackList.traverseTopDown(visitor);
        if(DesOpt!=null) DesOpt.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(DesUnpackList!=null) DesUnpackList.traverseBottomUp(visitor);
        if(DesOpt!=null) DesOpt.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("DesUnpackListDerived1(\n");

        if(DesUnpackList!=null)
            buffer.append(DesUnpackList.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(DesOpt!=null)
            buffer.append(DesOpt.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [DesUnpackListDerived1]");
        return buffer.toString();
    }
}
