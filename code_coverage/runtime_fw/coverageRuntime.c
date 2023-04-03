#include <stdint.h>
#include <stdio.h>
uintptr_t prev_site_id = 0;
void _sym_notify_basic_block_cc(uintptr_t site_id) {
    //FILE * f= fopen("coverage", "a");
    //fprintf(f,"%lx\n",site_id ^ prev_site_id);
    //fprintf(f,"%lx\n",site_id);
    printf("%x\n",site_id ^ prev_site_id);
    prev_site_id = site_id >> 1;
}
