//
// Created by charl on 8/4/2022.
//
#include "getTimeStamp.h"

const uint64_t kUsToS = 1000000;
uint64_t getTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * kUsToS + tv.tv_usec;
}