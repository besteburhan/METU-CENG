package hw1;

public class Num implements MathExpression{
    private int value;
    public Num(int value){
        this.value = value;
    }
    public int getValue(){
        return this.value;
    }
    @Override
    public <T> T accept(MathVisitor<T> visitor) {
        return visitor.visit(this); 
    }
    @Override
    public boolean match(MathExpression me) {
        int val;
        if(me instanceof Num){
            val = ((Num) me).getValue();
            if (val == this.value)
                return true;
        }
        return false;
    }
}