package hw1;

public class XDotZeroIsZeroRule implements Rule{
    private Var x;
    private MathExpression premise;
    private MathExpression entail;
    public XDotZeroIsZeroRule(Var x){
        this.x = x;
        this.premise = new Op("*", x, new Num(0));
        this.entail = new Num(0);
    }
    public Var getX(){
        return this.x;
    }
    @Override
    public MathExpression getPremise(){
        return this.premise;
    }
    @Override
    public MathExpression getEntails(){
        return this.entail;
    }
}