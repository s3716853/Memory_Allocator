#include <iostream>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <string>
#include <list> 
#include <vector>
#include <random>

#include "memory_manager.h"
#include "allocate_variables.cpp"

#define METHOD_LOCATION 1
#define THREAD_AMOUNT_LOCATION 2
#define MIN_ALLOC_AMOUNT_LOCATION 3
#define MAX_ALLOC_AMOUNT_LOCATION 4
#define MIN_SIZE_LOCATION 5
#define MAX_SIZE_LOCATION 6
#define SEED_LOCATION 7
#define MINIMUM_COMMAND_LINE_ARGUMENTS 7
#define EXIT_SUCCESS 0

void experiment(int argc, char ** argv, Method method);
void * allocateList(void * list);

std::vector<std::list<int>> experimentData;

int main(int argc, char ** argv){
    if(argc >= MINIMUM_COMMAND_LINE_ARGUMENTS) {
        std::string arg = argv[METHOD_LOCATION];
        if(arg == "-f"){
            experiment(argc, argv, FIRST);
        }else if(arg == "-w"){
            experiment(argc, argv, WORST);
        }else if (arg == "-b"){
            experiment(argc, argv, BEST);
        }else{
            std::cout << "Invalid memory managing type" << std::endl <<
            "{executable} -{type} {test_file_01} {test_file_02}..." << std::endl <<
            "{type} = f/b/w (first/best/worst)" << std::endl << 
            "{test_file} = files of strings to load into allocator minimum of 1 file but can include many" <<
            "Each Test file is run as its own process after the first" << std::endl;
        }
    }else{
        std::cout << "Invalid command arguments" << std::endl <<
        "{executable} -{type} {test_file_01} {test_file_02}..." << std::endl <<
        "{type} = f/b/w (first/best/worst)" << std::endl << 
        "{test_file} = files of strings to load into allocator minimum of 1 file but can include many" << std::endl;
    }

    return EXIT_SUCCESS;
}


void experiment(int argc, char ** argv, Method method){
    const char* methods[] = {"FIRST", "WORST", "BEST"};
    initilise(method);

    int seed = atoi(argv[SEED_LOCATION]);
    int noThreads = atoi(argv[THREAD_AMOUNT_LOCATION]);
    int minAllocAmount = atoi(argv[MIN_ALLOC_AMOUNT_LOCATION]);
    int maxAllocAmount = atoi(argv[MAX_ALLOC_AMOUNT_LOCATION]);
    int minSize = atoi(argv[MIN_SIZE_LOCATION]);
    int maxSize = atoi(argv[MAX_SIZE_LOCATION]);
    
    pthread_t threads[noThreads];
    //std::list<int> * experimentData[noThreads];

    std::default_random_engine engine(seed);
    std::uniform_int_distribution<int> size_dist(minSize, maxSize);
    std::uniform_int_distribution<int> alloc_dist(minAllocAmount, maxAllocAmount);

    for(int i = 0; i < noThreads+1; ++i){
        //std::cout << "LOADING DATA FOR THREAD: " << i << std::endl;
        int threadAllocAmount = alloc_dist(engine);
        std::list<int> threadData;
        for(int j = 0; j < threadAllocAmount; ++j){
            threadData.push_back(size_dist(engine));
        }
        experimentData.push_back(threadData);
    }


    // std::list<void*> memoryPointers;
    // for(int i = 0; i<10000; ++i){
    //     //std::cout << *threadNo << " : " << allocAmount << std::endl;
    //     memoryPointers.push_back(alloc(100));
    // }
    // for(void* memory: memoryPointers){
    //     dealloc(memory);
    // }

    //initilise memory for the threads to use
    int * thread = new int(0);
    allocateList((void *) thread);

    auto start = std::chrono::high_resolution_clock::now();
    clock_t cpuTime = clock();

    for(int i = 0; i < noThreads; ++i){
        int * thread = new int(i+1);
        pthread_create(&threads[i], NULL, allocateList, (void *) thread);
    }
    for(int i = 0; i < noThreads; ++i){
        pthread_join(threads[i], NULL);
    }

    cpuTime = clock() - cpuTime;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << std::endl << "----------TEST RESULTS----------" << std::endl 
    << "Experiment ran for: " << duration.count() << " microseconds" << std::endl 
    << "CPU time: " << ((float)cpuTime / CLOCKS_PER_SEC) * 1000000 << " microseconds" << std::endl;
    
    printLists();
}

void * allocateList(void * thread){
    int * threadNo = (int*) thread;
    std::list<int> allocList = experimentData[*threadNo];
    //std::cout << *threadNo << std::endl;
    std::list<void*> memoryPointers;
    for(int allocAmount: allocList){
        //std::cout << *threadNo << " : " << allocAmount << std::endl;
        memoryPointers.push_back(alloc(allocAmount));
    }
    for(void* memory: memoryPointers){
        dealloc(memory);
    }
}