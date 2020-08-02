package hw1;

public class XDotYDivXIsYRule implements Rule{
    private Var x;
    private Var y;
    private MathExpression premise;
    private MathExpression entail;
    public XDotYDivXIsYRule(Var x, Var y){
        this.x = x;
        this.y = y;
        this.premise = new Op("/", new Op("*", x, y), x);
        this.entail = y;
    }
    public Var getX(){
        return this.x;
    }
    public Var getY(){
        return this.y;
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