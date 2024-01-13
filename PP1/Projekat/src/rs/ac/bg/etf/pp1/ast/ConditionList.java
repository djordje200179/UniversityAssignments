// generated with ast extension for cup
// version 0.8
// 13/0/2024 13:23:46


package rs.ac.bg.etf.pp1.ast;

public class ConditionList extends Condition {

    private Condition Condition;
    private CondOrJoiner CondOrJoiner;
    private CondTerm CondTerm;

    public ConditionList (Condition Condition, CondOrJoiner CondOrJoiner, CondTerm CondTerm) {
        this.Condition=Condition;
        if(Condition!=null) Condition.setParent(this);
        this.CondOrJoiner=CondOrJoiner;
        if(CondOrJoiner!=null) CondOrJoiner.setParent(this);
        this.CondTerm=CondTerm;
        if(CondTerm!=null) CondTerm.setParent(this);
    }

    public Condition getCondition() {
        return Condition;
    }

    public void setCondition(Condition Condition) {
        this.Condition=Condition;
    }

    public CondOrJoiner getCondOrJoiner() {
        return CondOrJoiner;
    }

    public void setCondOrJoiner(CondOrJoiner CondOrJoiner) {
        this.CondOrJoiner=CondOrJoiner;
    }

    public CondTerm getCondTerm() {
        return CondTerm;
    }

    public void setCondTerm(CondTerm CondTerm) {
        this.CondTerm=CondTerm;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(Condition!=null) Condition.accept(visitor);
        if(CondOrJoiner!=null) CondOrJoiner.accept(visitor);
        if(CondTerm!=null) CondTerm.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(Condition!=null) Condition.traverseTopDown(visitor);
        if(CondOrJoiner!=null) CondOrJoiner.traverseTopDown(visitor);
        if(CondTerm!=null) CondTerm.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(Condition!=null) Condition.traverseBottomUp(visitor);
        if(CondOrJoiner!=null) CondOrJoiner.traverseBottomUp(visitor);
        if(CondTerm!=null) CondTerm.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ConditionList(\n");

        if(Condition!=null)
            buffer.append(Condition.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(CondOrJoiner!=null)
            buffer.append(CondOrJoiner.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(CondTerm!=null)
            buffer.append(CondTerm.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ConditionList]");
        return buffer.toString();
    }
}
