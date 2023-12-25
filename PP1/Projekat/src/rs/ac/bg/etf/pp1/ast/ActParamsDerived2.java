// generated with ast extension for cup
// version 0.8
// 25/11/2023 23:7:5


package rs.ac.bg.etf.pp1.ast;

public class ActParamsDerived2 extends ActParams {

    private ActParam ActParam;

    public ActParamsDerived2 (ActParam ActParam) {
        this.ActParam=ActParam;
        if(ActParam!=null) ActParam.setParent(this);
    }

    public ActParam getActParam() {
        return ActParam;
    }

    public void setActParam(ActParam ActParam) {
        this.ActParam=ActParam;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(ActParam!=null) ActParam.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ActParam!=null) ActParam.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ActParam!=null) ActParam.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ActParamsDerived2(\n");

        if(ActParam!=null)
            buffer.append(ActParam.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ActParamsDerived2]");
        return buffer.toString();
    }
}
