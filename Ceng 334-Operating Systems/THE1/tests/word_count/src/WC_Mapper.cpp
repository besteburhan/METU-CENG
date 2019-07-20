//
//  main.cpp
//  WC_Mapper
//
//  Created by Emre ISIKLIGIL
//  Copyright (c) 2015 Emre ISIKLIGIL. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "WordCount.h"
using namespace std;

wc_t words[1024];


int main(int argc, const char * argv[]) {
    char line[256];
    char *word;
    /*stringstream ss;
    ss << getpid();
    string s = "Log_Mapper" + ss.str() + ".txt";
    ofstream os(s.c_str());
    */
    while(readInput(line, 0, 256) > 0) {
        //cout << "Received: " << line << endl;
        word = strtok(line, " \n");
        while(word != NULL) {
            incWordCount(word, 1, words);
            word = strtok(NULL, " \n");
        }
    }
    
    for(int i=0; i < 2014; i++) {
        if(words[i].word[0] == '\0') {
            break;
        }
        
        //os << "Sent: " << words[i].word << " " << words[i].count << endl;
        cout << words[i].word << " " << words[i].count << endl;
    }
    
    //os.close();
    return 0;
}
