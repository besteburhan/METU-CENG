package hw1;

public class ClearVarsVisitor implements MathVisitor<Void>{
    @Override
    public Void visit(Op op) {
        op.getFirst().accept(new ClearVarsVisitor());
        op.getSecond().accept(new ClearVarsVisitor());
        return null;
    }  
    @Override
    public Void visit(Num num) {
        return null;
    }  
    @Override
    public Void visit(Sym sym) {
        return null;
    }  
    @Override
    public Void visit(Var var) {
        var.setPreviousMatch(null);
        return null;
    }   
}