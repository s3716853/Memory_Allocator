#include <list> 
#include <iostream>
#include <iterator>
#include <unistd.h>
 
#include "memory_chunk.cpp"

class MemoryManager {
    public:
    
        MemoryManager();
        ~MemoryManager();

        void * alloc(size_t chunk_size);
        void dealloc(void * chunk);

        void setBestFit();
        void setWorstFit();
        void setFirstFit();

        void reset();
    private:
        
        std::list<MemoryChunk> unallocatedMemory;
        std::list<MemoryChunk> allocatedMemory;

        void * bestFitAlloc(size_t chunk_size);
        void * worstFitAlloc(size_t chunk_size);
        void * firstFitAlloc(size_t chunk_size);

        void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);

        bool bestFit;
        bool worstFit;
        bool firstFit;


        //TEST METHODS
        void printListSize();
};