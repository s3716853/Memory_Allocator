#include <iostream>
#include <sys/resource.h>
#include <unistd.h>
#include <fstream>

#include "memory_manager.h"

#define MINIMUM_COMMAND_LINE_ARGUMENTS 2
#define TEST_FOLDER "tests/"
#define MAX_STRING_SIZE 100

void readFile(std::string filepath);

int main(int argc, char ** argv){
    if(argc > MINIMUM_COMMAND_LINE_ARGUMENTS) {
        std::string arg = argv[1];
        bool methodSet = true;
        if(arg == "-f"){
            setMethod(FIRST);
        }else if(arg == "-w"){
            setMethod(WORST);
        }else if (arg == "-b"){
            setMethod(BEST);
        }else{
            std::cout << "Invalid memory managing type" << std::endl <<
            "{executable} -{type} {test_file_01} {test_file_02}..." << std::endl <<
            "{type} = f/b/w (first/best/worst)" << std::endl << 
            "{test_file} = files of strings to load into allocator minimum of 1 file but can include many" << std::endl;
            methodSet = false;
        }
        if(methodSet){
            for(int i = 2; i < argc; ++i){
                readFile(argv[i]);
            }
        }
    }else{
        std::cout << "Invalid command arguments" << std::endl <<
        "{executable} -{type} {test_file_01} {test_file_02}..." << std::endl <<
        "{type} = f/b/w (first/best/worst)" << std::endl << 
        "{test_file} = files of strings to load into allocator minimum of 1 file but can include many" << std::endl;
    }

    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    std::cout << r_usage.ru_utime << std::endl;
}

void readFile(std::string filepath){

    std::cout << "!!Running experiment file " << filepath << "!!" << std::endl << std::endl;
    std::list<char *> pointerList;
    std::ifstream myfile(TEST_FOLDER + filepath);
    
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

            std::cout << word << std::endl;

            

            pointerList.push_back(word);
        }
    }

    for(char * allocatedMemory: pointerList){
        std::cout << std::endl << "<<<<<<<<<<Deallocating word " << allocatedMemory 
        << ">>>>>>>>>>" <<std::endl << std::endl;
        dealloc((void*) allocatedMemory);
    }

}

// void test(std::string typeName){
//     std::cout << "----------" << typeName << "----------" << std::endl;
//     runAllTests();

//     // struct rusage r_usage;
//     // getrusage(RUSAGE_SELF, &r_usage);
//     // std::cout << r_usage.ru_maxrss << std::endl;
// }

// void runAllTests(){
//     // testMethod1();
//     // testMethod2();
//     // testMethod3();
//     testMethod4();
// }

// //tests creating a large memory location, small one, then deleleting both
// //and then making another memory allocation of the size of the small one
// void testMethod1(){
//     reset();
//     std::cout << std::endl << "||TEST METHOD 1||" << std::endl;
//     void * test = alloc(10);
//     void * test2 = alloc(sizeof(int));
//     std::cout << test << std::endl;
//     std::cout << test2 << std::endl;
//     dealloc(test);
//     dealloc(test2);
//     char* num = (char*)alloc(sizeof(int));
//     std::cout << (void*)num << std::endl;
// }

// //tests creating a small memory location, large one, then deleleting both
// //and then making another memory allocation of the size of the small one
// void testMethod2(){
//     reset();
//     std::cout << std::endl << "||TEST METHOD 2||" << std::endl;
//     void * test = alloc(sizeof(int));
//     void * test2 = alloc(4);
//     std::cout << test << std::endl;
//     std::cout << test2 << std::endl;
//     dealloc(test);
//     dealloc(test2);
//     char* num = (char*)alloc(sizeof(int));
//     std::cout << (void*)num << std::endl;
// }

// //tests creating a large memory location, small one, then deleleting both
// //and then making another memory allocation of the size of the small one
// //then again making another allocation of size (large-small)
// void testMethod3(){
//     reset();
//     std::cout << std::endl << "||TEST METHOD 3||" << std::endl;
//     void * test = alloc(sizeof(int)*2);
//     void * test2 = alloc(sizeof(int));
//     std::cout << test << std::endl;
//     std::cout << test2 << std::endl;
//     dealloc(test);
//     dealloc(test2);
//     char* num = (char*)alloc(sizeof(int));
//     char* num2 = (char*)alloc(sizeof(int));
//     char* num3 = (char*)alloc(sizeof(int));
//     std::cout << (void*)num << std::endl;
//     std::cout << (void*)num2 << std::endl;
//     std::cout << (void*)num3 << std::endl;
// }

// void testMethod4(){
//     //std::cout << std::endl << "||TEST METHOD 4||" << std::endl;

//     bool inputCont = true;
    
//     int count = 0;
//     //char[] inputArray[];
    
//     // while(inputCont){
//     //     char input[100];
//     //     std::cin >> input;

//     //     // inputArray[count] = input;
//     //     // ++cout
//     // }


    

//     // char hello[] = "hello";
//     // char * helloMemPoint = (char*) alloc(sizeof(hello));
//     // helloMemPoint = hello;
//     // for(int i =0; i < 7; ++i){
//     //     std::cout << helloMemPoint[i];
//     // }
    
// }