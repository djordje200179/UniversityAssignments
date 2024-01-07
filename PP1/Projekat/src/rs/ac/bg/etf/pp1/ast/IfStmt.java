// generated with ast extension for cup
// version 0.8
// 7/0/2024 17:35:6


package rs.ac.bg.etf.pp1.ast;

public class IfStmt extends Statement {

    private IfBeforeCond IfBeforeCond;
    private ConditionTry ConditionTry;
    private IfAfterCond IfAfterCond;
    private Statement Statement;

    public IfStmt (IfBeforeCond IfBeforeCond, ConditionTry ConditionTry, IfAfterCond IfAfterCond, Statement Statement) {
        this.IfBeforeCond=IfBeforeCond;
        if(IfBeforeCond!=null) IfBeforeCond.setParent(this);
        this.ConditionTry=ConditionTry;
        if(ConditionTry!=null) ConditionTry.setParent(this);
        this.IfAfterCond=IfAfterCond;
        if(IfAfterCond!=null) IfAfterCond.setParent(this);
        this.Statement=Statement;
        if(Statement!=null) Statement.setParent(this);
    }

    public IfBeforeCond getIfBeforeCond() {
        return IfBeforeCond;
    }

    public void setIfBeforeCond(IfBeforeCond IfBeforeCond) {
        this.IfBeforeCond=IfBeforeCond;
    }

    public ConditionTry getConditionTry() {
        return ConditionTry;
    }

    public void setConditionTry(ConditionTry ConditionTry) {
        this.ConditionTry=ConditionTry;
    }

    public IfAfterCond getIfAfterCond() {
        return IfAfterCond;
    }

    public void setIfAfterCond(IfAfterCond IfAfterCond) {
        this.IfAfterCond=IfAfterCond;
    }

    public Statement getStatement() {
        return Statement;
    }

    public void setStatement(Statement Statement) {
        this.Statement=Statement;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(IfBeforeCond!=null) IfBeforeCond.accept(visitor);
        if(ConditionTry!=null) ConditionTry.accept(visitor);
        if(IfAfterCond!=null) IfAfterCond.accept(visitor);
        if(Statement!=null) Statement.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(IfBeforeCond!=null) IfBeforeCond.traverseTopDown(visitor);
        if(ConditionTry!=null) ConditionTry.traverseTopDown(visitor);
        if(IfAfterCond!=null) IfAfterCond.traverseTopDown(visitor);
        if(Statement!=null) Statement.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(IfBeforeCond!=null) IfBeforeCond.traverseBottomUp(visitor);
        if(ConditionTry!=null) ConditionTry.traverseBottomUp(visitor);
        if(IfAfterCond!=null) IfAfterCond.traverseBottomUp(visitor);
        if(Statement!=null) Statement.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("IfStmt(\n");

        if(IfBeforeCond!=null)
            buffer.append(IfBeforeCond.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ConditionTry!=null)
            buffer.append(ConditionTry.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(IfAfterCond!=null)
            buffer.append(IfAfterCond.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(Statement!=null)
            buffer.append(Statement.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [IfStmt]");
        return buffer.toString();
    }
}
