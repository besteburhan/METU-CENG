package hw1;

import java.util.ArrayList;
/** import java.io.*; */ 

public class Document implements DocElement{
    private String title;
    private ArrayList<DocElement> docElementList;

    public Document(String title){
        this.setTitle(title);
        this.docElementList = new ArrayList<DocElement>();
    }
    public ArrayList<DocElement> getElements(){
        /*  a getter of all the elements, in the order they were added */
        return docElementList;
    }
    public void setElements(ArrayList<DocElement> arr){
        this.docElementList = new ArrayList<DocElement>();
        for(DocElement de : arr)
            this.docElementList.add(de);
            /**??? shallow copy or deep copy */
        System.out.println("Document setElements: " + this.docElementList);
    }
    public void add(DocElement de){
        this.docElementList.add(de);

    }
    public void setTitle(String title){
        this.title = title;
    }
    public String getTitle(){
        return this.title;
    } 
    @Override
    public <T> T accept(TextVisitor<T> visitor) {
        return visitor.visit(this);
    }
}