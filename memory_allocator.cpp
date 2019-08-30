#include <iostream>
#include <sys/resource.h>
#include <unistd.h>
#include "memory_manager.h"
#include <fstream>


void test(std::string typeName);
void runAllTests();
void testMethod1();
void testMethod2();
void testMethod3();
void testMethod4();
char** readFile(std::string filepath);

int main(int argc, char ** argv){
    readFile("experimental_input_01");
    if(argc > 1) {
        std::string arg = argv[1];
        if(arg == "-f"){
            setMethod(FIRST);
            test("FIRST FIT");
        }else if(arg == "-w"){
            setMethod(WORST);
            test("WORST FIT");
        }else if (arg == "-b"){
            setMethod(BEST);
            test("BEST FIT");
        }else{
            std::cout << "Invalid memory managing type" << std::endl <<
            "{executable} -{type}" << std::endl <<
            "{type} = f/b/w (first/best/worst)" << std::endl;   
        }
    }else{
        std::cout << "Specify method" << std::endl <<
        "{executable} -{type}" << std::endl <<
        "{type} = f/b/w (first/best/worst)" << std::endl; 
    }
}

void test(std::string typeName){
    std::cout << "----------" << typeName << "----------" << std::endl;
    runAllTests();

    // struct rusage r_usage;
    // getrusage(RUSAGE_SELF, &r_usage);
    // std::cout << r_usage.ru_maxrss << std::endl;
}

void runAllTests(){
    // testMethod1();
    // testMethod2();
    // testMethod3();
    testMethod4();
}

//tests creating a large memory location, small one, then deleleting both
//and then making another memory allocation of the size of the small one
void testMethod1(){
    reset();
    std::cout << std::endl << "||TEST METHOD 1||" << std::endl;
    void * test = alloc(10);
    void * test2 = alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    dealloc(test);
    dealloc(test2);
    char* num = (char*)alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
}

//tests creating a small memory location, large one, then deleleting both
//and then making another memory allocation of the size of the small one
void testMethod2(){
    reset();
    std::cout << std::endl << "||TEST METHOD 2||" << std::endl;
    void * test = alloc(sizeof(int));
    void * test2 = alloc(4);
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    dealloc(test);
    dealloc(test2);
    char* num = (char*)alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
}

//tests creating a large memory location, small one, then deleleting both
//and then making another memory allocation of the size of the small one
//then again making another allocation of size (large-small)
void testMethod3(){
    reset();
    std::cout << std::endl << "||TEST METHOD 3||" << std::endl;
    void * test = alloc(sizeof(int)*2);
    void * test2 = alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    dealloc(test);
    dealloc(test2);
    char* num = (char*)alloc(sizeof(int));
    char* num2 = (char*)alloc(sizeof(int));
    char* num3 = (char*)alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
    std::cout << (void*)num2 << std::endl;
    std::cout << (void*)num3 << std::endl;
}

void testMethod4(){
    std::cout << std::endl << "||TEST METHOD 4||" << std::endl;

    bool inputCont = true;
    
    int count = 0;
    //char[] inputArray[];
    
    // while(inputCont){
    //     char input[100];
    //     std::cin >> input;

    //     // inputArray[count] = input;
    //     // ++cout
    // }


    

    // char hello[] = "hello";
    // char * helloMemPoint = (char*) alloc(sizeof(hello));
    // helloMemPoint = hello;
    // for(int i =0; i < 7; ++i){
    //     std::cout << helloMemPoint[i];
    // }
    
}

char** readFile(std::string filepath){
    //std::string line = "";
    std::ifstream myfile(filepath);
    char stringArray[100][50]; 
    int wordCount = 0;
    
    char arr[105];

    if (myfile.is_open()){
        while(!myfile.eof()){
            myfile.getline(stringArray[wordCount],100,'\n');
            ++wordCount;
        }   
        
    }

    for(int i = 0; i< 50; ++i){
        std::cout << stringArray[i] << std::endl;
    }

    return nullptr;
}