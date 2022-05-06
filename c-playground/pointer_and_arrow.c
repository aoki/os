#include <stdio.h>
#include <stdlib.h>

struct MemoryMap {
    uint64_t buffer_size;
    void *buffer;
    uint64_t map_size;
    uint64_t map_key;
    uint64_t descriptor_size;
    uint32_t descriptor_version;
    // uint32_t descriptor_version2;
    // uint32_t descriptor_version3;
};

int main() {
    printf("# Hello pointer and arrow\n");

    struct MemoryMap m;
    struct MemoryMap *pm;

    printf("m's size: \t%4lu\n", sizeof(m));
    printf("\tm: \t\t%p\n", &m);
    printf("\tbuffer_size: \t%p\n", &m.buffer_size);
    printf("\tbuffer: \t%p\n", &m.buffer);
    printf("\tmap_size: \t%p\n", &m.map_size);
    printf("\tmap_key: \t%p\n", &m.map_key);
    printf("\tdescriptor_size: \t%p\n", &m.descriptor_size);
    printf("\tdescriptor_version: \t%p\n", &m.descriptor_version);
    printf("pm's size: \t%4lu\n", sizeof(pm));
    printf("\tpm: \t\t%p\n", &pm);

    printf("\n");
    printf("pm->buffer_size: %llu\n", pm->buffer_size);
    printf("(*pm).buffer_size: %llu\n", (*pm).buffer_size);
}
