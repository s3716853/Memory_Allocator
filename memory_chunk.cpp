#include <unistd.h>
#include <pthread.h>
struct MemoryChunk {
    pthread_mutex_t memoryChunkLock = PTHREAD_MUTEX_INITIALIZER;
    void * address;
    size_t size;
};