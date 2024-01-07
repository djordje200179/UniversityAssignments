// generated with ast extension for cup
// version 0.8
// 7/0/2024 17:35:6


package rs.ac.bg.etf.pp1.ast;

public class ForSign implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private ForDesStmtList ForDesStmtList;
    private ForBeforeCond ForBeforeCond;
    private ForCond ForCond;
    private ForAfterCond ForAfterCond;
    private ForDesStmtList ForDesStmtList1;

    public ForSign (ForDesStmtList ForDesStmtList, ForBeforeCond ForBeforeCond, ForCond ForCond, ForAfterCond ForAfterCond, ForDesStmtList ForDesStmtList1) {
        this.ForDesStmtList=ForDesStmtList;
        if(ForDesStmtList!=null) ForDesStmtList.setParent(this);
        this.ForBeforeCond=ForBeforeCond;
        if(ForBeforeCond!=null) ForBeforeCond.setParent(this);
        this.ForCond=ForCond;
        if(ForCond!=null) ForCond.setParent(this);
        this.ForAfterCond=ForAfterCond;
        if(ForAfterCond!=null) ForAfterCond.setParent(this);
        this.ForDesStmtList1=ForDesStmtList1;
        if(ForDesStmtList1!=null) ForDesStmtList1.setParent(this);
    }

    public ForDesStmtList getForDesStmtList() {
        return ForDesStmtList;
    }

    public void setForDesStmtList(ForDesStmtList ForDesStmtList) {
        this.ForDesStmtList=ForDesStmtList;
    }

    public ForBeforeCond getForBeforeCond() {
        return ForBeforeCond;
    }

    public void setForBeforeCond(ForBeforeCond ForBeforeCond) {
        this.ForBeforeCond=ForBeforeCond;
    }

    public ForCond getForCond() {
        return ForCond;
    }

    public void setForCond(ForCond ForCond) {
        this.ForCond=ForCond;
    }

    public ForAfterCond getForAfterCond() {
        return ForAfterCond;
    }

    public void setForAfterCond(ForAfterCond ForAfterCond) {
        this.ForAfterCond=ForAfterCond;
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
        if(ForBeforeCond!=null) ForBeforeCond.accept(visitor);
        if(ForCond!=null) ForCond.accept(visitor);
        if(ForAfterCond!=null) ForAfterCond.accept(visitor);
        if(ForDesStmtList1!=null) ForDesStmtList1.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ForDesStmtList!=null) ForDesStmtList.traverseTopDown(visitor);
        if(ForBeforeCond!=null) ForBeforeCond.traverseTopDown(visitor);
        if(ForCond!=null) ForCond.traverseTopDown(visitor);
        if(ForAfterCond!=null) ForAfterCond.traverseTopDown(visitor);
        if(ForDesStmtList1!=null) ForDesStmtList1.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ForDesStmtList!=null) ForDesStmtList.traverseBottomUp(visitor);
        if(ForBeforeCond!=null) ForBeforeCond.traverseBottomUp(visitor);
        if(ForCond!=null) ForCond.traverseBottomUp(visitor);
        if(ForAfterCond!=null) ForAfterCond.traverseBottomUp(visitor);
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

        if(ForBeforeCond!=null)
            buffer.append(ForBeforeCond.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ForCond!=null)
            buffer.append(ForCond.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ForAfterCond!=null)
            buffer.append(ForAfterCond.toString("  "+tab));
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
