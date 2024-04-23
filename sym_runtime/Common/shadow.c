// This file is part of the CO3 runtime.
//
// The CO3 runtime is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The CO3 runtime is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CO3. If not, see <https://www.gnu.org/licenses/>.


#include "shadow.h"
#include <string.h>
#include <stdio.h>

typedef struct{
    uintptr_t addr;
    void * shadow;
} shadow_page;

typedef struct{
    shadow_page* mem;
    int size;
    int used;
} shadow_mem;

#define CO3ShadowArraySize 100
#define CO3RealPageSize 4096
#define CO3ShadowPageSize CO3RealPageSize >> 3


/// Compute the corresponding page address.
uintptr_t pageStart(uintptr_t addr) {
  return (addr & ~(CO3RealPageSize - 1));
}

/// Compute the corresponding offset into the page.
uintptr_t pageOffset(uintptr_t addr) {
  return (addr & (CO3RealPageSize - 1));
}


const unsigned initial_size = 2;
struct hashmap_s hashmap;

void initShadow(){
    if (0 != hashmap_create(initial_size, &hashmap)) {
        printf("hashmap_create failed\n");
        exit(1);
    }
}


char* access_shadow(char * addr) {
    uintptr_t addr_int = (uintptr_t)addr;
    uintptr_t pageStartAddr = pageStart((uintptr_t)addr);
    char ** p_addr = (char **)malloc(sizeof(char*));
    *p_addr = pageStartAddr;
    void * element = hashmap_get(&hashmap, (char*)p_addr, sizeof(char*));
    if(element == NULL){
        char* shadow_page = (char*)malloc(CO3ShadowPageSize);
        memset(shadow_page, 0, CO3ShadowPageSize);
        if(hashmap_put(&hashmap, (char*)p_addr, sizeof(char *), shadow_page) != 0){
            printf("allocating shadow page failed\n");
            exit(1);
        }
        element = shadow_page;
    }else{
        // p_addr is already allocated, we can free this one
        free(p_addr);
    }

    uintptr_t offset = pageOffset(addr_int) >> 3;
    return ((char*)element + offset) ;
}

int freeAll(void* const context, struct hashmap_element_s *const e) {
    free(e->key);
    free(e->data);
    return -1;
}
void freeShadow(){
    if (0!=hashmap_iterate_pairs(&hashmap, freeAll, (void *)NULL)) {
        perror("failed to deallocate hashmap entries\n");
        exit(1);
    }
    hashmap_destroy(&hashmap);
}