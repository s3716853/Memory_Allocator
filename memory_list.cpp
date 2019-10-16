#include "memory_list.h" 


std::list<MemoryChunk>::iterator MemoryList::erase(std::list<MemoryChunk>::iterator it){
    
    std::list<MemoryChunk>::iterator returnIt;
    
    writeInitialise();
    returnIt = list.erase(it);
    //std::cout << "ERASE: " << pthread_self() << std::endl;
    writeTerminate();

    return returnIt;
}

void MemoryList::push_back(const MemoryChunk chunk){

    writeInitialise();
    list.push_back(chunk);
    writeTerminate();

}

std::list<MemoryChunk>::iterator MemoryList::find(void * address){
    
    bool chunkFound = false;

    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator chunk = list.end();
    
    readInitialise();
    for(it = list.begin(); it != list.end() && !chunkFound; ++it){
        if(it->address == address){
            if(pthread_mutex_trylock(&(it->lock)) == 0){
                chunk = it;
                chunkFound = true;
            }
        }
    }
    readTerminate();

    return chunk;
}

std::list<MemoryChunk>::iterator MemoryList::find(size_t size, Method method){
    
    std::list<MemoryChunk>::iterator chunk;

    if(method == FIRST){
        chunk = findFirst(size);
    }else if(method == WORST){
        chunk = findWorst(size);
    }else if (method == BEST){
        chunk = findBest(size);
    }

    if(chunk != list.end()){
        if(chunk->size > size){
            chunk = resizeChunk(chunk, size);
        }
    }

    return chunk;
}

std::list<MemoryChunk>::iterator MemoryList::end(){
    return list.end();
}

std::string MemoryList::memorySize(){
    
    readInitialise();
    std::string output = "==========Total chunks is " + std::to_string(list.size()) + "==========\n";
    for(MemoryChunk chunk:list){
        output += "[" + std::to_string(chunk.size) + "], ";
    }
    readTerminate();

    return output;
}

std::list<MemoryChunk>::iterator MemoryList::findFirst(size_t size){

    bool chunkFound = false;
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator firstFitMemory = list.end();
    
    readInitialise();
    for(it = list.begin(); it != list.end() && !chunkFound; ++it){
        if(it->size >= size){
            if(pthread_mutex_trylock(&(it->lock)) == 0){
                chunkFound = true;
                firstFitMemory = it;           
            }
        }
    }
    readTerminate();

    return firstFitMemory;
}
std::list<MemoryChunk>::iterator MemoryList::findWorst(size_t size){
    
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator worstFitMemory = list.end();
    bool memoryLocationFound = false;

    readInitialise();
    for(it = list.begin(); it != list.end(); ++it){
        if(it->size >= size  && (!memoryLocationFound || it->size > worstFitMemory->size)){
            if(pthread_mutex_trylock(&(it->lock)) == 0){
                if(memoryLocationFound){
                    pthread_mutex_unlock(&(worstFitMemory->lock));
                }
                worstFitMemory = it;
                memoryLocationFound = true;
            }
        }
    }
    readTerminate();

    return worstFitMemory;
}
std::list<MemoryChunk>::iterator MemoryList::findBest(size_t size){
    
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator bestFitMemory = list.end();
    bool sameSizeChunkFound = false;
    bool memoryLocationFound = false;

    readInitialise();
    for(it = list.begin(); it != list.end() && !sameSizeChunkFound; ++it){
        if(it->size == size){
            if(pthread_mutex_trylock(&(it->lock)) == 0){
                //if the allocator finds a chunk that fits better,
                //it needs to give up the old one
                if(memoryLocationFound){
                    pthread_mutex_unlock(&(bestFitMemory->lock));
                }
                bestFitMemory = it;
                //if chunk of same size is found, that would be the best fit for the chunk
                //so search can end early
                sameSizeChunkFound = true; 
                memoryLocationFound = true;
            }
        }else if(it->size > size  && (!memoryLocationFound || it->size < bestFitMemory->size)){
            if(pthread_mutex_trylock(&(it->lock)) == 0){
                if(memoryLocationFound){
                    pthread_mutex_unlock(&(bestFitMemory->lock));
                }
                bestFitMemory = it;
                memoryLocationFound = true;
            }
        }
    }
    readTerminate();

    return bestFitMemory;
}

//Takes iterator from unallocatedMemory to resize and add to allocated memory and add chunk
//of size (originalChunkSize-ResizeTo) to unnallocated memory
//Returns the pointer of the memory inside the resized chunk
std::list<MemoryChunk>::iterator MemoryList::resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo){
    // std::cout << "~~~~~~~~~~Resizing chunk of size " 
    // << it->size << " to get " << resizeTo << "~~~~~~~~~~" << std::endl;

    writeInitialise();

    //getting memory address that will become the chunk
    //that is the remaining memory not given to the user
    void * largeChunk = (void *) ((char*)it->address + resizeTo);

    //Making new chunk for remaining space after resize
    MemoryChunk chunk;
    chunk.address = largeChunk;
    chunk.size = it->size - resizeTo;
    
    list.push_back(chunk);
    
    //Changing size of old chunk and swapping which list its in
    it->size = resizeTo;
    
    writeTerminate();

    return it;
}

void MemoryList::increaseReaders(){
    pthread_mutex_lock(&readersLock);
    ++readers;
    //std::cout << "READERS:" << readers << std::endl;
    pthread_mutex_unlock(&readersLock);
}

void MemoryList::decreaseReaders(){
    pthread_mutex_lock(&readersLock);
    --readers;
    //std::cout << "READERS:" << readers << std::endl;
    if(readers == 0){
        pthread_cond_broadcast(&readersComplete);
    }
    pthread_mutex_unlock(&readersLock);
}

void MemoryList::increaseWriters(){
    pthread_mutex_lock(&writersLock);
    ++writers;
    //std::cout << "WRITERS:" << writers << std::endl;
    pthread_mutex_unlock(&writersLock);
}

void MemoryList::decreaseWriters(){
    pthread_mutex_lock(&writersLock);
    --writers;
    //std::cout << "WRITERS:" << writers << std::endl;
    if(writers == 0){
        pthread_cond_broadcast(&writersComplete);
    }
    pthread_mutex_unlock(&writersLock);
}

void MemoryList::writerWait(){
    pthread_mutex_lock(&writersLock);
    while(writers > 0){
        pthread_cond_wait(&writersComplete, &writersLock);
    }
    pthread_mutex_unlock(&writersLock);
}

void MemoryList::readersWait(){
    pthread_mutex_lock(&readersLock);
    while(readers > 0){
        pthread_cond_wait(&readersComplete, &readersLock);
    }
    pthread_mutex_unlock(&readersLock);
}

void MemoryList::writeInitialise(){
    readersWait();
    increaseWriters();
    pthread_mutex_lock(&listLock);
}

void MemoryList::writeTerminate(){
    pthread_mutex_unlock(&listLock);
    decreaseWriters();
}

void MemoryList::readInitialise(){
    writerWait();
    increaseReaders();
}

void MemoryList::readTerminate(){
    decreaseReaders();
}