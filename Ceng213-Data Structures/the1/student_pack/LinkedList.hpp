#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T> 
class LinkedList {
    private:
        /*First node of the linked-list*/
        Node<T>* head;
        /*Last node of the linked-list*/
        Node<T>* tail;
        /*size of the linked-list*/
        size_t  size; 
    public:

        LinkedList();
        LinkedList(const LinkedList<T>& ll);
        LinkedList<T>& operator=(const LinkedList<T>& ll);
        ~LinkedList();

        /* Return head of the linked-list*/
        Node<T>* getHead() const;
        /* Set head of the linked-list*/
        void setHead(Node<T>* n);
        /* Return tail of the linked-list*/
        Node<T>* getTail() const;
        /* Set tail of the linked-list*/
        void setTail(Node<T>* n);
        /* Get the previous node of the node that contains the data item. 
         * If the head node contains the data item, this method returns NULL.*/
        Node<T>* findPrev(const T& data) const;
        /* Get the node that stores the data item. 
         * If data is not found in the list, this function returns NULL.*/
        Node<T>* findNode(const T& data) const;
        /* Insert a new node to store the data item. 
         * The new node should be placed after the “prev” node. 
         * If prev is NULL then insert new node to the head.*/
        void insertNode(Node<T>* prev, const T& data); 
        /* This method is used to delete the node that is next to “prevNode”. 
         * PS:prevNode is not the node to be deleted. 
         * If prev is NULL then delete head node. 
         */
        void deleteNode(Node<T>* prevNode);  
        /* This method is used to clear the contents of the list.*/
        void clear();
        /* This method returns true if the list empty, otherwise returns false.*/
        bool isEmpty() const;
        /* This method returns the current size of the list. */
        size_t getSize() const;
        /*Prints the list. This method was already implemented. Do not modify.*/
        void print() const;
};

template <class T>
void LinkedList<T>::print() const{
    const Node<T>* node = head;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template<class T>
LinkedList<T>::LinkedList(){
    head=NULL;
    tail=NULL;
    size=0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll){
    head = new Node<T>;
    tail=head;
    size=ll.getSize();
    *this = ll;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
    if(this != &ll){
        clear();
        if(ll.getHead()==NULL) return *this;
        Node<T> *newNode = new Node<T>(ll.getHead()->getData());
        head = newNode;
        Node<T> *temp=head;
        Node<T> *r = ll.getHead()->getNext();
        while (r) {
            Node<T> *newNode = new Node<T>(r->getData());
            temp->setNext(newNode);
            temp=temp->getNext();
            r=r->getNext();
            
        }
    }
    return *this;
}

template<class T>
LinkedList<T>::~LinkedList(){
    clear();
   
}

template <class T>
Node<T>* LinkedList<T>::getHead() const{
    return head;
}

template <class T>
void LinkedList<T>::setHead(Node<T>* n){
    head = n;
}
template <class T>
Node<T>* LinkedList<T>::getTail() const{
    return tail;
}
template <class T>
void LinkedList<T>::setTail(Node<T>* n){
    tail = n;
}
template<class T>
Node<T>* LinkedList<T>::findNode(const T& data) const{

    Node<T>* p = head;

    while (p){
        if (p->getData() == data){
            return p;
        }
        p = p->getNext();
    }

    return NULL;
}

template<class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const{
    Node<T>* p = head;
    if(p->getData() == data) return NULL;

    while (p->getNext()){
        if (p->getNext()->getData() == data){
            return p;
        }
        p = p->getNext();
    }

    return NULL;
}
template<class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data){

    Node<T>* newNode= new Node<T>(data);
    if(prev==NULL) {
        newNode->setNext(head);// bak
        head=newNode;
    }
    else{
        newNode->setNext(prev->getNext());
        prev->setNext(newNode);
    }
    size++;
    Node<T>* t = head;
    for(int i=1;i<size;i++){
            t=t->getNext();
    }
    tail=t;
    
} 

template<class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
    size--;
    Node<T>* p = prevNode;
    if (p != NULL && p->getNext()){
        Node<T>* tmp = p->getNext();
        p->setNext(tmp->getNext());
        delete tmp;
    }
    else if(p == NULL) {
        Node<T>* tmp = head;
        head=head->getNext();
        delete tmp;
    }
    
    if(head){
        Node<T>* t = head;
        for(int i=1;i<size;i++){
            t=t->getNext();
        }
        tail=t;
    }
}
template<class T>
void LinkedList<T>::clear(){
    if(!(this->isEmpty())){

        Node<T>* prev = head;

        while(prev->getNext() )
            {   
                Node<T>* temp=prev->getNext();
                prev->setNext(temp->getNext());
                delete temp;
            }
       
        delete prev;
       

  
    }
    size=0;
}

template<class T>
size_t LinkedList<T>::getSize() const{
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const{
    return head==NULL;
}

/* end of your implementations*/

#endif