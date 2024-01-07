package rs.ac.bg.etf.pp1;

import rs.etf.pp1.symboltable.concepts.Struct;
import rs.etf.pp1.symboltable.visitors.DumpSymbolTableVisitor;

public class ObjPrinter extends DumpSymbolTableVisitor {
	private static final ObjPrinter instance = new ObjPrinter();

	public static ObjPrinter getInstance() {
		return instance;
	}

	private ObjPrinter() {
		super();
	}

	@Override
	public void visitStructNode(Struct structToVisit) {
		if (structToVisit.getKind() == Struct.Bool)
			output.append("bool");
		else
			super.visitStructNode(structToVisit);
	}

	@Override
	public String getOutput() {
		var res = super.getOutput();
		output = new StringBuilder();
		return res;
	}
}
