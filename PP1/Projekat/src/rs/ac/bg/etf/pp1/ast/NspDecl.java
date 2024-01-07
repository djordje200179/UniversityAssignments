// generated with ast extension for cup
// version 0.8
// 7/0/2024 20:31:52


package rs.ac.bg.etf.pp1.ast;

public class NspDecl implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private NspName NspName;
    private GlobalDecls GlobalDecls;
    private MethodDecls MethodDecls;

    public NspDecl (NspName NspName, GlobalDecls GlobalDecls, MethodDecls MethodDecls) {
        this.NspName=NspName;
        if(NspName!=null) NspName.setParent(this);
        this.GlobalDecls=GlobalDecls;
        if(GlobalDecls!=null) GlobalDecls.setParent(this);
        this.MethodDecls=MethodDecls;
        if(MethodDecls!=null) MethodDecls.setParent(this);
    }

    public NspName getNspName() {
        return NspName;
    }

    public void setNspName(NspName NspName) {
        this.NspName=NspName;
    }

    public GlobalDecls getGlobalDecls() {
        return GlobalDecls;
    }

    public void setGlobalDecls(GlobalDecls GlobalDecls) {
        this.GlobalDecls=GlobalDecls;
    }

    public MethodDecls getMethodDecls() {
        return MethodDecls;
    }

    public void setMethodDecls(MethodDecls MethodDecls) {
        this.MethodDecls=MethodDecls;
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
        if(NspName!=null) NspName.accept(visitor);
        if(GlobalDecls!=null) GlobalDecls.accept(visitor);
        if(MethodDecls!=null) MethodDecls.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(NspName!=null) NspName.traverseTopDown(visitor);
        if(GlobalDecls!=null) GlobalDecls.traverseTopDown(visitor);
        if(MethodDecls!=null) MethodDecls.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(NspName!=null) NspName.traverseBottomUp(visitor);
        if(GlobalDecls!=null) GlobalDecls.traverseBottomUp(visitor);
        if(MethodDecls!=null) MethodDecls.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("NspDecl(\n");

        if(NspName!=null)
            buffer.append(NspName.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(GlobalDecls!=null)
            buffer.append(GlobalDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(MethodDecls!=null)
            buffer.append(MethodDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [NspDecl]");
        return buffer.toString();
    }
}
