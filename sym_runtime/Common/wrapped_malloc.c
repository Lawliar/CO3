/*
 * wrapped_malloc.c
 *
 *  Created on: Feb 24, 2023
 *      Author: lcm
 */

#include "wrapped_malloc.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

void* shelf[MAX_ALLOC];
unsigned numAlloc = 0;

void * w_malloc(int numBytes){
    if(numAlloc >= MAX_ALLOC){
        printf("Number of malloc exceed %d\n", MAX_ALLOC);
        return (void*)NULL;
    }
    void *c = malloc(numBytes);
    if(c != NULL && c != (void *) -1){
        // malloc succeeds
        bool foundVacant = false;
        int i = 0;
        for(; i < MAX_ALLOC ; i++){
            if( shelf[i] == NULL){
                foundVacant = true;
                shelf[i] = c;
                numAlloc++;
                break;
            }
        }
        if(! foundVacant){
            free(c);
            printf("There should be vacant slot while there is not");
            return (void*)NULL;
        }
        return c;
    }else{
        printf("malloc failed");
        return (void*)NULL;
    }
}
void w_free(void * ptr){
    bool found = false;
    int i = 0;
    for(; i < MAX_ALLOC ; i++){
        if(shelf[i] == ptr){
            found = true;
            free(ptr);
            shelf[i] = NULL;
            numAlloc--;
            break;
        }
    }
    if( ! found ){
        printf("freed ptr is not book kept");
    }
}

void whole_free(){
    int i = 0;
    for(; i < MAX_ALLOC ; i++){
        if(shelf[i] != NULL){
            free(shelf[i]);
            shelf[i] = NULL;
            numAlloc--;
        }
    }
    if(numAlloc  != 0){
        printf("freed ptrs and the number do not match up\n");
    }
}
