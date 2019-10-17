#include <list> 
#include <iostream>
#include <iterator>
#include <unistd.h>
#include <sys/resource.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <pthread.h>

#include "memory_list.h"
#include "memory_chunk.cpp"
#include "method.cpp"

void * alloc(size_t chunk_size);
void dealloc(void * chunk);
void setMethod(Method method);

// //Methods to assist my experiments
// double memoryChunkAmount();
// double totalMemoryAllocatedSize();
void printListSize();
void allocatedMemorySize();