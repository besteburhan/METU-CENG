#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "Node.hpp"

/*You are free to add private/public members inside this template..*/
template <class T>
class MyStack{
    private:
    Node<T> *top;
;
    
    public:
        /*Default constructor*/
    MyStack();                                      
        /*copy constructor*/
    MyStack(const MyStack<T>& rhs);                 
        /*destructor*/
    ~MyStack(); 
        /*overloaded = operator*/
    MyStack<T>& operator=(const MyStack<T>& rhs);   
        /*returns true if stack is empty*/
    bool isEmpty() const;
        /*push newItem to stack*/
    void push(const T& newItem);
        /*pop item from stack*/
    void pop();
        /*return top item of the stack*/
    Node<T>* Top() const;
        /*Prints the stack entries. This method was already implemented. Do not modify.*/
    void print() const;

    void clear() const;
   

    
};

template <class T>
void MyStack<T>::print() const{
    const Node<T>* node = top;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/* TO-DO: method implementations below */

template<class T>
MyStack<T>::MyStack(){
    top =NULL;

}

template<class T>
MyStack<T>::MyStack(const MyStack<T> & rhs){
    top= new Node<T>;
    *this=rhs; 
}

template<class T>
MyStack<T>::~MyStack(){
    Node<T>* prev = top;
    while(prev->getNext() )
            {   
                Node<T>* temp = prev->getNext();
                prev->setNext(temp->getNext());
                delete temp;
            }
       
        delete prev;
    
}

template<class T>
MyStack<T>& MyStack<T>::operator=(const MyStack<T>& rhs){
    if(this != &rhs){
        clear();
        
        Node<T> *newNode= new Node<T>(rhs.Top()->getData());
        top= newNode;
        Node<T> *temp=top;
        Node<T> *r = rhs.Top()->getNext();
      
        
        while (r) {
            Node<T> *newNode= new Node<T>(r->getData());
            temp->setNext(newNode);
            temp=temp->getNext();
            r=r->getNext();
            
        }
    }
    return *this;
}

template<class T>
bool MyStack<T>::isEmpty() const{
    return top==NULL;

}

template<class T>
void MyStack<T>::push(const T& newItem){
    Node<T> *newNode= new Node<T>(newItem);
    Node<T> *t =top;
    
    if(top==NULL) {
        
        top=newNode;

    }
    else{
        newNode->setNext(t);
        top=newNode;
    }
}

template<class T>
void MyStack<T>::clear() const{
    if(!(this->isEmpty())){
        Node<T>* prev=top;
        while(prev->getNext() )
            {   
                Node<T>* temp=prev->getNext();
                prev->setNext(temp->getNext());
                delete temp;
            }
        delete prev;
    }
}

template<class T>
void MyStack<T>::pop(){
    if(!(this->isEmpty())){
        Node<T>* temp=top;
        top=top->getNext();
        delete temp;
        
    }
}



template<class T>
Node<T>* MyStack<T>::Top() const{
    return top;
}



#endif /* MYSTACK_HPP */
















