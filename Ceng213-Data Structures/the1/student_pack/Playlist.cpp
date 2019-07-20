#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Playlist.hpp"
#include "Entry.hpp"

using namespace std;


Playlist::Playlist()
{
    srand(15);
}

int Playlist::getRandomNumber(int i, int n) const
{
    int range=n-i;
    int random = rand() % range + i;
    return random;
}

void Playlist::print()
{
    cout << "[PLAYLIST SIZE=" << entries.getSize() <<"]";
    entries.print();
}
void Playlist::printHistory()
{
    cout<<"[HISTORY]";
    history.print();
}

/* TO-DO: method implementations below */


    
    /*load the list of playlist entries from the given file.*/
void Playlist::load(std::string fileName){
    
    ifstream file;
    string line;
    file.open(fileName.c_str());
    while (file.good())
    {
        getline(file, line);
        string del=";";
        string title=line.substr(0,line.find(del));
        line.erase(0,line.find(del)+1);
        string genre=line.substr(0,line.find(del));
        line.erase(0,line.find(del)+1);
        string year=line;
        
        Entry entry = Entry(title,genre,year);
        insertEntry(entry);
        
    }
}
    /*Inserts a new entry to the end of playlist entries. 
     *For UNDO operation, you should save the insert operation.*/
void Playlist::insertEntry(const Entry &e){
    
    entries.insertNode(entries.getTail(),e);
    HistoryRecord h= HistoryRecord(INSERT,e);
    history.push(h);
    
}
    
    
    /*Deletes the entry with given title from the list. 
     *If the delete operation is successful (i.e. the entry with given title is in the playlist and deleted successfully)..
     *you should save the this  operation for UNDO operation.*/
void Playlist::deleteEntry(const std::string &_title){
    string t=_title;
    Entry e= Entry(t);
    Node<Entry>* node=entries.findNode(e);
    e= node->getData();
    Node<Entry>* nodePrev=entries.findPrev(e);
    entries.deleteNode(nodePrev);
    HistoryRecord h = HistoryRecord(DELETE,e);
    history.push(h);
    
}
    
    
    /*Moves the entry with given title to the left.*/
void Playlist::moveLeft(const std::string &title){
    Entry entry=Entry(title);
    Node<Entry> *prev = entries.findPrev(entry);
    Node<Entry> *point =entries.findNode(entry);
    entry = point->getData();
    if(prev!=NULL){
        Entry prevEntry = prev->getData(); 
        entries.deleteNode(entries.findPrev(prev->getData()));
        entries.insertNode(point,prevEntry);
    }
}
    
    
    /*Moves the entry with given title to the right.*/
void Playlist::moveRight(const std::string &title){
    Entry entry=Entry(title);
    Node<Entry>* point = entries.findNode(entry);
    Node<Entry>* after = point->getNext();
    
    moveLeft(after->getData().getTitle());
    
}
    
    
    /*Reverse the playlist entries.*/
void Playlist::reverse(){
    MyStack<Entry>* stack1 = new MyStack<Entry>;
    Node<Entry>* node1 = entries.getHead();
    for( ; node1!=NULL ; node1 = node1->getNext()){
        stack1->push(node1->getData());
    }
    Node<Entry>* entryHead = entries.getHead();
    while(entryHead->getNext()) entries.deleteNode(entryHead);
    
    node1 = stack1->Top();
    
    for( ; node1!=NULL ; node1 = node1->getNext()){
        
        entries.insertNode(entries.getTail(),node1->getData());
    }
    delete stack1;
    entries.deleteNode(NULL);
    HistoryRecord h=HistoryRecord(REVERSE);
    history.push(h);
    
}
    
    
    /*Sort the entries of the playlist from lowest to highest according to their “title”s.*/
void Playlist::sort(){
    Node<Entry>* node1 = entries.getHead(); 
    Node<Entry>* node2 = node1->getNext();
    Node<Entry>* min = node1;
    for( ;  node1!=NULL  ;  node1=node1->getNext()){
       
        min = node1;
        node2 = node1->getNext();
        for( ;  node2!=NULL  ;  node2=node2->getNext()){
            if(min->getData().getTitle() > node2->getData().getTitle()) min = node2;
             
        }
        if(!(node1->getData()==min->getData())){
            Entry node1Entry = node1->getData();
            Entry minEntry = min->getData();
            Node<Entry>* node1Prev = entries.findPrev(node1Entry);
            entries.deleteNode(node1Prev);
            entries.insertNode(entries.findNode(min->getData()),node1Entry);
            entries.deleteNode(entries.findPrev(minEntry));
            entries.insertNode(node1Prev,minEntry);
            node1=entries.findNode(minEntry);

        }
    }
}
    
    
    /*Merge the sorted lists while keeping the sort order*/
void Playlist::merge(const Playlist & pl){
    
    Node<Entry>* node1= pl.entries.getHead();
    
    for ( ; node1!=NULL ; node1=node1->getNext() ){
        insertEntry(node1->getData());
        
    }
    sort();
    
}
    
    
    /*Shuffle the playlist entries. Use getRandomNumber function to generate a random number. */
void Playlist::shuffle(){
     Node<Entry>* node1 = entries.getHead(); 
    Node<Entry>* node2 ;
    int randNum,i=0;
    for( ;   i<entries.getSize()-1  ;  node1=node1->getNext() , i++){
        randNum = getRandomNumber(i,entries.getSize());
        if(randNum!=i){
            node2=node1;
            for(int k=i ; k < randNum ; k++){
                node2 = node2->getNext();
            }
            //swap
            Entry node1Entry = node1->getData(); 
            Entry node2Entry = node2->getData();
            Node<Entry>* node1Prev = entries.findPrev(node1Entry);
            entries.deleteNode(node1Prev);
            entries.insertNode(entries.findNode(node2Entry),node1Entry);
            entries.deleteNode(entries.findPrev(node2Entry));
            entries.insertNode(node1Prev,node2Entry);
            
            node1 = entries.findNode(node2Entry);

            
            
        }
    }
}
    
    
    /*UNDO the the operations*/
void Playlist::undo(){
    HistoryRecord h = history.Top()->getData();
    history.pop();
    if(h.getOperation()==REVERSE) {
        
        reverse();
        history.pop();
        
    }
    else if(h.getOperation()==INSERT){
        
        Entry e = h.getEntry();
        deleteEntry(e.getTitle());
        history.pop();
        
    }
    else if(h.getOperation()==DELETE){
        
        Entry e = h.getEntry();
        insertEntry(e);
        history.pop();
    }
}
    