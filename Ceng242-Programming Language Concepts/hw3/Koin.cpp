#include "Koin.h"
#include "Utilizer.h"
#include "iomanip"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
	Koin::Koin()= default;


    Koin::Koin(double value){
    	this->value= value;
    	this->next = nullptr;
    };
    Koin::Koin(const Koin& rhs){
    	this->value = rhs.getValue();
    	this->next = rhs.getNext();
    };
    Koin& Koin::operator=(const Koin& rhs){
    	this->value = rhs.getValue();
    	this->next =rhs.getNext();
        return *this;
    };
    Koin::~Koin()=default;
    double  Koin::getValue() const{
        
    	return this->value;
    };
    Koin* Koin::getNext() const{
    	return next;
    };
    void Koin::setNext(Koin *next){
    	this->next = next;

    };
    bool Koin::operator==(const Koin& rhs) const{
    	
        if(this->value-rhs.getValue()< Utilizer::doubleSensitivity() && (this->next)==(rhs.getNext()))
                return true;
        else return false;
    }
    bool Koin::operator!=(const Koin& rhs) const{
        return !(*this==rhs);
    };
    Koin& Koin::operator*=(int multiplier){
        this->value=value*multiplier;
        return *this;
    };
    Koin& Koin::operator/=(int divisor){
        this->value=value/divisor;
        return *this;
    };
    std::ostream& operator<<(std::ostream& os, const Koin& koin){

		os << std::setprecision(Utilizer::koinPrintPrecision()) << std::fixed << koin.value << "--" ;
		if(koin.next)
		{os << *(koin.next);
		}
		else 
		os << "|";
		return os;

    };
        