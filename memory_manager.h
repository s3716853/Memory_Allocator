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
        
        // static void setBestFit();
        // static void setWorstFit();
        // static void setFirstFit();

        static void reset();
    private:
        // static bool bestFit;
        // static bool worstFit;
        // static bool firstFit = true;

        static void * bestFitAlloc(size_t chunk_size);
        static void * worstFitAlloc(size_t chunk_size);
        static void * firstFitAlloc(size_t chunk_size);

        static void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);

        //TEST METHODS
        static void printListSize();
};