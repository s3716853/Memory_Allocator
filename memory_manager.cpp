#include "memory_manager.h"

void * bestFitAlloc(size_t chunk_size);
void * worstFitAlloc(size_t chunk_size);
void * firstFitAlloc(size_t chunk_size);
void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);
void printListSize();

std::list<MemoryChunk> unallocatedMemory;
std::list<MemoryChunk> allocatedMemory;
Method allocMethod = FIRST;

void * alloc(size_t chunk_size){
    
    void * returnChunk = nullptr;

    if(allocMethod == FIRST){
        returnChunk = firstFitAlloc(chunk_size);
    }else if(allocMethod == WORST){
        returnChunk = worstFitAlloc(chunk_size);
    }else if(allocMethod == BEST){
        returnChunk = bestFitAlloc(chunk_size);
    }

    if(returnChunk == nullptr){
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
    
    //printListSize();

    std::list<MemoryChunk>::iterator it;
    
    bool chunkFound = false;;
    for(it = allocatedMemory.begin(); it != allocatedMemory.end() && !chunkFound; ++it){
        std::cout << "~~~~~~~~~~Deallocating " << chunk << " of size " << 
        it->size << "~~~~~~~~~~" << std::endl;
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

void setMethod(Method method){
    allocMethod = method;
}

void * bestFitAlloc(size_t chunk_size){

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
            std::cout << "~~~~~~~~~~Exact chunk of size " << bestFitMemory->size << 
            " found~~~~~~~~~~" << std::endl;
            returnChunk = bestFitMemory->address;
            allocatedMemory.push_back(*bestFitMemory);
            unallocatedMemory.erase(bestFitMemory);
        }else{
            returnChunk = resizeChunk(bestFitMemory, chunk_size);
        }
    }

    return returnChunk;
}

void * worstFitAlloc(size_t chunk_size){

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
            std::cout << "~~~~~~~~~~Worst chunk of size " << worstFitMemory->size << 
            " found~~~~~~~~~~" << std::endl;
            returnChunk = worstFitMemory->address;
            allocatedMemory.push_back(*worstFitMemory);
            unallocatedMemory.erase(worstFitMemory);
        }else{
            returnChunk = resizeChunk(worstFitMemory, chunk_size);
        }
    }

    return returnChunk;
}

void * firstFitAlloc(size_t chunk_size){

    void * returnChunk = nullptr;
    std::list<MemoryChunk>::iterator it;
    bool chunkFound = false;
    for(it = unallocatedMemory.begin(); it != unallocatedMemory.end() && !chunkFound; ++it){
        if(it->size == chunk_size){
            std::cout << "~~~~~~~~~~Exact chunk of size " << it->size << 
            " found~~~~~~~~~~" << std::endl;
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

//Takes iterator from unallocatedMemory to resize and add to allocated memory and add chunk
//of size (originalChunkSize-ResizeTo) to unnallocated memory
//Returns the pointer of the memory inside the resized chunk
void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo){
    std::cout << "~~~~~~~~~~Resizing chunk of size " 
    << it->size << " to get " << resizeTo << "~~~~~~~~~~" << std::endl;

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

void printListSize(){

    std::cout << "==========Total chunks allocated is " << allocatedMemory.size() <<
    "==========" << std::endl;
    for(MemoryChunk chunk:allocatedMemory){
        std::cout << "[" << chunk.size << "], ";
    }
    std::cout << std::endl << "==========Total chunks unallocated is " << unallocatedMemory.size() <<
    "==========" << std::endl;
    for(MemoryChunk chunk:unallocatedMemory){
        std::cout << "[" << chunk.size << "], ";
    }
    
    std::cout << std::endl << "==========Total chunks created is " <<
    allocatedMemory.size() + unallocatedMemory.size() <<
    "==========" << std::endl;
}

double memoryChunkAmount(){
    return unallocatedMemory.size() + allocatedMemory.size();
}

double totalMemoryAllocatedSize(){
    double total = 0;
    for(MemoryChunk chunk: unallocatedMemory){
        total += chunk.size;
    }
    return total;
}