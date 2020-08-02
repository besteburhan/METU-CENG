package hw1;

public class Var implements MathExpression{
    private int id;
    private MathExpression previousMatch;
    public Var(int id){
        this.id = id;
        this.previousMatch = null;
    }
    public int getId(){
        return this.id;
    }
    public MathExpression getPreviousMatch(){
        return previousMatch;
    }
    public void setPreviousMatch(MathExpression me){
        this.previousMatch = me;
    }
    @Override
    public <T> T accept(MathVisitor<T> visitor) {
        return visitor.visit(this); 
    }
    @Override
    public boolean match(MathExpression me){
        if(this.previousMatch == null){
            this.setPreviousMatch(me);
            return true;
        }
        else{
            return previousMatch.match(me);
        }
    }
}