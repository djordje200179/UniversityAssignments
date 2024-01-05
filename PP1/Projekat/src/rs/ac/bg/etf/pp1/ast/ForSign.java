// generated with ast extension for cup
// version 0.8
// 5/0/2024 12:37:54


package rs.ac.bg.etf.pp1.ast;

public class ForSign implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private ForDesStmtList ForDesStmtList;
    private ForCondition ForCondition;
    private ForDesStmtList ForDesStmtList1;

    public ForSign (ForDesStmtList ForDesStmtList, ForCondition ForCondition, ForDesStmtList ForDesStmtList1) {
        this.ForDesStmtList=ForDesStmtList;
        if(ForDesStmtList!=null) ForDesStmtList.setParent(this);
        this.ForCondition=ForCondition;
        if(ForCondition!=null) ForCondition.setParent(this);
        this.ForDesStmtList1=ForDesStmtList1;
        if(ForDesStmtList1!=null) ForDesStmtList1.setParent(this);
    }

    public ForDesStmtList getForDesStmtList() {
        return ForDesStmtList;
    }

    public void setForDesStmtList(ForDesStmtList ForDesStmtList) {
        this.ForDesStmtList=ForDesStmtList;
    }

    public ForCondition getForCondition() {
        return ForCondition;
    }

    public void setForCondition(ForCondition ForCondition) {
        this.ForCondition=ForCondition;
    }

    public ForDesStmtList getForDesStmtList1() {
        return ForDesStmtList1;
    }

    public void setForDesStmtList1(ForDesStmtList ForDesStmtList1) {
        this.ForDesStmtList1=ForDesStmtList1;
    }

    public SyntaxNode getParent() {
        return parent;
    }

    public void setParent(SyntaxNode parent) {
        this.parent=parent;
    }

    public int getLine() {
        return line;
    }

    public void setLine(int line) {
        this.line=line;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(ForDesStmtList!=null) ForDesStmtList.accept(visitor);
        if(ForCondition!=null) ForCondition.accept(visitor);
        if(ForDesStmtList1!=null) ForDesStmtList1.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ForDesStmtList!=null) ForDesStmtList.traverseTopDown(visitor);
        if(ForCondition!=null) ForCondition.traverseTopDown(visitor);
        if(ForDesStmtList1!=null) ForDesStmtList1.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ForDesStmtList!=null) ForDesStmtList.traverseBottomUp(visitor);
        if(ForCondition!=null) ForCondition.traverseBottomUp(visitor);
        if(ForDesStmtList1!=null) ForDesStmtList1.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ForSign(\n");

        if(ForDesStmtList!=null)
            buffer.append(ForDesStmtList.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ForCondition!=null)
            buffer.append(ForCondition.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ForDesStmtList1!=null)
            buffer.append(ForDesStmtList1.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ForSign]");
        return buffer.toString();
    }
}
