package rs.ac.bg.etf.pp1;

import rs.ac.bg.etf.pp1.ast.*;
import rs.etf.pp1.mj.runtime.Code;
import rs.etf.pp1.symboltable.Tab;

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
}
