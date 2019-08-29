#include <list> 
#include <iostream>
#include <iterator>
#include <unistd.h>
 
#include "memory_chunk.cpp"
#include "method.cpp"

class MemoryManager {
    public:
        static void * alloc(size_t chunk_size);
        static void dealloc(void * chunk);

        static void setMethod(Method method);
        
        //TEST METHODS
        static void reset();
    private:

        static void * bestFitAlloc(size_t chunk_size);
        static void * worstFitAlloc(size_t chunk_size);
        static void * firstFitAlloc(size_t chunk_size);

        static void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);

        //TEST METHODS
        static void printListSize();
};