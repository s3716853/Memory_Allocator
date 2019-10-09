#include "memory_manager.h"

void * bestFitAlloc(size_t chunk_size);
void * worstFitAlloc(size_t chunk_size);
void * firstFitAlloc(size_t chunk_size);

void addUnallocatedReaders();
void lowerUnallocatedReaders();

void addAllocatedReaders();
void lowerAllocatedReaders();

void addUnallocatedWriters();
void lowerUnallocatedWriters();

void addAllocatedWriters();
void lowerAllocatedWriters();

void writeLockUnallocatedMemory();
void writeUnlockUnallocatedMemory();
void writeLockAllocatedMemory();
void writeUnlockAllocatedMemory();

void * resizeChunk(std::list<MemoryChunk>::iterator it, size_t resizeTo);
//void printListSize();

pthread_mutex_t unallocatedMemoryReadLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t unallocatedMemoryWriteLock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t allocatedMemoryReadLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t allocatedMemoryWriteLock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t unallocatedReadersLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t allocatedReadersLock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t unallocatedWritersLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t allocatedWritersLock = PTHREAD_MUTEX_INITIALIZER;


std::list<MemoryChunk> unallocatedMemory;
std::list<MemoryChunk> allocatedMemory;
Method allocMethod = FIRST;

unsigned int unallocatedReaders = 0;
unsigned int allocatedReaders = 0;
unsigned int unallocatedWriters = 0;
unsigned int allocatedWriters = 0;

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

        addAllocatedWriters();

        writeLockAllocatedMemory();
        allocatedMemory.push_back(chunk);
        writeUnlockAllocatedMemory();

        lowerAllocatedWriters();

        std::cout << "~~~~~~~~~~Creating new memory " << returnChunk << " of size " << 
        chunk_size << "~~~~~~~~~~" << std::endl;
    }

    printListSize();

    return returnChunk;
}

void dealloc(void * chunk){

    std::list<MemoryChunk>::iterator it;
    
    bool chunkFound = false;
    addAllocatedReaders();
    while(allocatedWriters > 0);
    for(it = allocatedMemory.begin(); it != allocatedMemory.end() && !chunkFound; ++it){
        std::cout << "~~~~~~~~~~Deallocating " << chunk << " of size " << 
        it->size << "~~~~~~~~~~" << std::endl;
        if(it->address == chunk){
            lowerAllocatedReaders();
            addAllocatedWriters();
            writeLockAllocatedMemory();
            allocatedMemory.erase(it);
            writeUnlockAllocatedMemory();

            writeLockUnallocatedMemory();
            unallocatedMemory.push_back(*it);
            writeUnlockUnallocatedMemory();
            lowerAllocatedWriters();
            chunkFound = true;
        }
    }
    

    if(!chunkFound){
        std::cout << "chunk not found error :-(" << std::endl;
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
    addUnallocatedReaders();
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
    lowerUnallocatedReaders();

    if(memoryLocationFound){
        if(bestFitMemory->size == chunk_size){
            std::cout << "~~~~~~~~~~Exact chunk of size " << bestFitMemory->size << 
            " found~~~~~~~~~~" << std::endl;
            returnChunk = bestFitMemory->address;

            unallocatedMemory.erase(bestFitMemory);
            
            allocatedMemory.push_back(*bestFitMemory);
            
            
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
    //addUnallocatedReaders();
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

    addUnallocatedWriters();
    writeLockUnallocatedMemory();
    unallocatedMemory.erase(it);
    writeUnlockUnallocatedMemory();
    lowerUnallocatedWriters();

    addAllocatedWriters();
    writeLockAllocatedMemory();
    allocatedMemory.push_back(*it);
    writeUnlockAllocatedMemory();
    lowerAllocatedWriters();

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

void addUnallocatedReaders(){
    pthread_mutex_lock(&unallocatedReadersLock);
    ++unallocatedReaders;
    pthread_mutex_unlock(&unallocatedReadersLock);
}

void lowerUnallocatedReaders(){
    pthread_mutex_lock(&unallocatedReadersLock);
    --unallocatedReaders;
    pthread_mutex_unlock(&unallocatedReadersLock);
}

void addAllocatedReaders(){
    pthread_mutex_lock(&allocatedReadersLock);
    ++allocatedReaders;
    pthread_mutex_unlock(&allocatedReadersLock);
}
void lowerAllocatedReaders(){
    pthread_mutex_lock(&allocatedReadersLock);
    --allocatedReaders;
    pthread_mutex_unlock(&allocatedReadersLock);
}

void addUnallocatedWriters(){
    pthread_mutex_lock(&unallocatedWritersLock);
    ++unallocatedWriters;
    pthread_mutex_unlock(&unallocatedWritersLock);
}

void lowerUnallocatedWriters(){
    pthread_mutex_lock(&unallocatedWritersLock);
    --unallocatedWriters;
    pthread_mutex_unlock(&unallocatedWritersLock);
}

void addAllocatedWriters(){
    pthread_mutex_lock(&allocatedWritersLock);
    ++allocatedWriters;
    pthread_mutex_unlock(&allocatedWritersLock);
}
void lowerAllocatedWriters(){
    pthread_mutex_lock(&allocatedWritersLock);
    --allocatedWriters;
    pthread_mutex_unlock(&allocatedWritersLock);
}

void writeLockUnallocatedMemory(){
    pthread_mutex_lock(&unallocatedMemoryReadLock);
    pthread_mutex_lock(&unallocatedMemoryWriteLock);
    while(unallocatedReaders!=0);
}

void writeUnlockUnallocatedMemory(){
    pthread_mutex_unlock(&unallocatedMemoryReadLock);
    pthread_mutex_unlock(&unallocatedMemoryWriteLock);
}

void writeLockAllocatedMemory(){
    pthread_mutex_lock(&allocatedMemoryReadLock);
    pthread_mutex_lock(&allocatedMemoryWriteLock);
    while(allocatedReaders!=0);
}

void writeUnlockAllocatedMemory(){
    pthread_mutex_unlock(&allocatedMemoryReadLock);
    pthread_mutex_unlock(&allocatedMemoryWriteLock);
}