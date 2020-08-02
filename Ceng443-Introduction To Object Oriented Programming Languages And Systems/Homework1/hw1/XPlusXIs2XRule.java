package hw1;

public class XPlusXIs2XRule implements Rule{
    private Var x;
    private MathExpression premise;
    private MathExpression entail;
    public XPlusXIs2XRule(Var x){
        this.x = x;
        this.premise = new Op("+", x, x);
        this.entail = new Op("*", new Num(2), x);
    }
    public Var getX(){
        return this.x;
    }
    @Override
    public MathExpression getPremise(){
        return premise;
    }
    @Override
    public MathExpression getEntails(){
        return entail;
    }
}