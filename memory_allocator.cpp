#include <iostream>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <string>
#include <list> 
#include <vector>

#include "memory_manager.h"
#include "allocate_variables.cpp"

#define EXIT_SUCCESS 0
#define MINIMUM_COMMAND_LINE_ARGUMENTS 2
#define MAX_STRING_SIZE 101
#define MEMORY_INITILISER_LOCATION 2
#define EXPERIMENT_FILE_START 3


void experiment(int argc, char ** argv, Method method);
//void * readFile(void * filepath);
std::list<std::string> readFile(void* filename);
void * allocateWord(void* word);
//void * readFile(void* fileLocation, bool test);
void * test(void *);
void * allocateWord(std::string word);

std::list<std::list<std::string>> wordList;

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

    int noThreads = argc - EXPERIMENT_FILE_START;
    pthread_t threads[noThreads];

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "!!Initilising Memory With Experiment File " << argv[MEMORY_INITILISER_LOCATION] << "!!" << std::endl
    << "!!Method is " << methods[method] << " fit!!"<< std::endl;

    //readFile((void*) argv[MEMORY_INITILISER_LOCATION]);

    int threadNo = 0;
    for(int i = EXPERIMENT_FILE_START; i < argc; ++i){
        // std::cout << std::endl << "!!Running experiment file " << argv[i] << "!!" << std::endl
        // << "!!Method is " << methods[method] << " fit!!" << std::endl;
        // if(pthread_create(&threads[threadNo], NULL, readFile, (void *) argv[i]) == 0){
        //     ++threadNo;
        // }else{
        //     fprintf(stderr, "pthread_create failed with i = %d. errno = %d, %s\n",
        //         i, errno, std::strerror(errno));
        // }
        std::cout << "i = " << i << std::endl;
        wordList.push_back(readFile((void*) argv[i]));
        //test((void *) &wordList);

        int * integer= new int(0);
        pthread_create(&threads[threadNo], NULL, test, (void *) &integer);
        ++threadNo;
        // for(std::list<std::string>::iterator i = wordList.begin(); i != wordList.end(); ++i){
        //     std::cout << *i << " " << std::endl;
        // }

    }

    

    // std::cout << "BEFORE JOIN" << std::endl;

    for(int i = 0; i<noThreads; ++i){
        std::cout << "PLEASE MAKE IT HERE" << std::endl;
        pthread_join(threads[i], NULL);
        std::cout << "PLEASE MAKE IT HERE" << std::endl;
    }

    // std::cout << "AFTER JOIN" << std::endl;

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

std::list<std::string> readFile(void* filename){
    std::list<std::string> list;
    char * filepath = (char *) filename;
    std::list<char *> pointerList;
    std::ifstream myfile(filepath);
    
    int wordCount = 0;
    if (myfile.is_open()){
        while(!myfile.eof()){
            //char wordFromFile[MAX_STRING_SIZE];

            std::string wordFromFile;
            std::getline(myfile, wordFromFile);
            ++wordCount;
            
            list.push_back(wordFromFile);
            
        }
    }

    return list;
}

void * test(void * thread){
    //std::list<std::string> * wordList = (std::list<std::string> *) wordsPointer;
    int * threadNo = (int *) thread;
    std::vector<void *> allocationPointers;
    
    std::list<std::list<std::string>>::iterator list = wordList.begin();
    std::list<std::list<std::string>>::iterator end = wordList.end();
    
    std::list<std::string> allocationWordList;

    std::cout << "IT START" << std::endl;
    for(int i = 0; i < *threadNo; ++i){
        std::cout << i << std::endl;
        ++list;
    }
    std::cout << "IT STOP" << std::endl;

    allocationWordList = *list;    

    std::cout << allocationWordList.size() << std::endl;

    for(std::string word : allocationWordList){
        std::cout << "PLEASE FOR THE LOVE OF GOD WORK YOU CUNT" << std::endl;
        std::cout << word << std::endl;
        std::cout << "PLEASE FOR THE LOVE OF GOD WORK YOU CUNT 2" << std::endl;  
    }
    
    // while(word != end){
    //     std::cout << "PLEASE FOR THE LOVE OF GOD WORK YOU CUNT" << std::endl;
    //     //word = next(word);
    //     //std::cout << *word << std::endl;
    //     ++word;
    //     std::cout << "PLEASE FOR THE LOVE OF GOD WORK YOU CUNT 2" << std::endl;
    // }
    // std::cout << "LOOP ESCAPE" << std::endl;

    // for(std::list<std::string>::iterator word = wordList->begin(); word != wordList->end(); ++word){
    // //     //std::cout << "READING WORD: " << *word << std::endl;
    // //     //void * wordPointer = allocateWord(*word);
    // //     //std::cout << "READ WORD: " << (char*) wordPointer << std::endl;
    // }

    //std::cout << "FINISHED WORDS" << std::endl;

    return NULL;
}

