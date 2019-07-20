#include "HistoryRecord.hpp"

std::ostream &operator<<(std::ostream &out, const HistoryRecord& t){
    out <<"<";
    if (t.operation==INSERT) out <<"INSERT:";
    if (t.operation==DELETE) out <<"DELETE:";
    if (t.operation==REVERSE) out <<"REVERSE:";
    out << t.entry <<">";
    return out;
};
/* TO-DO: method implementations below */



HistoryRecord::HistoryRecord(){
    //entry=Entry();
    
    
}




    /*constructor*/
HistoryRecord::HistoryRecord(Operation oper, Entry e){
    operation=oper;
    entry=e;
    
}



    /*returns one of the Operations: INSERT, DELETE or DELETE*/
Operation HistoryRecord::getOperation() const{
    return operation;
    
}




    /*returns the playlist entry for the corresponding history record*/
Entry HistoryRecord::getEntry() const{
    return entry;
}