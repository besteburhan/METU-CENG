#include "Article.h"



Article::Article( int table_size,int h1_param, int h2_param )
{    
    
    n =0;
    this->table_size = table_size;
    this->h1_param = h1_param;
    this->h2_param = h2_param;
    table = new std::pair<std::string,int> [table_size] ;
    
    for(int i=0; i<table_size ; i++){
        table [i].first = EMPTY_KEY;
        table [i].second = EMPTY_INDEX;
        
    }
}

Article::~Article()
{
    
    delete [] table;
}

int Article::get( std::string key, int nth, std::vector<int> &path ) const
{
    
    if(n > table_size) return -1;
    int occurence = nth;
    int strIntVal = convertStrToInt(key);
    int h1,h2;
    int tableIndex=0;
    int i =0;
    h1=Article::h1(strIntVal);
    h2=Article::h2(strIntVal);
    
    
    while(occurence!=0 && path.size() < table_size-1 )
    {
        tableIndex = (h1+h2*i)%table_size;
        if(table[tableIndex].first==key) occurence--;
        
        if(i!=0 )
            path.push_back(tableIndex);
        if(table[tableIndex].first==EMPTY_KEY && table[tableIndex].second==EMPTY_INDEX ) return -1;
        i++;
    }
    if(table[tableIndex].first == key ) return table[tableIndex].second;
    else
        return -1;  //nth occurence not found;
}

int Article::insert( std::string key, int original_index )
{
    
    
    if(getLoadFactor()>MAX_LOAD_FACTOR) expand_table();
    int strIntVal = convertStrToInt(key);
    int h1,h2;
    int i=0;
    int tableIndex=0;
    h1=Article::h1(strIntVal);
    h2=Article::h2(strIntVal);
    while(true){
    
        tableIndex = (h1+h2*i)%table_size;
        if(table[tableIndex].first==EMPTY_KEY) {
            table[tableIndex].first=key;
            table[tableIndex].second = original_index;
            n++;
            return i;
        }
        else if(table[tableIndex].first == key && table[tableIndex].second > original_index)
        {
            int t = table[tableIndex].second;
            table[tableIndex].second = original_index;
            return i+insert(key,t);
        }
        else //if(table[tableIndex].first != key || (table[tableIndex].first == key && table[tableIndex].second < original_index))
        {
            i++;
        }
    }
}


int Article::remove( std::string key, int nth )
{
   
    if(n > table_size) return -1;
    int occurence = nth;
    int strIntVal = convertStrToInt(key);
    int h1,h2;
    int tableIndex=0;
    int i =0;
    h1=Article::h1(strIntVal);
    h2=Article::h2(strIntVal);
    int probe=0;
    
    while(occurence!=0 && probe < table_size-1 )
    {
        tableIndex = (h1+h2*i)%table_size;
        if(table[tableIndex].first==key) occurence--;
        if(i!=0)
            probe++;
       
        i++;
    }
    if(table[tableIndex].first == key ) 
    {
        table[tableIndex].first=EMPTY_KEY;
        table[tableIndex].second=MARKED_INDEX;
        n--;
        return probe;
    }
    else
        return -1;  //nth occurence not found;
    
}

double Article::getLoadFactor() const
{
    
    double t =n;
    double loadFactor = t/table_size;
    return loadFactor;
}

void Article::getAllWordsFromFile( std::string filepath )
{
   
    std::ifstream file;
    std::string line;
     file.open(filepath.c_str());
     int i;
     while (file.good())
     {
        getline(file, line);
        std::string del=" ";
        std::string word=line.substr(0,line.find(del));
        for( i=1; word!="";i++)
        {    
            insert(word,i);
            if(word==line) break;
            line.erase(0,line.find(del)+1);
            word=line.substr(0,line.find(del));
            
            
            
        }
        
    }
}

void Article::expand_table()
{
    std::pair<std::string,int> *p = table;
    int oldSize = table_size;
    int newSize= nextPrimeAfter(2*table_size);
    table_size= newSize;
    this->table = new std::pair<std::string,int> [table_size];
    n=0;
    for(int i=0; i<table_size ; i++){
        table [i].first = EMPTY_KEY;
        table [i].second = EMPTY_INDEX;
        
    }
    newSize--;
    while(!isPrime(newSize) && newSize !=0){
        newSize--;
    }
    h2_param=newSize;
    for(int i=0; i < oldSize ; i++)
    {
        if(p[i].first != EMPTY_KEY)
        {
            this->insert(p[i].first,p[i].second);
        }
    }
    
    delete []p;
}

int Article::hash_function( std::string& key, int i ) const
{
   
    int strIntVal = convertStrToInt(key);
    int h1,h2;
    int tableIndex=0;
    do
    {
        h1=Article::h1(strIntVal);
        h2=Article::h2(strIntVal);
        
        tableIndex = (h1+h2*i)%table_size;
        i++;
    }
    while(table[tableIndex].first!=EMPTY_KEY);
    return tableIndex;
}

int Article::h1( int key ) const
{
   
    int k = key;
    int h1=0;
    while(k){
        if(k%2)
            h1++;
        k=k/2;
    }
    h1 = h1*h1_param;

    return h1;
}

int Article::h2( int key ) const
{
    
    int h2 ;
    h2=key%h2_param;
    h2 = h2_param-h2;

    return h2;
}







