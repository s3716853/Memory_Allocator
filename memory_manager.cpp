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
        allocatedMemory.push_back(*memory);
        unallocatedMemory.erase(memory);
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

    std::list<MemoryChunk>::iterator it = allocatedMemory.find(chunk);
    
    if(it == allocatedMemory.end()){
        std::cout << "chunk not found error :-(" << std::endl;
        abort();
    }else{
        allocatedMemory.erase(it);
        unallocatedMemory.push_back(*it);
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

// void addUnallocatedReaders(){
//     pthread_mutex_lock(&unallocatedReadersLock);
//     ++unallocatedReaders;
//     pthread_mutex_unlock(&unallocatedReadersLock);
// }

// void lowerUnallocatedReaders(){
//     pthread_mutex_lock(&unallocatedReadersLock);
//     --unallocatedReaders;
//     pthread_mutex_unlock(&unallocatedReadersLock);
// }

// void addAllocatedReaders(){
//     pthread_mutex_lock(&allocatedReadersLock);
//     ++allocatedReaders;
//     pthread_mutex_unlock(&allocatedReadersLock);
// }
// void lowerAllocatedReaders(){
//     pthread_mutex_lock(&allocatedReadersLock);
//     --allocatedReaders;
//     pthread_mutex_unlock(&allocatedReadersLock);
// }

// void addUnallocatedWriters(){
//     pthread_mutex_lock(&unallocatedWritersLock);
//     ++unallocatedWriters;
//     pthread_mutex_unlock(&unallocatedWritersLock);
// }

// void lowerUnallocatedWriters(){
//     pthread_mutex_lock(&unallocatedWritersLock);
//     --unallocatedWriters;
//     pthread_mutex_unlock(&unallocatedWritersLock);
// }

// void addAllocatedWriters(){
//     pthread_mutex_lock(&allocatedWritersLock);
//     ++allocatedWriters;
//     pthread_mutex_unlock(&allocatedWritersLock);
// }
// void lowerAllocatedWriters(){
//     pthread_mutex_lock(&allocatedWritersLock);
//     --allocatedWriters;
//     pthread_mutex_unlock(&allocatedWritersLock);
// }

// void writeLockUnallocatedMemory(){
//     pthread_mutex_lock(&unallocatedMemoryReadLock);
//     pthread_mutex_lock(&unallocatedMemoryWriteLock);
//     while(unallocatedReaders!=0);
// }

// void writeUnlockUnallocatedMemory(){
//     pthread_mutex_unlock(&unallocatedMemoryReadLock);
//     pthread_mutex_unlock(&unallocatedMemoryWriteLock);
// }

// void writeLockAllocatedMemory(){
//     pthread_mutex_lock(&allocatedMemoryReadLock);
//     pthread_mutex_lock(&allocatedMemoryWriteLock);
//     while(allocatedReaders!=0);
// }

// void writeUnlockAllocatedMemory(){
//     pthread_mutex_unlock(&allocatedMemoryReadLock);
//     pthread_mutex_unlock(&allocatedMemoryWriteLock);
// }