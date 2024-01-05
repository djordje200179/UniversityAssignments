package rs.ac.bg.etf.pp1;

import rs.ac.bg.etf.pp1.ast.*;
import rs.etf.pp1.mj.runtime.Code;
import rs.etf.pp1.symboltable.Tab;
import rs.etf.pp1.symboltable.concepts.Obj;

public class CodeGenerator extends VisitorAdaptor {
	@Override
	public void visit(Program prog) {
		Code.dataSize = prog.getProgName().obj.getLocalSymbols().size();
	}

	@Override
	public void visit(VoidMethodSign methodSign) {
		methodSign.obj.setAdr(Code.pc);

		if (methodSign.obj.getName().equals("main"))
			Code.mainPc = Code.pc;

		Code.put(Code.enter);
		Code.put(methodSign.obj.getLevel());
		Code.put(methodSign.obj.getLocalSymbols().size());
	}

	@Override
	public void visit(ValueMethodSign methodSign) {
		methodSign.obj.setAdr(Code.pc);

		Code.put(Code.enter);
		Code.put(methodSign.obj.getLevel());
		Code.put(methodSign.obj.getLocalSymbols().size());
	}

	@Override
	public void visit(MethodDecl methodDecl) {
		if (methodDecl.getMethodSign() instanceof ValueMethodSign) {
			Code.put(Code.trap);
			Code.put(1);
		} else {
			Code.put(Code.exit);
			Code.put(Code.return_);
		}
	}

	@Override
	public void visit(IntConst cnst) {
		Code.loadConst(cnst.getValue());
	}

	@Override
	public void visit(CharConst cnst) {
		Code.loadConst(cnst.getValue());
	}

	@Override
	public void visit(BoolConst cnst) {
		Code.loadConst(cnst.getValue() ? 1 : 0);
	}

	@Override
	public void visit(PrintStmt stmt) {
		Code.loadConst(0);

		if (stmt.getExpr().struct == Tab.intType)
			Code.put(Code.print);
		else
			Code.put(Code.bprint);
	}

	@Override
	public void visit(PrintWidthStmt stmt) {
		Code.loadConst(stmt.getWidth());

		if (stmt.getExpr().struct == Tab.intType)
			Code.put(Code.print);
		else
			Code.put(Code.bprint);
	}

	@Override
	public void visit(ReadStmt stmt) {
		if (stmt.getDesignator().obj.getType() == Tab.intType)
			Code.put(Code.read);
		else
			Code.put(Code.bread);

		Code.store(stmt.getDesignator().obj);
	}

	@Override
	public void visit(VoidReturnStmt stmt) {
		Code.put(Code.exit);
		Code.put(Code.return_);
	}

	@Override
	public void visit(ValueReturnStmt stmt) {
		Code.put(Code.exit);
		Code.put(Code.return_);
	}

	@Override
	public void visit(Call call) {
		int offset = call.getDesignator().obj.getAdr() - Code.pc;
		Code.put(Code.call);
		Code.put2(offset);
	}

	@Override
	public void visit(NewArrayFactor factor) {
		Code.put(Code.newarray);

		int size = 1;
		if (factor.getType().struct == Tab.charType || factor.getType().struct == Types.boolType)
			size = 0;

		Code.put(size);
	}

	@Override
	public void visit(FactorTermList termList) {
		if (termList.getMulop() instanceof AsteriskMulop)
			Code.put(Code.mul);
		else if (termList.getMulop() instanceof SlashMulop)
			Code.put(Code.div);
		else
			Code.put(Code.rem);
	}

	@Override
	public void visit(ExprTermList exprList) {
		if (exprList.getAddop() instanceof PlusAddOp)
			Code.put(Code.add);
		else
			Code.put(Code.sub);
	}

	@Override
	public void visit(ExprNegTerm term) {
		Code.put(Code.neg);
	}

	@Override
	public void visit(BlankVarRef varRef) {
		var designator = (Designator) varRef.getParent();
		if (designator.getElemAccess() instanceof ArrayElemAccess)
			Code.load(varRef.obj);
	}

	@Override
	public void visit(NspVarRef varRef) {
		var designator = (Designator) varRef.getParent();
		if (designator.getElemAccess() instanceof ArrayElemAccess)
			Code.load(varRef.obj);
	}

	@Override
	public void visit(Designator designator) {
		if (designator.getParent() instanceof Call || designator.getParent() instanceof DesAssignStmt)
			return;

		Code.load(designator.obj);
	}

	@Override
	public void visit(DesCallStmt stmt) {
		if (stmt.getCall().struct != Tab.noType)
			Code.put(Code.pop);
	}

	@Override
	public void visit(DesAssignStmt stmt) {
		Code.store(stmt.getDesignator().obj);
	}

	@Override
	public void visit(DesIncStmt stmt) {
		Code.loadConst(1);
		Code.put(Code.add);
		Code.store(stmt.getDesignator().obj);
	}

	@Override
	public void visit(DesDecStmt stmt) {
		Code.loadConst(1);
		Code.put(Code.sub);
		Code.store(stmt.getDesignator().obj);
	}
}
