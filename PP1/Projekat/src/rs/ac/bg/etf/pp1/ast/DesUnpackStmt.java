// generated with ast extension for cup
// version 0.8
// 6/0/2024 20:4:36


package rs.ac.bg.etf.pp1.ast;

public class DesUnpackStmt extends DesignatorStatement {

    private DesUnpackList DesUnpackList;
    private Designator Designator;
    private Designator Designator1;

    public DesUnpackStmt (DesUnpackList DesUnpackList, Designator Designator, Designator Designator1) {
        this.DesUnpackList=DesUnpackList;
        if(DesUnpackList!=null) DesUnpackList.setParent(this);
        this.Designator=Designator;
        if(Designator!=null) Designator.setParent(this);
        this.Designator1=Designator1;
        if(Designator1!=null) Designator1.setParent(this);
    }

    public DesUnpackList getDesUnpackList() {
        return DesUnpackList;
    }

    public void setDesUnpackList(DesUnpackList DesUnpackList) {
        this.DesUnpackList=DesUnpackList;
    }

    public Designator getDesignator() {
        return Designator;
    }

    public void setDesignator(Designator Designator) {
        this.Designator=Designator;
    }

    public Designator getDesignator1() {
        return Designator1;
    }

    public void setDesignator1(Designator Designator1) {
        this.Designator1=Designator1;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(DesUnpackList!=null) DesUnpackList.accept(visitor);
        if(Designator!=null) Designator.accept(visitor);
        if(Designator1!=null) Designator1.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(DesUnpackList!=null) DesUnpackList.traverseTopDown(visitor);
        if(Designator!=null) Designator.traverseTopDown(visitor);
        if(Designator1!=null) Designator1.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(DesUnpackList!=null) DesUnpackList.traverseBottomUp(visitor);
        if(Designator!=null) Designator.traverseBottomUp(visitor);
        if(Designator1!=null) Designator1.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("DesUnpackStmt(\n");

        if(DesUnpackList!=null)
            buffer.append(DesUnpackList.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(Designator!=null)
            buffer.append(Designator.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(Designator1!=null)
            buffer.append(Designator1.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [DesUnpackStmt]");
        return buffer.toString();
    }
}
