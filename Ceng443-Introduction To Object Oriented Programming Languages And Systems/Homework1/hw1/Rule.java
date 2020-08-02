package hw1;

public interface Rule{
    default void clear(){
        MathExpression premise = this.getPremise();
        premise.accept(new ClearVarsVisitor());
        MathExpression entail = this.getEntails();
        entail.accept(new ClearVarsVisitor());
    }
    default boolean apply(MathExpression me){
        this.clear();
        boolean matched =  this.getPremise().match(me);
        if (!matched){
            this.clear();
        }
        return matched;

            
    }
    MathExpression getPremise();
    MathExpression getEntails();
    default MathExpression entails(MathExpression me){
        this.apply(me);
        return getEntails();
    }
}