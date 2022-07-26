#include <stdint.h>
#include <stdio.h>

void _sym_notify_basic_block(uintptr_t site_id) {
    FILE * f= fopen("coverage", "a");
    fprintf(f,"%lx\n",site_id);
    fclose(f);
}