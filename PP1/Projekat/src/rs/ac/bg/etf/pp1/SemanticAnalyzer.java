package rs.ac.bg.etf.pp1;

import rs.ac.bg.etf.pp1.ast.*;
import rs.etf.pp1.symboltable.Tab;
import rs.etf.pp1.symboltable.concepts.Obj;
import rs.etf.pp1.symboltable.concepts.Struct;
import java.util.ArrayList;

public class SemanticAnalyzer extends VisitorAdaptor {
	private boolean errorDetected = false;

	private int constCount = 0,
				globalVarCount = 0,
				localVarCount = 0,
				callsCount = 0,
				arrAccessCount = 0,
				argUsageCount = 0;

	public void dumpStats() {
		var sb = new StringBuilder();

		sb.append("=====================================\n");
		sb.append("Number of constants: ");
		sb.append(constCount);
		sb.append('\n');
		sb.append("Number of global variables: ");
		sb.append(globalVarCount);
		sb.append('\n');
		sb.append("Number of local variables: ");
		sb.append(localVarCount);
		sb.append('\n');
		sb.append("Number of function calls: ");
		sb.append(callsCount);
		sb.append('\n');
		sb.append("Number of array accesses: ");
		sb.append(arrAccessCount);
		sb.append('\n');
		sb.append("Number of usages of arguments: ");
		sb.append(argUsageCount);
		sb.append('\n');
		sb.append("=====================================\n");

		System.out.println(sb);
	}

	public boolean hasError() {
		return errorDetected;
	}

	public void reportError(String message, SyntaxNode info) {
		errorDetected = true;

		System.err.println(info != null ? message + " at line " + info.getLine() : message);
	}

	private String currNsp = "";
	private Obj currMethod = null;
	private int loopCnt = 0;

	@Override
	public void visit(Type type) {
		var typeObj = Tab.find(type.getName());
		type.struct = typeObj.getType();

		if (typeObj == Tab.noObj || typeObj.getKind() != Obj.Type)
			reportError("Can't resolve type <" + type.getName() + ">", type);
	}

	@Override
	public void visit(ProgName progName) {
		progName.obj = Tab.insert(Obj.Prog, progName.getName(), Tab.noType);
		Tab.openScope();
	}
	
	@Override
	public void visit(Program prog) {
		var mainMethod = Tab.find("main");
		if (mainMethod == Tab.noObj || mainMethod.getKind() != Obj.Meth || mainMethod.getType() != Tab.noType || mainMethod.getLevel() != 0)
			reportError("Suitable main method not found", prog);

		Tab.chainLocalSymbols(prog.getProgName().obj);
    	Tab.closeScope();
	}
	
	@Override
	public void visit(NspName nspName) {
		currNsp = nspName.getName() + "::";
	}

	@Override
	public void visit(NspDecl nsp) {
		this.currNsp = "";
	}

	@Override
	public void visit(IntConst cnst) {
		cnst.struct = Tab.intType;
	}

	@Override
	public void visit(CharConst cnst) {
		cnst.struct = Tab.charType;
	}

	@Override
	public void visit(BoolConst cnst) {
		cnst.struct = Types.boolType;
	}
	
	@Override
	public void visit(VoidMethodSign methodSign) {
		if (Tab.currentScope.findSymbol(methodSign.getName()) != null) {
			reportError("Identifier " + methodSign.getName() + " redefined", methodSign);
			return;
		}
		
		methodSign.obj = currMethod = Tab.insert(Obj.Meth, currNsp + methodSign.getName(), Tab.noType);
		
		Tab.openScope();
	}
	
	@Override
	public void visit(ValueMethodSign methodSign) {
		if (Tab.currentScope.findSymbol(methodSign.getName()) != null) {
			reportError("Identifier " + methodSign.getName() + " redefined", methodSign);
			return;
		}

		methodSign.obj = currMethod = Tab.insert(Obj.Meth, currNsp + methodSign.getName(), methodSign.getType().struct);
		
		Tab.openScope();
	}
	
