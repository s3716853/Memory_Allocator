#include "memory_manager.h"

MemoryList unallocatedMemory;
MemoryList allocatedMemory;
Method allocMethod = FIRST;

pthread_mutex_t newMemoryLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t methodLock = PTHREAD_MUTEX_INITIALIZER;

void * alloc(size_t chunk_size){
    
    void * returnChunk = nullptr;

    std::list<MemoryChunk>::iterator memory = unallocatedMemory.find(chunk_size, allocMethod);

    if(memory != unallocatedMemory.end()){
        
        MemoryChunk chunk = *memory;
        returnChunk = chunk.address;

        unallocatedMemory.erase(memory);
        
        allocatedMemory.push_back(chunk);

        pthread_mutex_unlock(&(chunk.lock));

    }else{
        
        pthread_mutex_lock(&newMemoryLock);
        returnChunk = sbrk(chunk_size);
        pthread_mutex_unlock(&newMemoryLock);

        MemoryChunk chunk;
        chunk.size = chunk_size;
        chunk.address = returnChunk;

        allocatedMemory.push_back(chunk);
    }

    return returnChunk;
}

void dealloc(void * chunk){
    
    std::list<MemoryChunk>::iterator memory = allocatedMemory.find(chunk);
    
    if(memory == allocatedMemory.end()){
        std::cout << "chunk not found error :-(" << std::endl;
        abort();
    }else{
        MemoryChunk chunk = *memory;
        allocatedMemory.erase(memory);
        
        unallocatedMemory.push_back(chunk);

        pthread_mutex_unlock(&(chunk.lock));
    }
}

void setMethod(Method method){
    pthread_mutex_lock(&methodLock);
    allocMethod = method;
    pthread_mutex_unlock(&methodLock);
}

void printLists(){
    std::cout << "ALLOCATED LIST:"<< std::endl;
    allocatedMemory.print();
    std::cout << "UNALLOCATED LIST:"<< std::endl;
    unallocatedMemory.print();
}