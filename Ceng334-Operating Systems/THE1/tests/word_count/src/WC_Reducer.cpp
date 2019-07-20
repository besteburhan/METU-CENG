//
//  main.cpp
//  WC_Reducer
//
//  Created by Emre ISIKLIGIL.
//  Copyright (c) 2015 Emre ISIKLIGIL. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include "WordCount.h"
using namespace std;

wc_t words[1024];

int main(int argc, const char * argv[]) {
    char line[256];
    char *word, *cnt;
    int count;
    int fd = 0;
    /*stringstream ss;
    ss << getpid();
    string s = "Log_Reducer" + ss.str() + ".txt";
    ofstream os(s.c_str());
    */
    while(1) {
        if(readInput(line, fd, 256) == -1) {
            //os << "Got EOF" << endl;
            if(fd == 0) {
                int status = fcntl(2, F_GETFL);
                if(!(status & O_RDWR)) {
                    //os << "Continue reading from stderr" << endl;
                    fd = 2;
                    continue;
                }
                else {
                    //os << "stderr is not opened read_only. Stop reading" << endl;
                    break;
                }
            }
            else {
                //os << "Stop reading" << endl;
                break;
            }
        }
        //os << "Received: " << line << endl;
        word = strtok(line, " \n");
        if(word != NULL) {
            cnt = strtok(NULL, " \n");
            count = atoi(cnt);
            incWordCount(word, count, words);
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
    
    //sleep(10);
    return 0;
}
