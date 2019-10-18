#ifndef mem_chunk
#define mem_chunk

#include <unistd.h>
#include <pthread.h>
struct MemoryChunk {
    pthread_mutex_t lock;
    void * address;
    size_t size;

    MemoryChunk(void* address, size_t size){
        this->address = address;
        this->size = size;
        pthread_mutex_init(&lock, NULL);
    }
    
};

#endif