//
// Created by charl on 8/4/2022.
//
#include <stdint.h>
#include <sys/time.h>
#include "stddef.h"
#ifndef SYMBACKEND_GETTIMESTAMP_H
#define SYMBACKEND_GETTIMESTAMP_H



const uint64_t kUsToS = 1000000;

uint64_t getTimeStamp();
#endif //SYMBACKEND_GETTIMESTAMP_H