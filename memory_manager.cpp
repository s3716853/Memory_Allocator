#include "memory_manager.h"

MemoryList unallocatedMemory;
MemoryList allocatedMemory;
Method allocMethod = FIRST;

void * alloc(size_t chunk_size){
    
    void * returnChunk = nullptr;

    std::list<MemoryChunk>::iterator memory = unallocatedMemory.find(chunk_size, allocMethod);

    if(memory != unallocatedMemory.end()){
        std::cout << "~~~~~~~~~~Chunk of size " << memory->size << 
        " found~~~~~~~~~~" << std::endl;
        returnChunk = memory->address;
        unallocatedMemory.erase(memory);
        allocatedMemory.push_back(*memory);
        pthread_mutex_unlock(&(memory->lock));
    }else{
        returnChunk = sbrk(chunk_size);
        MemoryChunk chunk;
        chunk.size = chunk_size;
        chunk.address = returnChunk;

        allocatedMemory.push_back(chunk);

        std::cout << "~~~~~~~~~~Creating new memory " << returnChunk << " of size " << 
        chunk_size << "~~~~~~~~~~" << std::endl;
    }

    printListSize();

    return returnChunk;
}

void dealloc(void * chunk){

    std::list<MemoryChunk>::iterator memory = allocatedMemory.find(chunk);
    
    if(memory == allocatedMemory.end()){
        std::cout << "chunk not found error :-(" << std::endl;
        abort();
    }else{
        allocatedMemory.erase(memory);
        unallocatedMemory.push_back(*memory);
        pthread_mutex_unlock(&(memory->lock));
    }
    
    printListSize();
}

void setMethod(Method method){
    allocMethod = method;
}

void printListSize(){
    allocatedMemory.memorySize();
    unallocatedMemory.memorySize();
}