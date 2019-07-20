#include "Blockchain.h"
#include "Utilizer.h"
#include <iostream>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/


   
Blockchain& Blockchain::operator=(Blockchain&& rhs) noexcept{

  if(this != &rhs){
      delete head;
      Koin * newKoinsPtr = new Koin(rhs.head->getValue());
      this->head = newKoinsPtr;
      Koin * copyKoin = rhs.head->getNext();
      while(copyKoin){
        Koin * newKoin = new Koin(copyKoin->getValue());
        newKoinsPtr->setNext(newKoin);
        copyKoin = copyKoin->getNext();
        newKoinsPtr=newKoinsPtr->getNext();
        newKoin = nullptr;
        delete newKoin;
      }
      newKoinsPtr =nullptr;
      delete newKoinsPtr;
    }
    rhs.head = nullptr;
    return *this;
  };
    


Blockchain::~Blockchain(){
	Koin * curr= this->head;
  this->head=nullptr;
  while(curr){
       	Koin * prev=curr;
       	curr=curr->getNext();
       	prev->setNext(nullptr);
        prev=nullptr;
        delete prev;
    }
};

int Blockchain::getID() const{
	return id;

};

Koin* Blockchain::getHead() const{
	return head;

};
void Blockchain::setHead(Koin *head){
      this->head = head;

    };

double Blockchain::getTotalValue() const{
	  double res=0;
     
    Koin *koins = this->head;
    
    if(this->head==nullptr) return 0;
       while(koins!= nullptr){
        
        	res += koins->getValue();
        	koins = koins->getNext();
        }
     return res;
};

long Blockchain::getChainLength() const{
  if(this->head==nullptr) return 0;

	long res= 0;
  Koin *koins = this->head;
   
  while(koins){

      res += 1;
      koins = koins->getNext();//??????????
    }
  return res;

};

void Blockchain::operator++(){
  Koin *newKoin = new Koin(Utilizer::fetchRandomValue());
  Koin* t = this->head;
  if(this->getHead()==nullptr) {this->head= newKoin; return;}
	
  while(t->getNext()) 
      t=t->getNext();

  t->setNext(newKoin);
  
};

   
void Blockchain::operator--(){
  if(this->getHead()==nullptr) return;
  if(this->getHead()->getNext()==nullptr){
    delete this->head;
    this->head = nullptr;
    return;
  }
    Koin* t = this->head;
    while(t->getNext()->getNext()) t=t->getNext();
    t->setNext(nullptr);
};

Blockchain& Blockchain::operator*=(int multiplier){
	Koin* koins = this->head;
  
    while(koins){
      
    	(*koins) *= multiplier;

       	koins= koins->getNext();
        }
        
   	return *this;

};

Blockchain& Blockchain::operator/=(int divisor){
	Koin* koins = this->head;
    while(koins){
    	*koins /= divisor;
       	koins= koins->getNext();
        }
   	return *this;
};

 std::ostream& operator<<(std::ostream& os, const Blockchain& blockchain){
    os << "Block "<< blockchain.id << ": " ;
    if(blockchain.getHead()==nullptr) {
      os << "Empty.";
      return os;
    }
    if(blockchain.getHead() != nullptr){
      os << *(blockchain.getHead()) ;}
    os << "(" << blockchain.getTotalValue() << ")";
    return os;
 };
