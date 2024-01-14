package rs.ac.bg.etf.pp1;

import rs.ac.bg.etf.pp1.ast.*;
import rs.etf.pp1.mj.runtime.Code;
import rs.etf.pp1.symboltable.Tab;
import rs.etf.pp1.symboltable.concepts.Obj;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Stack;

public class CodeGenerator extends VisitorAdaptor {
	private Map<String, Obj> helperVars;

	public CodeGenerator(Map<String, Obj> helperVars) {
		this.helperVars = helperVars;
	}

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
		if (cnst.getParent() instanceof ConstDecl)
			return;

		Code.loadConst(cnst.getValue());
	}

	@Override
	public void visit(CharConst cnst) {
		if (cnst.getParent() instanceof ConstDecl)
			return;

		Code.loadConst(cnst.getValue());
	}

	@Override
	public void visit(BoolConst cnst) {
		if (cnst.getParent() instanceof ConstDecl)
			return;

		Code.loadConst(cnst.getValue() ? 1 : 0);
	}

	@Override
	public void visit(PrintStmt stmt) {
		Code.loadConst(0);

		if (stmt.getExpr().struct != Tab.charType)
			Code.put(Code.print);
		else
			Code.put(Code.bprint);
	}

	@Override
	public void visit(PrintWidthStmt stmt) {
		Code.loadConst(stmt.getWidth());

		if (stmt.getExpr().struct != Tab.charType)
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
		var funcName = call.getDesignator().obj.getName();
		switch (funcName) {
		case "len":
			Code.put(Code.arraylength);
			return;
		case "chr":
		case "ord":
			return;
		}

		int offset = call.getDesignator().obj.getAdr() - Code.pc;
		Code.put(Code.call);
		Code.put2(offset);
	}

	@Override
	public void visit(NewArrayFactor factor) {
		Code.put(Code.newarray);

		int size = 1;
		if (factor.getType().struct == Tab.charType)
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
	public void visit(ArrayElemAccess elemAccess) {
		var designator = (Designator) elemAccess.getParent();
		if (designator.getParent() instanceof DesIncStmt || designator.getParent() instanceof DesDecStmt)
			Code.put(Code.dup2);
	}

	@Override
	public void visit(Designator designator) {
		var parent = designator.getParent();
		if (
			parent instanceof Call ||
			parent instanceof DesAssignStmt ||
			parent instanceof ReadStmt ||
			parent instanceof DesOptDes
		)
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

	public static class CondScope {
		public final List<Integer> thenPatchLocations = new ArrayList<>(),
								   elsePatchLocations = new ArrayList<>();
	}

	public static class ForScope {
		public int condTestLocation, updateLocation;
		public final List<Integer> breakPatchLocations = new ArrayList<>();
	}

	private final Stack<CondScope> condScopes = new Stack<>();
	private final Stack<ForScope> forScopes = new Stack<>();

	@Override
	public void visit(CondFactExpr factExpr) {
		Code.loadConst(0);
		Code.putFalseJump(Code.ne, 0);
		condScopes.peek().elsePatchLocations.add(Code.pc - 2);
	}

	@Override
	public void visit(Comparison comparison) {
		if (comparison.getRelop() instanceof EqualityRelop)
			Code.putFalseJump(Code.eq, 0);
		else if (comparison.getRelop() instanceof InequalityRelop)
			Code.putFalseJump(Code.ne, 0);
		else if (comparison.getRelop() instanceof GreaterRelop)
			Code.putFalseJump(Code.gt, 0);
		else if (comparison.getRelop() instanceof GreaterEqualRelop)
			Code.putFalseJump(Code.ge, 0);
		else if (comparison.getRelop() instanceof LessRelop)
			Code.putFalseJump(Code.lt, 0);
		else
			Code.putFalseJump(Code.le, 0);

		condScopes.peek().elsePatchLocations.add(Code.pc - 2);
	}

	@Override
	public void visit(CondOrJoiner joiner) {
		Code.putJump(0);

		var topScope = condScopes.peek();
		topScope.thenPatchLocations.add(Code.pc - 2);
		topScope.elsePatchLocations.forEach(Code::fixup);
		topScope.elsePatchLocations.clear();
	}

	@Override
	public void visit(IfBeforeCond beforeCond) {
		condScopes.push(new CondScope());
	}

	@Override
	public void visit(IfAfterCond afterCond) {
		condScopes.peek().thenPatchLocations.forEach(Code::fixup);
		condScopes.peek().thenPatchLocations.clear();
	}

	@Override
	public void visit(IfElseStart elseStart) {
		Code.putJump(0);

		var topScope = condScopes.peek();
		topScope.thenPatchLocations.add(Code.pc - 2);
		topScope.elsePatchLocations.forEach(Code::fixup);
	}

	@Override
	public void visit(IfStmt stmt) {
		condScopes.pop().elsePatchLocations.forEach(Code::fixup);
	}

	@Override
	public void visit(IfElseStmt stmt) {
		condScopes.pop().thenPatchLocations.forEach(Code::fixup);
	}

	@Override
	public void visit(ForBeforeCond beforeCond) {
		forScopes.push(new ForScope());
		condScopes.push(new CondScope());

		forScopes.peek().condTestLocation = Code.pc;
	}

	@Override
	public void visit(ForAfterCond afterCond) {
		Code.putJump(0);

		condScopes.peek().thenPatchLocations.add(Code.pc - 2);
		forScopes.peek().updateLocation = Code.pc;
	}

	@Override
	public void visit(ForSign sign) {
		Code.putJump(forScopes.peek().condTestLocation);
		condScopes.peek().thenPatchLocations.forEach(Code::fixup);
		condScopes.peek().thenPatchLocations.clear();
	}

	@Override
	public void visit(ForStmt stmt) {
		var forScope = forScopes.pop();
		var condScope = condScopes.pop();

		Code.putJump(forScope.updateLocation);

		condScope.elsePatchLocations.forEach(Code::fixup);
		forScope.breakPatchLocations.forEach(Code::fixup);
	}

	@Override
	public void visit(ContinueStmt stmt) {
		Code.putJump(forScopes.peek().updateLocation);
	}

	@Override
	public void visit(BreakStmt stmt) {
		Code.putJump(0);
		forScopes.peek().breakPatchLocations.add(Code.pc - 2);
	}

	@Override
	public void visit(DesUnpackStmt stmt) {
		var targets = new ArrayList<Obj>();
		stmt.getDesUnpackList().traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(DesOptDes optDes) {
				targets.add(optDes.getDesignator().obj);
			}

			@Override
			public void visit(DesOptEmpty optEmpty) {
				targets.add(Tab.noObj);
			}
		});

		/*
		overfill source
		source overfill source
		source overfill sourcelen
		source overfill N T
		 */

		Code.put(Code.dup_x1);
		Code.put(Code.dup);
		Code.put(Code.arraylength);
		Code.loadConst(targets.size());
		Code.putFalseJump(Code.le, Code.pc + (1 + 2) + (1 + 1));
		Code.put(Code.trap);
		Code.put(2);

		var indexVar = helperVars.get("index");
		var lengthVar = helperVars.get("length");

		var elemType = stmt.getDesignator().obj.getType().getElemType();

		Code.loadConst(0);
		Code.store(indexVar);

		Code.put(Code.dup);
		Code.put(Code.arraylength);
		Code.loadConst(targets.size());
		Code.put(Code.sub);
		Code.store(lengthVar);

		// index = 0, length = N - T

		// arr5[0] = arr6[T];
		// arr5[1] = arr6[T+ 1];
		// ...
		// arr5[N - T - 1] = arr6[N - 1];

		/*
			while (index < length) {
				arr5[index] = arr6[index + T];
				index = index + 1;
			}
		 */

		int copyStartAddr = Code.pc;

		Code.load(indexVar);
		Code.load(lengthVar);
		Code.putFalseJump(Code.lt, 0);
		int jumpAddr = Code.pc - 2;

		Code.put(Code.dup2);  // arr5 arr6
		Code.load(indexVar);  // arr5 arr6 index
		Code.put(Code.dup_x1); // arr5 index arr6 index
		Code.loadConst(targets.size()); // arr5 index arr6 index T
		Code.put(Code.add); // arr5 index arr6 index + T
		if (elemType == Tab.charType) {
			Code.put(Code.baload);
			Code.put(Code.bastore);
		} else {
			Code.put(Code.aload);
			Code.put(Code.astore);
		}

		Code.load(indexVar);
		Code.loadConst(1);
		Code.put(Code.add);
		Code.store(indexVar);

		Code.putJump(copyStartAddr);

		Code.fixup(jumpAddr);

		Code.put(Code.pop);
		Code.put(Code.pop);

		for (var i = targets.size() - 1; i >= 0; i--) {
			var target = targets.get(i);
			if (target == Tab.noObj)
				continue;

			Code.put(target.getKind() == Obj.Elem ? Code.dup_x2 : Code.dup);
			Code.loadConst(i);
			Code.put(elemType == Tab.charType ? Code.baload : Code.aload);
			Code.store(target);
		}
		Code.put(Code.pop);
	}
}
