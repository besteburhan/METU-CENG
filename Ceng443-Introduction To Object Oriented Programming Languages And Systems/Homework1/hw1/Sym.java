package hw1;

public class Sym implements MathExpression{
    private String value;

    public Sym(String value){
        this.value = value;
    }
    public String getValue(){
        return this.value;
    } 
    @Override
    public <T> T accept(MathVisitor<T> visitor) {
        return visitor.visit(this);
    }
    @Override
    public boolean match(MathExpression me) {
        String val;
        if(me instanceof Sym){
            val = ((Sym) me).getValue();
            if (val.equals(this.value))
                return true;
        }
        return false;
    }
}