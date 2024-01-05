// generated with ast extension for cup
// version 0.8
// 5/0/2024 18:48:54


package rs.ac.bg.etf.pp1.ast;

public interface Visitor { 

    public void visit(Mulop Mulop);
    public void visit(VarRef VarRef);
    public void visit(Relop Relop);
    public void visit(NspDecls NspDecls);
    public void visit(ElemAccess ElemAccess);
    public void visit(VarQuantity VarQuantity);
    public void visit(DesStmtList DesStmtList);
    public void visit(ActParams ActParams);
    public void visit(Addop Addop);
    public void visit(GlobalDecls GlobalDecls);
    public void visit(ForCondition ForCondition);
    public void visit(Factor Factor);
    public void visit(CondTerm CondTerm);
    public void visit(MethodSign MethodSign);
    public void visit(Term Term);
    public void visit(Condition Condition);
    public void visit(FormParams FormParams);
    public void visit(ForDesStmtList ForDesStmtList);
    public void visit(StmtList StmtList);
    public void visit(Expr Expr);
    public void visit(ActPars ActPars);
    public void visit(DesignatorStatement DesignatorStatement);
    public void visit(Const Const);
    public void visit(VarDecls VarDecls);
    public void visit(Statement Statement);
    public void visit(ConstDecls ConstDecls);
    public void visit(CondFact CondFact);
    public void visit(VarTypedDecls VarTypedDecls);
    public void visit(GlobalDecl GlobalDecl);
    public void visit(FormPars FormPars);
    public void visit(MethodDecls MethodDecls);
    public void visit(CondTermCondition CondTermCondition);
    public void visit(ConditionList ConditionList);
    public void visit(CondFactTerm CondFactTerm);
    public void visit(CondTermList CondTermList);
    public void visit(CondFactExpr CondFactExpr);
    public void visit(Comparison Comparison);
    public void visit(SingularAccess SingularAccess);
    public void visit(ArrayElemAccess ArrayElemAccess);
    public void visit(Designator Designator);
    public void visit(NspVarRef NspVarRef);
    public void visit(BlankVarRef BlankVarRef);
    public void visit(ExprNegTerm ExprNegTerm);
    public void visit(ExprTerm ExprTerm);
    public void visit(ExprTermList ExprTermList);
    public void visit(FactorTerm FactorTerm);
    public void visit(FactorTermList FactorTermList);
    public void visit(DesignatorFactor DesignatorFactor);
    public void visit(CallFactor CallFactor);
    public void visit(NewArrayFactor NewArrayFactor);
    public void visit(GroupedFactor GroupedFactor);
    public void visit(ConstFactor ConstFactor);
    public void visit(ActParam ActParam);
    public void visit(ActParamsDerived2 ActParamsDerived2);
    public void visit(ActParamsDerived1 ActParamsDerived1);
    public void visit(ActParsDerived2 ActParsDerived2);
    public void visit(ActParsDerived1 ActParsDerived1);
    public void visit(Call Call);
    public void visit(RelopDerived4 RelopDerived4);
    public void visit(RelopDerived3 RelopDerived3);
    public void visit(RelopDerived2 RelopDerived2);
    public void visit(RelopDerived1 RelopDerived1);
    public void visit(InequalityRelop InequalityRelop);
    public void visit(EqualityRelop EqualityRelop);
    public void visit(PercentMulop PercentMulop);
    public void visit(SlashMulop SlashMulop);
    public void visit(AsteriskMulop AsteriskMulop);
    public void visit(MinusAddOp MinusAddOp);
    public void visit(PlusAddOp PlusAddOp);
    public void visit(DesDecStmt DesDecStmt);
    public void visit(DesIncStmt DesIncStmt);
    public void visit(DesAssignStmt DesAssignStmt);
    public void visit(DesCallStmt DesCallStmt);
    public void visit(DesStmtListDerived2 DesStmtListDerived2);
    public void visit(DesStmtListDerived1 DesStmtListDerived1);
    public void visit(ForDesStmtListDerived2 ForDesStmtListDerived2);
    public void visit(ForDesStmtListDerived1 ForDesStmtListDerived1);
    public void visit(InfiniteFor InfiniteFor);
    public void visit(FiniteFor FiniteFor);
    public void visit(ForSign ForSign);
    public void visit(StatementDerived1 StatementDerived1);
    public void visit(ForStmt ForStmt);
    public void visit(IfStmt IfStmt);
    public void visit(IfElseStmt IfElseStmt);
    public void visit(DesStmt DesStmt);
    public void visit(PrintStmt PrintStmt);
    public void visit(PrintWidthStmt PrintWidthStmt);
    public void visit(ReadStmt ReadStmt);
    public void visit(ContinueStmt ContinueStmt);
    public void visit(BreakStmt BreakStmt);
    public void visit(VoidReturnStmt VoidReturnStmt);
    public void visit(ValueReturnStmt ValueReturnStmt);
    public void visit(StmtListDerived2 StmtListDerived2);
    public void visit(StmtListDerived1 StmtListDerived1);
    public void visit(FormParam FormParam);
    public void visit(FormParamsDerived2 FormParamsDerived2);
    public void visit(FormParamsDerived1 FormParamsDerived1);
    public void visit(FormParsDerived2 FormParsDerived2);
    public void visit(FormParsDerived1 FormParsDerived1);
    public void visit(VoidMethodSign VoidMethodSign);
    public void visit(ValueMethodSign ValueMethodSign);
    public void visit(MethodDecl MethodDecl);
    public void visit(MethodDeclsDerived2 MethodDeclsDerived2);
    public void visit(MethodDeclsDerived1 MethodDeclsDerived1);
    public void visit(SingularVar SingularVar);
    public void visit(ArrayVar ArrayVar);
    public void visit(VarDecl VarDecl);
    public void visit(VarDeclsDerived2 VarDeclsDerived2);
    public void visit(VarDeclsDerived1 VarDeclsDerived1);
    public void visit(VarTypedDecl VarTypedDecl);
    public void visit(VarTypedDeclsDerived2 VarTypedDeclsDerived2);
    public void visit(VarTypedDeclsDerived1 VarTypedDeclsDerived1);
    public void visit(BoolConst BoolConst);
    public void visit(CharConst CharConst);
    public void visit(IntConst IntConst);
    public void visit(ConstDecl ConstDecl);
    public void visit(ConstDeclsDerived2 ConstDeclsDerived2);
    public void visit(ConstDeclsDerived1 ConstDeclsDerived1);
    public void visit(ConstTypedDecl ConstTypedDecl);
    public void visit(GlobalVarDecl GlobalVarDecl);
    public void visit(GlobalConstDecl GlobalConstDecl);
    public void visit(GlobalDeclsDerived2 GlobalDeclsDerived2);
    public void visit(GlobalDeclsDerived1 GlobalDeclsDerived1);
    public void visit(Type Type);
    public void visit(NspName NspName);
    public void visit(NspDecl NspDecl);
    public void visit(NspDeclsDerived2 NspDeclsDerived2);
    public void visit(NspDeclsDerived1 NspDeclsDerived1);
    public void visit(ProgName ProgName);
    public void visit(Program Program);

}
