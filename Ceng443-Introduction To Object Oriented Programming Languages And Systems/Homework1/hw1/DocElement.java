package hw1;

public interface DocElement{
    <T> T accept(TextVisitor<T> visitor);
}