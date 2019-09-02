#include <iostream>
#include <chrono>
#include <unistd.h>
#include <fstream>

#include "memory_manager.h"

#define MINIMUM_COMMAND_LINE_ARGUMENTS 2
#define MAX_STRING_SIZE 100
#define FILE_ARGUMENT_START 2

void experiment(int argc, char ** argv, Method method);
void readFile(std::string filepath);

int main(int argc, char ** argv){
    if(argc > MINIMUM_COMMAND_LINE_ARGUMENTS) {
        std::string arg = argv[1];
        //bool methodSet = true;
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
            "{test_file} = files of strings to load into allocator minimum of 1 file but can include many" << std::endl;
        }
    }else{
        std::cout << "Invalid command arguments" << std::endl <<
        "{executable} -{type} {test_file_01} {test_file_02}..." << std::endl <<
        "{type} = f/b/w (first/best/worst)" << std::endl << 
        "{test_file} = files of strings to load into allocator minimum of 1 file but can include many" << std::endl;
    }
}

void experiment(int argc, char ** argv, Method method){
    const char* methods[] = {"FIRST", "WORST", "BEST"};
    setMethod(method);
    clock_t cpuTime = clock();
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = FILE_ARGUMENT_START; i < argc; ++i){
        std::cout << "!!Running experiment file " << argv[i] << "!!" << std::endl
        << "!!Method is " << methods[method] << " fit!!"<< std::endl;
        readFile(argv[i]);
    }

    cpuTime = clock() - cpuTime;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << std::endl << "----------TEST RESULTS----------" << std::endl 
    << "Experiment ran for: " << duration.count() << " microseconds" << std::endl 
    << "CPU time: " << ((float)cpuTime / CLOCKS_PER_SEC) * 1000000 << " microseconds" << std::endl
    << "Total size of memory allocated by allocator: " << totalMemoryAllocatedSize() << " bytes" << std::endl 
    << "Total chunks made: " << memoryChunkAmount() << std::endl
    << "Average chunk size: " << totalMemoryAllocatedSize()/memoryChunkAmount() << std::endl;
}

void readFile(std::string filepath){

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