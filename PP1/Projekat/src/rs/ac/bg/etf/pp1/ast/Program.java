// generated with ast extension for cup
// version 0.8
// 25/11/2023 15:6:21


package rs.ac.bg.etf.pp1.ast;

public class Program implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private ProgName ProgName;
    private NspDecls NspDecls;
    private GlobalDecls GlobalDecls;
    private MethodDecls MethodDecls;

    public Program (ProgName ProgName, NspDecls NspDecls, GlobalDecls GlobalDecls, MethodDecls MethodDecls) {
        this.ProgName=ProgName;
        if(ProgName!=null) ProgName.setParent(this);
        this.NspDecls=NspDecls;
        if(NspDecls!=null) NspDecls.setParent(this);
        this.GlobalDecls=GlobalDecls;
        if(GlobalDecls!=null) GlobalDecls.setParent(this);
        this.MethodDecls=MethodDecls;
        if(MethodDecls!=null) MethodDecls.setParent(this);
    }

    public ProgName getProgName() {
        return ProgName;
    }

    public void setProgName(ProgName ProgName) {
        this.ProgName=ProgName;
    }

    public NspDecls getNspDecls() {
        return NspDecls;
    }

    public void setNspDecls(NspDecls NspDecls) {
        this.NspDecls=NspDecls;
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
        if(ProgName!=null) ProgName.accept(visitor);
        if(NspDecls!=null) NspDecls.accept(visitor);
        if(GlobalDecls!=null) GlobalDecls.accept(visitor);
        if(MethodDecls!=null) MethodDecls.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ProgName!=null) ProgName.traverseTopDown(visitor);
        if(NspDecls!=null) NspDecls.traverseTopDown(visitor);
        if(GlobalDecls!=null) GlobalDecls.traverseTopDown(visitor);
        if(MethodDecls!=null) MethodDecls.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ProgName!=null) ProgName.traverseBottomUp(visitor);
        if(NspDecls!=null) NspDecls.traverseBottomUp(visitor);
        if(GlobalDecls!=null) GlobalDecls.traverseBottomUp(visitor);
        if(MethodDecls!=null) MethodDecls.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("Program(\n");

        if(ProgName!=null)
            buffer.append(ProgName.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(NspDecls!=null)
            buffer.append(NspDecls.toString("  "+tab));
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
        buffer.append(") [Program]");
        return buffer.toString();
    }
}
