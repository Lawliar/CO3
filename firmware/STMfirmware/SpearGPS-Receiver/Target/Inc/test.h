#include "gps.h"

uint8_t gps_init(gps_t* gh);
uint8_t gps_process(gps_t* gh, const void* data, size_t len);