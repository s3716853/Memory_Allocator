#include <list> 
#include <pthread.h>
#include <iostream>

#include "memory_chunk.cpp"
#include "method.cpp"

class MemoryList {
    public:
        std::list<MemoryChunk>::iterator find(void * address);
        std::list<MemoryChunk>::iterator find(size_t size, Method method);

        void push_back(const MemoryChunk chunk);

        std::list<MemoryChunk>::iterator erase(std::list<MemoryChunk>::iterator it);

        std::list<MemoryChunk>::iterator end();

        void memorySize();
    private:
        std::list<MemoryChunk>::iterator findFirst(size_t size);
        std::list<MemoryChunk>::iterator findWorst(size_t size);
        std::list<MemoryChunk>::iterator findBest(size_t size);
        std::list<MemoryChunk>::iterator resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);

        std::list<MemoryChunk> list;

        unsigned int readers;
        unsigned int writers;

        pthread_mutex_t readLock = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_t writeLock = PTHREAD_MUTEX_INITIALIZER;
};