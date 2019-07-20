//
//  WordCount.h
//  WC_Mapper
//
//  Created by Emre ISIKLIGIL on 18/03/15.
//  Copyright (c) 2015 Emre ISIKLIGIL. All rights reserved.
//

#ifndef __WC_Mapper__WordCount__
#define __WC_Mapper__WordCount__

#include <stdio.h>

typedef struct {
    char word[256];
    int count;
} wc_t;

void incWordCount(char *word, int inc, wc_t *words);
int readInput(char *buffer, int fd, int size);

#endif /* defined(__WC_Mapper__WordCount__) */
