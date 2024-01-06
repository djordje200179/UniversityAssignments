// generated with ast extension for cup
// version 0.8
// 6/0/2024 23:1:17


package rs.ac.bg.etf.pp1.ast;

public class ForStmt extends Statement {

    private ForSign ForSign;
    private Statement Statement;

    public ForStmt (ForSign ForSign, Statement Statement) {
        this.ForSign=ForSign;
        if(ForSign!=null) ForSign.setParent(this);
        this.Statement=Statement;
        if(Statement!=null) Statement.setParent(this);
    }

    public ForSign getForSign() {
        return ForSign;
    }

    public void setForSign(ForSign ForSign) {
        this.ForSign=ForSign;
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
        if(ForSign!=null) ForSign.accept(visitor);
        if(Statement!=null) Statement.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ForSign!=null) ForSign.traverseTopDown(visitor);
        if(Statement!=null) Statement.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ForSign!=null) ForSign.traverseBottomUp(visitor);
        if(Statement!=null) Statement.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ForStmt(\n");

        if(ForSign!=null)
            buffer.append(ForSign.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(Statement!=null)
            buffer.append(Statement.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ForStmt]");
        return buffer.toString();
    }
}
