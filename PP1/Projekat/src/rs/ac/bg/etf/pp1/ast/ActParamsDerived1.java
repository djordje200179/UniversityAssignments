// generated with ast extension for cup
// version 0.8
// 25/11/2023 23:7:5


package rs.ac.bg.etf.pp1.ast;

public class ActParamsDerived1 extends ActParams {

    private ActParams ActParams;
    private ActParam ActParam;

    public ActParamsDerived1 (ActParams ActParams, ActParam ActParam) {
        this.ActParams=ActParams;
        if(ActParams!=null) ActParams.setParent(this);
        this.ActParam=ActParam;
        if(ActParam!=null) ActParam.setParent(this);
    }

    public ActParams getActParams() {
        return ActParams;
    }

    public void setActParams(ActParams ActParams) {
        this.ActParams=ActParams;
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
        if(ActParams!=null) ActParams.accept(visitor);
        if(ActParam!=null) ActParam.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ActParams!=null) ActParams.traverseTopDown(visitor);
        if(ActParam!=null) ActParam.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ActParams!=null) ActParams.traverseBottomUp(visitor);
        if(ActParam!=null) ActParam.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ActParamsDerived1(\n");

        if(ActParams!=null)
            buffer.append(ActParams.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ActParam!=null)
            buffer.append(ActParam.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ActParamsDerived1]");
        return buffer.toString();
    }
}
