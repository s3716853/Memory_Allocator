#include "memory_list.h" 


MemoryList::MemoryList(){
    pthread_mutexattr_t test;
    pthread_mutexattr_settype(&test, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&listLock, NULL);
    pthread_mutex_init(&userCounter, NULL);
    pthread_cond_init(&readersComplete, NULL);
    pthread_cond_init(&writersComplete, NULL);
}

std::list<MemoryChunk>::iterator MemoryList::erase(std::list<MemoryChunk>::iterator it){
    
    std::list<MemoryChunk>::iterator returnIt;
    
    writeInitialise();
    returnIt = list.erase(it);
    writeTerminate();

    return returnIt;
}

void MemoryList::push_back(MemoryChunk chunk){

    writeInitialise();    
    list.push_back(chunk);
    writeTerminate();

}

std::list<MemoryChunk>::iterator MemoryList::find(void * address){
    
    bool chunkFound = false;
    
    std::list<MemoryChunk>::iterator it;

    readInitialise();
    std::list<MemoryChunk>::iterator chunk = list.end();
    for(it = list.begin(); it != list.end() && !chunkFound; ++it){
        if(it->address == address){
            // int pthread_return = pthread_mutex_trylock(&(it->lock));
            // if(pthread_return == 0){
            //     //std::cout << "LOCK OBTAIN" << std::endl;
            //     pthread_mutex_unlock(&(it->lock));
            // }else{
            //     //std::cout << "WHAT LOCK?" << std::endl;
            // }
            chunk = it;
            chunkFound = true;
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

void MemoryList::print(){

    readInitialise();
    size_t size = 0;
    for(MemoryChunk chunk: list){
        size+=chunk.size;
    }
    
    std::cout << "Total chunks is " << std::to_string(list.size()) << std::endl
    << "Size of List is: " << std::to_string(size) << std::endl;
    // for(MemoryChunk chunk:list){
    //     std::cout << "[" << chunk.size << "], ";
    // }
    // std::cout << std::endl;
    readTerminate();
}


std::list<MemoryChunk>::iterator MemoryList::findFirst(size_t size){

    bool chunkFound = false;
    std::list<MemoryChunk>::iterator it;

    readInitialise();
    std::list<MemoryChunk>::iterator firstFitMemory = list.end();
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
    bool memoryLocationFound = false;

    readInitialise();
    std::list<MemoryChunk>::iterator worstFitMemory = list.end();
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
    
    bool sameSizeChunkFound = false;
    bool memoryLocationFound = false;

    readInitialise();
    std::list<MemoryChunk>::iterator bestFitMemory = list.end();
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

    //getting memory address that will become the chunk
    //that is the remaining memory not given to the user
    void * largeChunk = (void *) ((char*)it->address + resizeTo);

    //Making new chunk for remaining space after resize
    MemoryChunk chunk(largeChunk, it->size - resizeTo);
    // chunk.address = largeChunk;
    // chunk.size = it->size - resizeTo;

    push_back(chunk);
    //Changing size of old chunk and swapping which list its in
    it->size = resizeTo;

    return it;
}

void MemoryList::writeInitialise(){
    pthread_mutex_lock(&userCounter);
    
    while(readers > 0){
        pthread_cond_wait(&readersComplete, &userCounter);
    }
    ++writers;
    
    pthread_mutex_unlock(&userCounter);
    
    pthread_mutex_lock(&listLock);
}

void MemoryList::writeTerminate(){
    pthread_mutex_unlock(&listLock);
    
    pthread_mutex_lock(&userCounter);
    
    --writers;
    
    if(writers == 0){
        pthread_cond_broadcast(&writersComplete);
    }
    
    pthread_mutex_unlock(&userCounter);
}

void MemoryList::readInitialise(){
    pthread_mutex_lock(&userCounter);
    
    while(writers > 0){
        pthread_cond_wait(&writersComplete, &userCounter);
    }
    
    ++readers;
    
    pthread_mutex_unlock(&userCounter);
}

void MemoryList::readTerminate(){
    pthread_mutex_lock(&userCounter);
    
    --readers;
    
    if(readers == 0){
        pthread_cond_broadcast(&readersComplete);
    }
    
    pthread_mutex_unlock(&userCounter);
}