void * allocateWord(std::string word){
    char * returnAddress = (char*) alloc(word.length()+1);
    for(int i = 0; i < word.length(); ++i){
        returnAddress[i] = word.at(i);
        std::cout << returnAddress[i] << std::endl;
    }
    returnAddress[word.length()+1] = '\0';

    return returnAddress;
}

// void * readFile(void* fileLocation){
    
//     for(int i = 0; i<1000; ++i){
//         void * pointer = alloc(100);
//         //dealloc(pointer);
//     }
//     // char * filepath = (char *) fileLocation;

//     // std::list<char *> pointerList;
//     // std::ifstream myfile(filepath);
    
//     // int wordCount = 0;
//     // if (myfile.is_open()){
//     //     while(!myfile.eof()){
//     //         char wordFromFile[MAX_STRING_SIZE];
//     //         myfile.getline(wordFromFile,MAX_STRING_SIZE,'\n');
//     //         ++wordCount;
            
//     //         int wordLength = 0;
//     //         int counter = 0;
//     //         while(wordFromFile[counter] != '\0'){
//     //             /*
//     //             this check is to fix an issue that came about
//     //             due to the fact I was making the test files on
//     //             windows but running the code on linux, 
//     //             as windows has 2 new line characters \r & \n
//     //             so running this on linux would cause issues
//     //             */
//     //             if(wordFromFile[counter] != '\r'){
//     //                 ++wordLength;
//     //             }
//     //             ++counter;
//     //         }

//     //         // std::cout << std::endl << "<<<<<<<<<<Reading word " << wordFromFile 
//     //         // << " of length " << wordLength << ">>>>>>>>>>" << std::endl
//     //         // << std::endl;

//     //         char * word = (char *) alloc(wordLength+1);
//     //         for(int i = 0; i < wordLength; ++i){
//     //             word[i] = wordFromFile[i];
//     //         }
//     //         word[wordLength+1] = '\0';

//     //         pointerList.push_back(word);
//     //     }
//     // }

//     // for(char * allocatedMemory: pointerList){
//     //     // std::cout << std::endl << "<<<<<<<<<<Deallocating word " << allocatedMemory 
//     //     // << ">>>>>>>>>>" <<std::endl << std::endl;
//     //     dealloc((void*) allocatedMemory);
//     // }

// }

// void * readFile(void* fileLocation, bool test){
    
//     // for(int i = 0; i<1000; ++i){
//     //     void * pointer = alloc(100);
//     //     //dealloc(pointer);
//     // }
//     char * filepath = (char *) fileLocation;

//     std::list<char *> pointerList;
//     std::ifstream myfile(filepath);
    
//     int wordCount = 0;
//     if (myfile.is_open()){
//         while(!myfile.eof()){
//             char wordFromFile[MAX_STRING_SIZE];
//             myfile.getline(wordFromFile,MAX_STRING_SIZE,'\n');
//             ++wordCount;
            
//             int wordLength = 0;
//             int counter = 0;
//             while(wordFromFile[counter] != '\0'){
//                 /*
//                 this check is to fix an issue that came about
//                 due to the fact I was making the test files on
//                 windows but running the code on linux, 
//                 as windows has 2 new line characters \r & \n
//                 so running this on linux would cause issues
//                 */
//                 if(wordFromFile[counter] != '\r'){
//                     ++wordLength;
//                 }
//                 ++counter;
//             }

//             // std::cout << std::endl << "<<<<<<<<<<Reading word " << wordFromFile 
//             // << " of length " << wordLength << ">>>>>>>>>>" << std::endl
//             // << std::endl;

//             char * word = (char *) alloc(wordLength+1);
//             for(int i = 0; i < wordLength; ++i){
//                 word[i] = wordFromFile[i];
//             }
//             word[wordLength+1] = '\0';

//             pointerList.push_back(word);
//         }
//     }

//     for(char * allocatedMemory: pointerList){
//         // std::cout << std::endl << "<<<<<<<<<<Deallocating word " << allocatedMemory 
//         // << ">>>>>>>>>>" <<std::endl << std::endl;
//         dealloc((void*) allocatedMemory);
//     }

// }