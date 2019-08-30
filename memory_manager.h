#include <list> 
#include <iostream>
#include <iterator>
#include <unistd.h>
 
#include "memory_chunk.cpp"
#include "method.cpp"

void * alloc(size_t chunk_size);
void dealloc(void * chunk);
void * alloc(size_t chunk_size);
void dealloc(void * chunk);
void setMethod(Method method);

//TEST METHODS
void reset();

void * bestFitAlloc(size_t chunk_size);
void * worstFitAlloc(size_t chunk_size);
void * firstFitAlloc(size_t chunk_size);

void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);

//TEST METHODS
void printListSize();