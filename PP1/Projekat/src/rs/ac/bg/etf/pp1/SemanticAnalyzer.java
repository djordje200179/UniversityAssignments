package rs.ac.bg.etf.pp1;

import rs.ac.bg.etf.pp1.ast.*;
import rs.etf.pp1.symboltable.Tab;
import rs.etf.pp1.symboltable.concepts.Obj;
import rs.etf.pp1.symboltable.concepts.Struct;

public class SemanticAnalyzer extends VisitorAdaptor {
	private boolean errorDetected = false;
	
	public boolean hasError() {
		return errorDetected;
	}

	public void reportError(String message, SyntaxNode info) {
		errorDetected = true;

		System.err.println(info != null ? message + " at line " + info.getLine() : message);
	}

	public void reportInfo(String message, SyntaxNode info) {
		System.out.println(info != null ? message + " at line " + info.getLine() : message);
	}
	
	private String currNsp = "";
	private Obj currMethod = null;

	@Override
	public void visit(Type type) {
		var typeObj = Tab.find(type.getName());
		type.struct = typeObj.getType();

		if (typeObj == Tab.noObj)
			reportError("Can't resolve type: " + type.getName(), type);
	}

	@Override
	public void visit(ProgName progName) {
		progName.obj = Tab.insert(Obj.Prog, progName.getName(), Tab.noType);
		Tab.openScope();
	}
	
	@Override
	public void visit(Program prog) {
		Tab.chainLocalSymbols(prog.getProgName().obj);
    	Tab.closeScope();
	}
	
	@Override
	public void visit(NspName nspName) {
		currNsp = nspName.getName() + "::";
	}

	@Override
	public void visit(Namespace nsp) {
		this.currNsp = "";
	}
	
	@Override
	public void visit(VoidMethodSign methodSign) {
		if (Tab.currentScope.findSymbol(methodSign.getName()) != null) {
			reportError("Identifier " + methodSign.getName() + " redefined", methodSign);
			return;
		}
		
		currMethod = Tab.insert(Obj.Meth, currNsp + methodSign.getName(), Tab.noType);
		
		Tab.openScope();
	}
	
	@Override
	public void visit(ValueMethodSign methodSign) {
		if (Tab.currentScope.findSymbol(methodSign.getName()) != null) {
			reportError("Identifier " + methodSign.getName() + " redefined", methodSign);
			return;
		}
		
		currMethod = Tab.insert(Obj.Meth, currNsp + methodSign.getName(), methodSign.getType().struct);
		
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
		var typeName = constTypedDecl.getType().getName();
		Struct type = constTypedDecl.getType().struct;
		
		constTypedDecl.traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(ConstDecl constDecl) {
				if (Tab.currentScope.findSymbol(constDecl.getName()) != null) {
					reportError("Identifier " + constDecl.getName() + " redefined", constDecl);
					return;
				}

				var obj = Tab.insert(Obj.Con, currNsp + constDecl.getName(), type);

				var cnst = constDecl.getConst();
				int value = 0;

				if (cnst instanceof IntConst) {
					if (type != Tab.intType) {
						reportError("Got: int, expected: " + typeName, cnst);
						return;
					}
					
					value = ((IntConst)(cnst)).getValue();
				} else if (cnst instanceof CharConst) {
					if (type != Tab.charType) {
						reportError("Got: char, expected: " + typeName, cnst);
						return;
					}
					
					value = ((CharConst)(cnst)).getValue();
				} else if (cnst instanceof BoolConst) {
					if (type != Types.boolType) {
						reportError("Got: bool, expected: " + typeName, cnst);
						return;
					}
					
					value = ((BoolConst)(cnst)).getValue() ? 1 : 0;
				}
				
				obj.setAdr(value);
			}
		});
	}
	
	@Override
	public void visit(VarTypedDecl varTypedDecl) {
		varTypedDecl.traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(VarDecl varDecl) {
				if (Tab.currentScope.findSymbol(varDecl.getName()) != null) {
					reportError("Identifier " + varDecl.getName() + " redefined", varDecl);
					return;
				}

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
	public void visit(FormParam formParam) {
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
}