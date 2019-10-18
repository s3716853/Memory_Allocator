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

        if(pthread_mutex_unlock(&chunk.lock) != 0){
            std::cout << "AINT LOCKED" << std::endl;
        }

        allocatedMemory.push_back(chunk);        

    }else{
        
        pthread_mutex_lock(&newMemoryLock);
        //std::cout << "ALLOCATING MORE MEMORY" <<std::endl;
        returnChunk = sbrk(chunk_size);
        pthread_mutex_unlock(&newMemoryLock);

        MemoryChunk chunk(returnChunk, chunk_size);
        // int test = pthread_mutex_trylock(&(chunk.lock));
        // std::cout << "ALLOCATING MORE MEMORY: " << test << std::endl;
        // int test2 = pthread_mutex_unlock(&(chunk.lock));
        // std::cout << "UNLOCK: " << test << std::endl;
        // chunk.size = chunk_size;
        // chunk.address = returnChunk;

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
    }
}

void setMethod(Method method){
    pthread_mutex_lock(&methodLock);
    allocMethod = method;
    pthread_mutex_unlock(&methodLock);
}

void initilise(Method method){
    setMethod(method);
    // pthread_mutex_init(&newMemoryLock, NULL);
    // pthread_mutex_init(&methodLock, NULL);
}

void printLists(){
    std::cout << "ALLOCATED LIST:"<< std::endl;
    allocatedMemory.print();
    std::cout << "UNALLOCATED LIST:"<< std::endl;
    unallocatedMemory.print();
}