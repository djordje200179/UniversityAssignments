// generated with ast extension for cup
// version 0.8
// 25/11/2023 15:6:21


package rs.ac.bg.etf.pp1.ast;

public class MethodDecl implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private MethodSign MethodSign;
    private FormPars FormPars;
    private VarTypedDecls VarTypedDecls;
    private Statement Statement;

    public MethodDecl (MethodSign MethodSign, FormPars FormPars, VarTypedDecls VarTypedDecls, Statement Statement) {
        this.MethodSign=MethodSign;
        if(MethodSign!=null) MethodSign.setParent(this);
        this.FormPars=FormPars;
        if(FormPars!=null) FormPars.setParent(this);
        this.VarTypedDecls=VarTypedDecls;
        if(VarTypedDecls!=null) VarTypedDecls.setParent(this);
        this.Statement=Statement;
        if(Statement!=null) Statement.setParent(this);
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

    public Statement getStatement() {
        return Statement;
    }

    public void setStatement(Statement Statement) {
        this.Statement=Statement;
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
        if(Statement!=null) Statement.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(MethodSign!=null) MethodSign.traverseTopDown(visitor);
        if(FormPars!=null) FormPars.traverseTopDown(visitor);
        if(VarTypedDecls!=null) VarTypedDecls.traverseTopDown(visitor);
        if(Statement!=null) Statement.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(MethodSign!=null) MethodSign.traverseBottomUp(visitor);
        if(FormPars!=null) FormPars.traverseBottomUp(visitor);
        if(VarTypedDecls!=null) VarTypedDecls.traverseBottomUp(visitor);
        if(Statement!=null) Statement.traverseBottomUp(visitor);
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

        if(Statement!=null)
            buffer.append(Statement.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [MethodDecl]");
        return buffer.toString();
    }
}
