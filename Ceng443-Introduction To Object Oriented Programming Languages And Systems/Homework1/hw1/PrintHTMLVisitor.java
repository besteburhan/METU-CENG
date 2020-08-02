package hw1;

import java.util.ArrayList;

public class PrintHTMLVisitor implements TextVisitor<String>{

    @Override
    public String visit(Document document){
        ArrayList<DocElement> docElementList = document.getElements();
        String text = "<html><head><title>" + 
                    document.getTitle() + 
                    "</title></head><body>";
        for (DocElement de : docElementList )
            text += de.accept(new PrintHTMLVisitor());
        text += "</body></html>";
        return text;

    }
    @Override
    public String visit(EquationText equationText){
        MathExpression me = equationText.getInnerMath();
        String text = "<math>" + 
                        me.accept(new PrintMathMLVisitor()) +
                        "</math>";
        return text;
    }
    @Override
    public String visit(Paragraph paragraph){
        String text = "<p>" + paragraph.getText() + "</p>";
        return text;
    }
}