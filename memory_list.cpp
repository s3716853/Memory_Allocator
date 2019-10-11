#include "memory_list.h" 


std::list<MemoryChunk>::iterator MemoryList::erase(std::list<MemoryChunk>::iterator it){
    return list.erase(it);
}

void MemoryList::push_back(const MemoryChunk chunk){
    
    list.push_back(chunk);
}

std::list<MemoryChunk>::iterator MemoryList::find(void * address){
    
    bool chunkFound = false;
    std::list<MemoryChunk>::iterator chunk = list.end();
    std::list<MemoryChunk>::iterator it;

    for(it = list.begin(); it != list.end() && !chunkFound; ++it){
        if(it->address == address){
            chunk = it;
            chunkFound = true;
        }
    }


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

    return chunk;
}

std::list<MemoryChunk>::iterator MemoryList::end(){
    return list.end();
}

void  MemoryList::memorySize(){
    std::cout << std::endl << "==========Total chunks allocated is " << list.size() <<
    "==========" << std::endl;
    for(MemoryChunk chunk:list){
        std::cout << "[" << chunk.size << "], ";
    }
}

std::list<MemoryChunk>::iterator MemoryList::findFirst(size_t size){
    
    bool chunkFound = false;
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator firstFitMemory = list.end();

    for(it = list.begin(); it != list.end() && !chunkFound; ++it){
        if(it->size >= size){
            //if(pthread_mutex_trylock(&(it->lock)) == 0){
                chunkFound = true;
                firstFitMemory = it;           
            //}
        }
    }

    if(chunkFound){
        if(firstFitMemory->size > size){
            firstFitMemory = resizeChunk(firstFitMemory, size);
        }
    }

    return firstFitMemory;
}
std::list<MemoryChunk>::iterator MemoryList::findWorst(size_t size){
    
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator worstFitMemory = list.end();
    bool memoryLocationFound = false;
    
    //finding worst fit chunk
    for(it = list.begin(); it != list.end(); ++it){
        if(it->size >= size  && (!memoryLocationFound || it->size > worstFitMemory->size)){
            worstFitMemory = it;
            memoryLocationFound = true;
        }
    }

    if(memoryLocationFound){
        if(worstFitMemory->size > size){
            worstFitMemory = resizeChunk(worstFitMemory, size);
        }
    }

    return worstFitMemory;
}
std::list<MemoryChunk>::iterator MemoryList::findBest(size_t size){
    
    std::list<MemoryChunk>::iterator it;
    std::list<MemoryChunk>::iterator bestFitMemory = list.end();
    bool sameSizeChunkFound = false;
    bool memoryLocationFound = false;

    //finding best fit chunk
    for(it = list.begin(); it != list.end() && !sameSizeChunkFound; ++it){
        if(it->size == size){
            //if(pthread_mutex_trylock(&(it->lock)) == 0){
                //if the allocator finds a chunk that fits better,
                //it needs to give up the old one
                // if(memoryLocationFound){
                //     pthread_mutex_unlock(&(it->lock));
                // }
                bestFitMemory = it;
                //if chunk of same size is found, that would be the best fit for the chunk
                //so search can end early
                sameSizeChunkFound = true; 
                memoryLocationFound = true;
            //}
        }else if(it->size > size  && (!memoryLocationFound || it->size < bestFitMemory->size)){
            //if(pthread_mutex_trylock(&(it->lock)) == 0){
                // if(memoryLocationFound){
                //     pthread_mutex_unlock(&(it->lock))
                // }
                bestFitMemory = it;
                memoryLocationFound = true;
            //}
        }
    }

    if(memoryLocationFound){
        if(bestFitMemory->size > size){
            bestFitMemory = resizeChunk(bestFitMemory, size);
        }
    }


    return bestFitMemory;
}

//Takes iterator from unallocatedMemory to resize and add to allocated memory and add chunk
//of size (originalChunkSize-ResizeTo) to unnallocated memory
//Returns the pointer of the memory inside the resized chunk
std::list<MemoryChunk>::iterator MemoryList::resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo){
    std::cout << "~~~~~~~~~~Resizing chunk of size " 
    << it->size << " to get " << resizeTo << "~~~~~~~~~~" << std::endl;

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

    return it;
}