package rs.ac.bg.etf.pp1;

import java.io.BufferedReader;
import java.io.FileReader;
import rs.ac.bg.etf.pp1.ast.Program;
import rs.etf.pp1.symboltable.Tab;
import rs.etf.pp1.symboltable.concepts.Obj;
import rs.etf.pp1.symboltable.concepts.Struct;
import rs.etf.pp1.symboltable.visitors.DumpSymbolTableVisitor;

public class Compiler {
	public static void main(String[] args) throws Exception {
		if (args.length < 2) {
			System.err.println("Not enough arguments supplied! Usage: MJParser <source-file> <obj-file> ");
			return;
		}
		
		try (var br = new BufferedReader(new FileReader(args[0]))) {
			var parser = new Parser(new Yylex(br));

	        var root = parser.parse();
			if (!(root.value instanceof Program) || parser.hasError())
				return;

	        var program = (Program)(root.value);
	        
			Tab.init();
			Tab.insert(Obj.Type, "bool", Types.boolType);
			var semanticAnalyzer = new SemanticAnalyzer();
			program.traverseBottomUp(semanticAnalyzer);

			if (semanticAnalyzer.hasError())
				return;

			Tab.dump(new DumpSymbolTableVisitor() {
				@Override
				public void visitStructNode(Struct structToVisit) {
					if (structToVisit.getKind() == Struct.Bool)
						output.append("bool");
					else
						super.visitStructNode(structToVisit);
				}
			});
	        	
        	/*File objFile = new File(args[1]);
        	if (objFile.exists())
        		objFile.delete();
        	
        	Code generation...
        	CodeGenerator codeGenerator = new CodeGenerator();
        	prog.traverseBottomUp(codeGenerator);
        	Code.dataSize = semanticAnalyzer.nVars;
        	Code.mainPc = codeGenerator.getMainPc();
        	Code.write(new FileOutputStream(objFile));*/
		}
	}
}
