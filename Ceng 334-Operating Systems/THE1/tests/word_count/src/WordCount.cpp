//
//  WordCount.cpp
//  WC_Mapper
//
//  Created by Emre ISIKLIGIL on 18/03/15.
//  Copyright (c) 2015 Emre ISIKLIGIL. All rights reserved.
//

#include "WordCount.h"
#include <unistd.h>
#include <cstring>

void incWordCount(char *word, int inc ,wc_t *words) {
    for(int i=0; i < 1024; i++) {
        if(words[i].word[0] == '\0') {
            strcpy(words[i].word, word);
            words[i].count += inc;
            break;
        }
        else if(!strcmp(words[i].word, word)) {
            words[i].count += inc;
            break;
        }
    }
}

int readInput(char *buffer, int fd, int size) {
    char c;
    int num = 0;
    long len;
    
    len = read(fd, &c, 1);
    while(len > 0) {
        buffer[num++] = c;
        if(c == '\n') {
            buffer[num++] = '\0';
            return num;
        }
        
        len = read(fd, &c, 1);
    }
    
    return -1;
}
