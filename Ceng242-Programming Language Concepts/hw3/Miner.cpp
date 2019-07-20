#include "Miner.h"
#include "Utilizer.h"
#include "iomanip"
#include <iostream>


/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

 	int Miner::getNextAvailableBlockchainID() const{
        if(blockchains.size()==0) return 0;
        return blockchains.back()->getID()+1;
    }
    Miner::Miner(std::string name){
        this->name = name;
    }
Miner::~Miner(){
        long s = this->getBlockchainCount();
        for(long i=0;i < s;i++){
            delete (blockchains[i]);
        }
    }
    
    void Miner::createNewBlockchain(){
        blockchains.push_back(new Blockchain(getNextAvailableBlockchainID()));
    }
void Miner::mineUntil(int cycleCount){
        while(cycleCount!=0){
            long s = this->getBlockchainCount();
            for(long i =0;i<s;i++){
                ++(*(blockchains[i]));
            }
            --cycleCount;
        }
    }
bool Miner::inSoftFork(int i){
        long s = sFork.size();
        long it =0;
        while(it!=s){
            if(sFork[it].second==i) return true;
            it++;
        }
        return false;
     }
bool Miner::inCreativeSoftFork(int i){
        long s = sFork.size();
        long it =0;
        while(it!=s){
            if(sFork[it].first==i) return true;
            it--;
        }
        return false;
     }
int Miner::createdSoftFork(int i){//oluşturduğu softforku döner
        long s = sFork.size();
        long it =0;
        while(it!=s){
            if(sFork[it].first==i) return sFork[it].second;
            it--;
        }
     }
void Miner::demineUntil(int cycleCount){
        while(cycleCount!=0){
            long s=this->getBlockchainCount();
            for(long i =0;i<s;i++){
                if(inCreativeSoftFork(i)){// bu blockchainden softforklanmış bir blockchain varsa
                    if(blockchains[createdSoftFork(i)]->getHead()==nullptr){
                        long s = sFork.size();
                        while(s!=0){
                        if(sFork[s-1].first==i) sFork.erase(sFork.begin()+s-2);
                        }
                        --(*(blockchains[i]));
                        continue;
                    }
                    else if(blockchains[createdSoftFork(i)]->getHead()->getNext()==nullptr)
                        return;
                }
                else
                    --(*(blockchains[i]));
                }
                --cycleCount;
            }
    }
    double Miner::getTotalValue() const{
        double res=0;
        long s = this->getBlockchainCount();
        for(long i =0;i< s ;i++){
            Blockchain *b =blockchains.at(i);
            res += ((*this)[i])->getTotalValue();
            }
        return res;
    }
    Blockchain* Miner::operator[](int id) const{
        long s = this->getBlockchainCount();
        if(id >= s || id<0) return nullptr;
        for(long i=0; i<s ; i++){
            if(id == blockchains[i]->getID()){
                Blockchain* b = blockchains[id];
                return b;
            }
        }
    }
    long Miner::getBlockchainCount() const{
        return blockchains.size();
    }

bool Miner::softFork(int blockchainID){
        if((blockchainID >= this->getBlockchainCount() || blockchainID <0)) return false;//2
        else{
            int nextAvailableBlockchainID = this->getNextAvailableBlockchainID();//3
            Blockchain* forkedBlockchain =(*this)[blockchainID];//1
            Koin* koins = forkedBlockchain->getHead();
            if(koins == nullptr){
                createNewBlockchain();
                return false;
            }
            while(koins->getNext()!=nullptr) koins = koins->getNext();
           
            this->blockchains.push_back(new Blockchain(nextAvailableBlockchainID,koins));
            
            std::pair<int, int> p1=  std::make_pair(blockchainID,nextAvailableBlockchainID);
            sFork.push_back(p1);
            return true;
        }

    }

bool Miner::hardFork(int blockchainID){
        if(blockchainID >= this->getBlockchainCount() || blockchainID <0) return false;
        else{
            int nextAvailableBlockchainID = this->getNextAvailableBlockchainID();
            Blockchain* forkedBlockchain =blockchains[blockchainID];
            Koin* koins = forkedBlockchain->getHead();
            if(koins == nullptr) {
                
                this->blockchains.push_back(new Blockchain(nextAvailableBlockchainID));
                return true;
                 }
            while(koins->getNext()!=nullptr) koins = koins->getNext();
            
            this->blockchains.push_back(new Blockchain(nextAvailableBlockchainID,new Koin(koins->getValue())));
            return true;
        }
    }
    
std::ostream& operator<<(std::ostream& os, const Miner& miner){
        os << "-- BEGIN MINER --"<<"\n";
        os <<"Miner name: "<< miner.name <<"\n";
        os << "Blockchain count: "<< miner.getBlockchainCount() << "\n";
        os <<std::setprecision(Utilizer::koinPrintPrecision()) << std::fixed << "Total value: " << miner.getTotalValue() << "\n";
        os << "\n";
        long c = miner.getBlockchainCount();
        long i=0;
        while(i!=c){
           
            os << *(miner[i]) << "\n";
            i++;
        }
        os << "\n";
        os << "-- END MINER --\n";
        return os;
    }



