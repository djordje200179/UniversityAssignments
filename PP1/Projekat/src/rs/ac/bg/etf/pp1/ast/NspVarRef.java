// generated with ast extension for cup
// version 0.8
// 6/0/2024 20:25:7


package rs.ac.bg.etf.pp1.ast;

public class NspVarRef extends VarRef {

    private String nspName;
    private String varName;

    public NspVarRef (String nspName, String varName) {
        this.nspName=nspName;
        this.varName=varName;
    }

    public String getNspName() {
        return nspName;
    }

    public void setNspName(String nspName) {
        this.nspName=nspName;
    }

    public String getVarName() {
        return varName;
    }

    public void setVarName(String varName) {
        this.varName=varName;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("NspVarRef(\n");

        buffer.append(" "+tab+nspName);
        buffer.append("\n");

        buffer.append(" "+tab+varName);
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [NspVarRef]");
        return buffer.toString();
    }
}
