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
	private Obj currProg = null,  currMethod = null;
	
	@Override
	public void visit(ProgName progName) {
		currProg = Tab.insert(Obj.Prog, progName.getName(), Tab.noType);
		Tab.openScope();
	}
	
	@Override
	public void visit(Program prog) {
		Tab.chainLocalSymbols(currProg);
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
		
		var typeObj = Tab.find(methodSign.getType());
		if (typeObj == Tab.noObj) {
			reportError("Can't resolve type: " + methodSign.getType(), methodSign);
			return;
		}
		
		currMethod = Tab.insert(Obj.Meth, currNsp + methodSign.getName(), typeObj.getType());
		
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
		var typeObj = Tab.find(constTypedDecl.getType());
		if (typeObj == Tab.noObj) {
			reportError("Can't resolve type: " + constTypedDecl.getType(), constTypedDecl);
			return;
		}
		
		Struct type = typeObj.getType();
		
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
						reportError("Got: int, expected: " + typeObj.getName(), cnst);
						return;
					}
					
					value = ((IntConst)(cnst)).getValue();
				} else if (cnst instanceof CharConst) {
					if (type != Tab.charType) {
						reportError("Got: char, expected: " + typeObj.getName(), cnst);
						return;
					}
					
					value = ((CharConst)(cnst)).getValue();
				} else if (cnst instanceof BoolConst) {
					if (type != Types.boolType) {
						reportError("Got: bool, expected: " + typeObj.getName(), cnst);
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
		var typeObj = Tab.find(varTypedDecl.getType());
		if (typeObj == Tab.noObj) {
			reportError("Can't resolve type", varTypedDecl);
			return;
		}
		
		varTypedDecl.traverseBottomUp(new VisitorAdaptor() {
			@Override
			public void visit(VarDecl varDecl) {
				if (Tab.currentScope.findSymbol(varDecl.getName()) != null) {
					reportError("Identifier " + varDecl.getName() + " redefined", varDecl);
					return;
				}

				var fullName = currMethod == null ? currNsp + varDecl.getName() : varDecl.getName();
				
				var obj = Tab.insert(Obj.Var, fullName, typeObj.getType());
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

		var typeObj = Tab.find(formParam.getType());
		if (typeObj == Tab.noObj) {
			reportError("Can't resolve type: " + formParam.getType(), formParam);
			return;
		}

		Tab.insert(Obj.Var, formParam.getName(), typeObj.getType());
		currMethod.setLevel(currMethod.getLevel() + 1);
	}
}