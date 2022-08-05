//
// Created by charl on 8/4/2022.
//
#include "getTimeStamp.h"

uint64_t getTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * kUsToS + tv.tv_usec;
}