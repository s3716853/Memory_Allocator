#ifndef mem_chunk
#define mem_chunk

#include <unistd.h>
#include <pthread.h>
struct MemoryChunk {
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    void * address;
    size_t size;
};

#endif