package hw1;

public interface MathExpression {
    <T> T accept(MathVisitor<T> visitor);
    boolean match(MathExpression me);
}
