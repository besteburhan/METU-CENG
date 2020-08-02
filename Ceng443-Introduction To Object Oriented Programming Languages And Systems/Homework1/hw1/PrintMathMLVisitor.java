package hw1;

public class PrintMathMLVisitor implements MathVisitor<String>{
    @Override
    public String visit(Op op) {
        String operand = op.getOperand();
        MathExpression first = op.getFirst();
        MathExpression second = op.getSecond();
        String text;
        if(operand.equals("/")){
            text = "<mrow><mfrac>"+ first.accept(new PrintMathMLVisitor()) + 
                    second.accept(new PrintMathMLVisitor()) +
                    "</mfrac></mrow>";
        }
        else{
            text = "<mrow><mo>(</mo>"+ 
                first.accept(new PrintMathMLVisitor()) +
                "<mo>";
            if(operand.equals("+"))
                text += "+";
            else if(operand.equals("*"))
                text += "&times;" ;
            else if(operand.equals("|-"))
                text += "&vdash;";
            text += "</mo>" +
                second.accept(new PrintMathMLVisitor()) +
                "<mo>)</mo></mrow>";
        }
        return text;
    }  
    @Override
    public String visit(Num num) {
        String text = "<mrow><mn>"+ num.getValue() + "</mn></mrow>";
        return text;
    }  
    @Override
    public String visit(Sym sym) {
        String text = "<mrow><mi>"+ sym.getValue() + "</mi></mrow>";
        return text;
    }  
    @Override
    public String visit(Var var) {
        MathExpression me = var.getPreviousMatch();
        String text = "";
        if (me == null){
            text += "<mrow><msub><mi>V</mi><mn>" + var.getId() + 
                    "</mn></msub></mrow>";
        }
        else{
            text += "<mrow><msub><mi>V</mi><mn>"+ var.getId() +
                    "</mn></msub><mo>[</mo>"+ 
                    me.accept(new PrintMathMLVisitor()) +
                    "<mo>]</mo></mrow>";
        }

        return text;
    } 
}