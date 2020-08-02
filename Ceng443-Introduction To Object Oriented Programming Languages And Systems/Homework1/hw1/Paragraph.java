package hw1;

public class Paragraph implements DocElement{
    private String text;
    public Paragraph(String text){
        this.text = text;
    }
    public String getText(){
        return this.text;
    }
    @Override
    public <T> T accept(TextVisitor<T> visitor) {
        return visitor.visit(this); 
    }
}