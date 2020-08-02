package hw1;

public class CountAtomicsVisitor implements MathVisitor<Integer>{
    @Override
    public Integer visit(Op op) {
        int cnt = 0;
        cnt += op.getFirst().accept(new CountAtomicsVisitor());
        cnt += op.getSecond().accept(new CountAtomicsVisitor());
        return cnt;
    }  
    @Override
    public Integer visit(Num num) {
        return 1;
    }  
    @Override
    public Integer visit(Sym sym) {
        return 1;
    }  
    @Override
    public Integer visit(Var var) {
        return 1;
    }       
}