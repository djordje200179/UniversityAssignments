// generated with ast extension for cup
// version 0.8
// 6/0/2024 20:4:36


package rs.ac.bg.etf.pp1.ast;

public class MethodDecl implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private MethodSign MethodSign;
    private FormPars FormPars;
    private VarTypedDecls VarTypedDecls;
    private StmtList StmtList;

    public MethodDecl (MethodSign MethodSign, FormPars FormPars, VarTypedDecls VarTypedDecls, StmtList StmtList) {
        this.MethodSign=MethodSign;
        if(MethodSign!=null) MethodSign.setParent(this);
        this.FormPars=FormPars;
        if(FormPars!=null) FormPars.setParent(this);
        this.VarTypedDecls=VarTypedDecls;
        if(VarTypedDecls!=null) VarTypedDecls.setParent(this);
        this.StmtList=StmtList;
        if(StmtList!=null) StmtList.setParent(this);
    }

    public MethodSign getMethodSign() {
        return MethodSign;
    }

    public void setMethodSign(MethodSign MethodSign) {
        this.MethodSign=MethodSign;
    }

    public FormPars getFormPars() {
        return FormPars;
    }

    public void setFormPars(FormPars FormPars) {
        this.FormPars=FormPars;
    }

    public VarTypedDecls getVarTypedDecls() {
        return VarTypedDecls;
    }

    public void setVarTypedDecls(VarTypedDecls VarTypedDecls) {
        this.VarTypedDecls=VarTypedDecls;
    }

    public StmtList getStmtList() {
        return StmtList;
    }

    public void setStmtList(StmtList StmtList) {
        this.StmtList=StmtList;
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
        if(MethodSign!=null) MethodSign.accept(visitor);
        if(FormPars!=null) FormPars.accept(visitor);
        if(VarTypedDecls!=null) VarTypedDecls.accept(visitor);
        if(StmtList!=null) StmtList.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(MethodSign!=null) MethodSign.traverseTopDown(visitor);
        if(FormPars!=null) FormPars.traverseTopDown(visitor);
        if(VarTypedDecls!=null) VarTypedDecls.traverseTopDown(visitor);
        if(StmtList!=null) StmtList.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(MethodSign!=null) MethodSign.traverseBottomUp(visitor);
        if(FormPars!=null) FormPars.traverseBottomUp(visitor);
        if(VarTypedDecls!=null) VarTypedDecls.traverseBottomUp(visitor);
        if(StmtList!=null) StmtList.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("MethodDecl(\n");

        if(MethodSign!=null)
            buffer.append(MethodSign.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(FormPars!=null)
            buffer.append(FormPars.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(VarTypedDecls!=null)
            buffer.append(VarTypedDecls.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(StmtList!=null)
            buffer.append(StmtList.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [MethodDecl]");
        return buffer.toString();
    }
}
