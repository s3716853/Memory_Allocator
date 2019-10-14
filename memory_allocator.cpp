#include <iostream>
#include <chrono>
#include <unistd.h>
#include <fstream>

#include <list> 

#include "memory_manager.h"
#include "allocate_variables.cpp"

#define EXIT_SUCCESS 0
#define MINIMUM_COMMAND_LINE_ARGUMENTS 2
#define MAX_STRING_SIZE 101
#define MEMORY_INITILISER_LOCATION 2
#define EXPERIMENT_FILE_START 3

void experiment(int argc, char ** argv, Method method);
void * readFile(void * filepath);

int main(int argc, char ** argv){
    if(argc > MINIMUM_COMMAND_LINE_ARGUMENTS) {
        std::string arg = argv[1];
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
    setMethod(method);
    clock_t cpuTime = clock();
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "!!Initilising Memory With Experiment File " << argv[MEMORY_INITILISER_LOCATION] << "!!" << std::endl
    << "!!Method is " << methods[method] << " fit!!"<< std::endl;

    readFile((void*) argv[MEMORY_INITILISER_LOCATION]);
    
    int noThreads = argc - EXPERIMENT_FILE_START;
    pthread_t threads[noThreads];


    int threadNo = 0;
    for(int i = EXPERIMENT_FILE_START; i < argc; ++i){
        std::cout << std::endl << "!!Running experiment file " << argv[i] << "!!" << std::endl
        << "!!Method is " << methods[method] << " fit!!" << std::endl;
        pthread_create(&threads[threadNo], NULL, readFile, (void *) argv[i]);
        ++threadNo;
    }

    for(int i = 0; i<noThreads; ++i){
        pthread_join(threads[i], NULL);
    }

    cpuTime = clock() - cpuTime;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    /*
    KNOWN ISSUE WITH CPUTIME:
    Records with acuracy of 1 second, so tests that are too small will give 0 cpu time
    */
    std::cout << std::endl << "----------TEST RESULTS----------" << std::endl 
    << "Experiment ran for: " << duration.count() << " microseconds" << std::endl 
    << "CPU time: " << ((float)cpuTime / CLOCKS_PER_SEC) * 1000000 << " microseconds" << std::endl;
    // << "Total size of memory allocated by allocator: " << totalMemoryAllocatedSize() << " bytes" << std::endl 
    // << "Total chunks made: " << memoryChunkAmount() << std::endl
    // << "Average chunk size: " << totalMemoryAllocatedSize()/memoryChunkAmount() << std::endl;
}

void * readFile(void* fileLocation){
    char * filepath = (char *) fileLocation;
    //std::string filepath = *file;
    std::list<char *> pointerList;
    std::ifstream myfile(filepath);
    
    int wordCount = 0;
    if (myfile.is_open()){
        while(!myfile.eof()){
            char wordFromFile[MAX_STRING_SIZE];
            myfile.getline(wordFromFile,MAX_STRING_SIZE,'\n');
            ++wordCount;
            
            int wordLength = 0;
            int counter = 0;
            while(wordFromFile[counter] != '\0'){
                /*
                this check is to fix an issue that came about
                due to the fact I was making the test files on
                windows but running the code on linux, 
                as windows has 2 new line characters \r & \n
                so running this on linux would cause issues
                */
                if(wordFromFile[counter] != '\r'){
                    ++wordLength;
                }
                ++counter;
            }

            std::cout << std::endl << "<<<<<<<<<<Reading word " << wordFromFile 
            << " of length " << wordLength << ">>>>>>>>>>" << std::endl
            << std::endl;

            char * word = (char *) alloc(wordLength+1);
            for(int i = 0; i < wordLength; ++i){
                word[i] = wordFromFile[i];
            }
            word[wordLength+1] = '\0';

            pointerList.push_back(word);
        }
    }

    for(char * allocatedMemory: pointerList){
        std::cout << std::endl << "<<<<<<<<<<Deallocating word " << allocatedMemory 
        << ">>>>>>>>>>" <<std::endl << std::endl;
        dealloc((void*) allocatedMemory);
    }

}