#include <list> 
#include <pthread.h>
#include <iostream>

#include "memory_chunk.cpp"
#include "method.cpp"

class MemoryList {
    public:

         //finds memory chunk with given address
        std::list<MemoryChunk>::iterator find(void * address);
        
        //finds memory chunk of specified size user specified method
        //returns list.end() if no chunk found
        std::list<MemoryChunk>::iterator find(size_t size, Method method);

        void push_back(const MemoryChunk chunk);

        std::list<MemoryChunk>::iterator erase(std::list<MemoryChunk>::iterator it);

        //returns list.end() for comparison to see if the find
        //methods found any chunks
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

        pthread_mutex_t listReadLock = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_t listWriteLock = PTHREAD_MUTEX_INITIALIZER;
        
        pthread_mutex_t readersLock = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_t writersLock = PTHREAD_MUTEX_INITIALIZER;

        pthread_cond_t readersComplete = PTHREAD_COND_INITIALIZER;
        pthread_cond_t writersComplete = PTHREAD_COND_INITIALIZER;

        void increaseReaders();
        void decreaseReaders();

        void increaseWriters();
        void decreaseWriters();

        void writerWait();
        void readersWait()
};