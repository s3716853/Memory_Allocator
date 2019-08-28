#include "memory_manager.h"

std::list<MemoryChunk> unallocatedMemory;
std::list<MemoryChunk> allocatedMemory;
Method allocMethod;

void * MemoryManager::alloc(size_t chunk_size){
    
    void * returnChunk = nullptr;

    if(allocMethod == FIRST){
        returnChunk = firstFitAlloc(chunk_size);
    }else if(allocMethod == WORST){
        returnChunk = worstFitAlloc(chunk_size);
    }else if(allocMethod == BEST){
        returnChunk = bestFitAlloc(chunk_size);
    }else{
        std::cout << "NONE OF THE METHODS DONE" << std::endl;
    }

    if(returnChunk == nullptr){
        returnChunk = sbrk(chunk_size);
        MemoryChunk chunk;
        chunk.size = chunk_size;
        chunk.address = returnChunk;
        allocatedMemory.push_back(chunk);
    }

    printListSize();

    return returnChunk;
}

void MemoryManager::dealloc(void * chunk){
    
    printListSize();

    std::list<MemoryChunk>::iterator it;
    
    bool chunkFound = false;;
    for(it = allocatedMemory.begin(); it != allocatedMemory.end() && !chunkFound; ++it){
        std::cout << "ADDRESS IN LIST " << it->address << " ADDRESS GIVEN" << chunk << std::endl;
        if(it->address == chunk){
            allocatedMemory.erase(it);
            unallocatedMemory.push_back(*it);
            chunkFound = true;
        }
    }

    if(!chunkFound){
        abort();
    }
    printListSize();
}

void MemoryManager::setMethod(Method method){
    allocMethod = method;
}

void * MemoryManager::bestFitAlloc(size_t chunk_size){

    void * returnChunk = nullptr;
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator bestFitMemory = it;
    bool sameSizeChunkFound = false;
    bool memoryLocationFound = false;
    
    //finding best fit chunk
    for(it = unallocatedMemory.begin(); it != unallocatedMemory.end() && !sameSizeChunkFound; ++it){
        if(it->size == chunk_size){
            bestFitMemory = it;
            //if chunk of same size is found, that would be the best fit for the chunk
            //so search can end early
            sameSizeChunkFound = true; 
            memoryLocationFound = true;
        }else if(it->size > chunk_size  && (!memoryLocationFound || it->size < bestFitMemory->size)){
            bestFitMemory = it;
            memoryLocationFound = true;
        }
    }

    if(memoryLocationFound){
        if(bestFitMemory->size == chunk_size){
            returnChunk = bestFitMemory->address;
            allocatedMemory.push_back(*bestFitMemory);
            unallocatedMemory.erase(bestFitMemory);
        }else{
            returnChunk = resizeChunk(bestFitMemory, chunk_size);
        }
    }

    return returnChunk;
}

void * MemoryManager::worstFitAlloc(size_t chunk_size){

    void * returnChunk = nullptr;
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator worstFitMemory = it;
    bool memoryLocationFound = false;
    
    //finding worst fit chunk
    for(it = unallocatedMemory.begin(); it != unallocatedMemory.end(); ++it){
        if(it->size >= chunk_size  && (!memoryLocationFound || it->size > worstFitMemory->size)){
            worstFitMemory = it;
            memoryLocationFound = true;
        }
    }

    if(memoryLocationFound){
        if(worstFitMemory->size == chunk_size){
            returnChunk = worstFitMemory->address;
            allocatedMemory.push_back(*worstFitMemory);
            unallocatedMemory.erase(worstFitMemory);
        }else{
            returnChunk = resizeChunk(worstFitMemory, chunk_size);
        }
    }

    return returnChunk;
}

void * MemoryManager::firstFitAlloc(size_t chunk_size){

    void * returnChunk = nullptr;
    std::list<MemoryChunk>::iterator it;
    bool chunkFound = false;
    for(it = unallocatedMemory.begin(); it != unallocatedMemory.end() && !chunkFound; ++it){
        if(it->size == chunk_size){
            std::cout << "!!EXACT SIZE FOUND!!" << std::endl;
            returnChunk = it->address;
            allocatedMemory.push_back(*it);
            unallocatedMemory.erase(it);
            chunkFound = true;
        }else if(it->size > chunk_size){
            returnChunk = resizeChunk(it, chunk_size);
            chunkFound = true;
        }
    }

    return returnChunk;
}

void * MemoryManager::resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo){
    std::cout << "!!BIG ONE FOUND!!" << std::endl;

    //getting memory address that will become the chunk
    //that is the remaining memory not given to the user
    void * largeChunk = (void *) ((char*)it->address + resizeTo);

    //Making new chunk for remaining space after resize
    MemoryChunk chunk;
    chunk.address = largeChunk;
    chunk.size = it->size - resizeTo;
    unallocatedMemory.push_back(chunk);

    //Changing size of old chunk and swapping which list its in
    it->size = resizeTo;
    unallocatedMemory.erase(it);
    allocatedMemory.push_back(*it);

    return it->address;
}

// void MemoryManager::setBestFit(){
//     bestFit = true;
//     worstFit = false;
//     firstFit = false;
// }

// void MemoryManager::setWorstFit(){
//     bestFit = false;
//     worstFit = true;
//     firstFit = false;
// }

// void MemoryManager::setFirstFit(){
//     bestFit = false;
//     worstFit = false;
//     firstFit = true;
// }

void MemoryManager::printListSize(){
    std::cout << "SIZE OF ALLOCATEDMEMORY: " << allocatedMemory.size() << std::endl;
    std::cout << "SIZE OF UNALLOCATEDMEMORY: " << unallocatedMemory.size() << std::endl;
}

void MemoryManager::reset(){
    // for(unsigned int i = 0; i < unallocatedMemory.size(); ++i){
    //     delete unallocatedMemory.back().address;
    //     unallocatedMemory.pop_back();
    // }

    // for(unsigned int i = 0;  i < allocatedMemory.size(); ++i){
    //     delete allocatedMemory.back().address;
    //     allocatedMemory.pop_back();
    // }
}