	@Override
	public void visit(MethodDecl methodDecl) {
		Tab.chainLocalSymbols(currMethod);
    	Tab.closeScope();
    	
    	currMethod = null;
	}
	
	@Override
	public void visit(ConstTypedDecl constTypedDecl) {
		Struct type = constTypedDecl.getType().struct;
		
		constTypedDecl.traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(ConstDecl constDecl) {
				if (Tab.currentScope.findSymbol(constDecl.getName()) != null) {
					reportError("Identifier " + constDecl.getName() + " redefined", constDecl);
					return;
				}

				var obj = Tab.insert(Obj.Con, currNsp + constDecl.getName(), type);
				constCount++;

				var cnst = constDecl.getConst();

				if (cnst.struct != type) {
					reportError("Incompatible types", constDecl);
					return;
				}

				int value = 0;
				if (type == Tab.intType)
					value = ((IntConst)(cnst)).getValue();
				else if (type == Tab.charType)
					value = ((CharConst)(cnst)).getValue();
				else if (type == Types.boolType)
					value = ((BoolConst)(cnst)).getValue() ? 1 : 0;
				
				obj.setAdr(value);
			}
		});
	}
	
	@Override
	public void visit(VarTypedDecl varTypedDecl) {
		varTypedDecl.traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(VarDeclOk varDecl) {
				if (Tab.currentScope.findSymbol(varDecl.getName()) != null) {
					reportError("Identifier " + varDecl.getName() + " redefined", varDecl);
					return;
				}

				if (currMethod != null)
					localVarCount++;
				else
					globalVarCount++;

				var fullName = currMethod == null ? currNsp + varDecl.getName() : varDecl.getName();
				var type = varDecl.getVarQuantity() instanceof ArrayVar ?
						new Struct(Struct.Array, varTypedDecl.getType().struct) :
						varTypedDecl.getType().struct;

				var obj = Tab.insert(Obj.Var, fullName, type);
				obj.setAdr(Tab.currentScope.getnVars() - 1);
			}
		});
	}

	@Override
	public void visit(FormParamOk formParam) {
		if (Tab.currentScope.findSymbol(formParam.getName()) != null) {
			reportError("Parameter " + formParam.getName() + " redefined", formParam);
			return;
		}

		var type = formParam.getVarQuantity() instanceof ArrayVar ?
				new Struct(Struct.Array, formParam.getType().struct) :
				formParam.getType().struct;

		Tab.insert(Obj.Var, formParam.getName(), type);
		currMethod.setLevel(currMethod.getLevel() + 1);
	}

	@Override
	public void visit(ActParam param) {
		param.struct = param.getExpr().struct;
	}

	@Override
	public void visit(Call call) {
		var funcObj = call.getDesignator().obj;
		if (funcObj.getKind() != Obj.Meth)
			reportError("Object is not a method", call);

		callsCount++;

		var args = new ArrayList<Struct>();
		call.getActPars().traverseBottomUp(new VisitorAdaptor() {
			private int inCalls;

			@Override
			public void visit(CallBefore callBefore) {
				inCalls++;
			}

			@Override
			public void visit(Call call) {
				inCalls--;
			}

			@Override
			public void visit(ActParam param) {
				if (inCalls != 0)
					return;

				args.add(param.struct);
			}
		});

		if (args.size() != funcObj.getLevel()) {
			reportError("Wrong number of arguments", call);
			return;
		}

		var params = funcObj.getLocalSymbols().stream().limit(funcObj.getLevel()).toList();
		for (var i = 0; i < params.size(); i++) {
			var arg = args.get(i);
			var param = params.get(i);

			if (!arg.assignableTo(param.getType())) {
				reportError("Incompatible argument type", call);
				return;
			}
		}

		call.struct = funcObj.getType();
	}

	@Override
	public void visit(ConstFactor factor) {
		factor.struct = factor.getConst().struct;
	}

	@Override
	public void visit(GroupedFactor factor) {
		factor.struct = factor.getExpr().struct;
	}

	@Override
	public void visit(NewArrayFactor factor) {
		factor.struct = new Struct(Struct.Array, factor.getType().struct);
	}

	@Override
	public void visit(CallFactor factor) {
		factor.struct = factor.getCall().struct;
	}

	@Override
	public void visit(DesignatorFactor factor) {
		factor.struct = factor.getDesignator().obj.getType();
	}

	@Override
	public void visit(FactorTerm term) {
		term.struct = term.getFactor().struct;
	}

	@Override
	public void visit(FactorTermList termList) {
		if (termList.getFactor().struct != termList.getTerm().struct || termList.getTerm().struct != Tab.intType) {
			reportError("Incompatible types in expression", termList);
			return;
		}

		termList.struct = Tab.intType;
	}

	@Override
	public void visit(ExprTerm term) {
		term.struct = term.getTerm().struct;
	}

	@Override
	public void visit(ExprTermList termList) {
		if (termList.getTerm().struct != termList.getExpr().struct || termList.getExpr().struct != Tab.intType)
			reportError("Incompatible types in expression", termList);

		termList.struct = Tab.intType;
	}

	@Override
	public void visit(ExprNegTerm term) {
		if (term.getTerm().struct != Tab.intType)
			reportError("Incompatible types in expression", term);

		term.struct = Tab.intType;
	}

	@Override
	public void visit(ValueReturnStmt stmt) {
		if (currMethod == null)
			reportError("Return statement outside of method", stmt);

		if (currMethod.getType() != stmt.getExpr().struct)
			reportError("Incompatible return type", stmt);
	}

	@Override
	public void visit(VoidReturnStmt stmt) {
		if (currMethod == null)
			reportError("Return statement outside of method", stmt);

		if (currMethod.getType() != Tab.noType)
			reportError("Returning value from void method", stmt);
	}

	@Override
	public void visit(BlankVarRef varRef) {
		var localObj = Tab.find(currNsp + varRef.getVarName());
		if (localObj == Tab.noObj)
			localObj = Tab.find(varRef.getVarName());

		varRef.obj = localObj;

		if (localObj == Tab.noObj)
			reportError("Identifier " + varRef.getVarName() + " not found", varRef);

		if (localObj.getLevel() > 0 && localObj.getAdr() < currMethod.getLevel())
			argUsageCount++;

		var sb = new StringBuilder();
		sb.append("Found identifier ");
		sb.append(varRef.getVarName());
		sb.append(" at line ");
		sb.append(varRef.getLine());
		sb.append(": ");
		varRef.obj.accept(ObjPrinter.getInstance());
		sb.append(ObjPrinter.getInstance().getOutput());
		System.out.println(sb);
	}


	@Override
	public void visit(NspVarRef varRef) {
		varRef.obj = Tab.find(varRef.getNspName() + "::" + varRef.getVarName());

		if (varRef.obj == Tab.noObj)
			reportError("Identifier " + varRef.getVarName() + " not found", varRef);

		var sb = new StringBuilder();
		sb.append("Found identifier ");
		sb.append(varRef.getVarName());
		sb.append(" at line ");
		sb.append(varRef.getLine());
		sb.append(": ");
		varRef.obj.accept(ObjPrinter.getInstance());
		sb.append(ObjPrinter.getInstance().getOutput());
		System.out.println(sb);
	}

	@Override
	public void visit(ArrayElemAccess elemAccess) {
		if (elemAccess.getExpr().struct != Tab.intType)
			reportError("Array index not of type int", elemAccess);

		arrAccessCount++;
	}

	@Override
	public void visit(Designator designator) {
		designator.obj = designator.getVarRef().obj;

		if (designator.getElemAccess() instanceof ArrayElemAccess) {
			if (designator.obj.getType().getKind() == Struct.Array)
				designator.obj = new Obj(Obj.Elem, "", designator.obj.getType().getElemType());
			else
				reportError("Object is not an array", designator);
		}
	}

	@Override
	public void visit(DesAssignStmt stmt) {
		var obj = stmt.getDesignator().obj;

		switch (obj.getKind()) {
		case Obj.Var:
		case Obj.Elem:
			break;
		default:
			reportError("Object is not a variable", stmt);
			return;
		}

		if (!stmt.getExpr().struct.assignableTo(obj.getType()))
			reportError("Incompatible types for assignment", stmt);
	}

	@Override
	public void visit(DesIncStmt stmt) {
		var obj = stmt.getDesignator().obj;

		switch (obj.getKind()) {
		case Obj.Var:
		case Obj.Elem:
			break;
		default:
			reportError("Object is not a variable", stmt);
			return;
		}

		if (obj.getType() != Tab.intType)
			reportError("Incompatible type for increment", stmt);
	}

	@Override
	public void visit(DesDecStmt stmt) {
		var obj = stmt.getDesignator().obj;

		switch (obj.getKind()) {
		case Obj.Var:
		case Obj.Elem:
			break;
		default:
			reportError("Object is not a variable", stmt);
			return;
		}

		if (obj.getType() != Tab.intType)
			reportError("Incompatible type for increment", stmt);
	}

	@Override
	public void visit(DesUnpackStmt stmt) {
		if (stmt.getDesignator().obj.getType().getKind() != Struct.Array) {
			reportError("Last element is not an array", stmt);
			return;
		}

		if (stmt.getDesignator1().obj.getType().getKind() != Struct.Array) {
			reportError("Unpacked element is not an array", stmt);
			return;
		}

		var elemType = stmt.getDesignator1().obj.getType().getElemType();

		if (!elemType.assignableTo(stmt.getDesignator().obj.getType().getElemType())) {
			reportError("Incompatible types for unpacking", stmt);
			return;
		}

		stmt.getDesUnpackList().traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(DesOptDes receiverDes) {
				var receiverObj = receiverDes.getDesignator().obj;
				switch (receiverObj.getKind()) {
				case Obj.Var:
				case Obj.Elem:
					break;
				default:
					reportError("Object is not a variable", receiverDes);
					return;
				}

				if (!elemType.assignableTo(receiverObj.getType()))
					reportError("Incompatible types for unpacking", receiverDes);
			}
		});
	}

	@Override
	public void visit(CondFactExpr expr) {
		if (expr.getExpr().struct != Types.boolType)
			reportError("Expression not of type bool", expr);
	}

	@Override
	public void visit(Comparison comp) {
		var exprType = comp.getExpr().struct;
		if (!exprType.compatibleWith(comp.getExpr1().struct))
			reportError("Incompatible types in comparison", comp);

		if (exprType.isRefType()) {
			if (!(comp.getRelop() instanceof EqualityRelop || comp.getRelop() instanceof InequalityRelop))
				reportError("Can't compare references", comp);
		}
	}

	@Override
	public void visit(ReadStmt stmt) {
		var obj = stmt.getDesignator().obj;

		switch (obj.getKind()) {
		case Obj.Var:
		case Obj.Elem:
			break;
		default:
			reportError("Object is not a variable", stmt);
			return;
		}

		if (obj.getType() != Tab.intType && obj.getType() != Tab.charType && obj.getType() != Types.boolType)
			reportError("Incompatible type for read", stmt);
	}

	@Override
	public void visit(PrintStmt stmt) {
		var expr = stmt.getExpr();
		if (expr.struct != Tab.intType && expr.struct != Tab.charType && expr.struct != Types.boolType)
			reportError("Incompatible type for print", stmt);
	}

	@Override
	public void visit(ForSign forSign) {
		loopCnt++;
	}

	@Override
	public void visit(ForStmt stmt) {
		loopCnt--;
	}

	@Override
	public void visit(ContinueStmt stmt) {
		if (loopCnt == 0)
			reportError("Continue statement outside of loop", stmt);
	}

	@Override
	public void visit(BreakStmt stmt) {
		if (loopCnt == 0)
			reportError("Break statement outside of loop", stmt);
	}
}