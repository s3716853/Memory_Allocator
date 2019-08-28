#include <iostream>
#include <sys/resource.h>
#include <unistd.h>
#include "memory_manager.h"


void test(std::string typeName);
void testAll();
void runAllTests();
void testMethod1();
void testMethod2();
void testMethod3();

int main(int argc, char ** argv){
    if(argc > 1) {
        std::string arg = argv[1];
        if(arg == "-f"){
            MemoryManager::setMethod(FIRST);
            test("FIRST FIT");
        }else if(arg == "-w"){
            MemoryManager::setMethod(WORST);
            test("WORST FIT");
        }else if (arg == "-b"){
            MemoryManager::setMethod(BEST);
            test("BEST FIT");
        }else{
            std::cout << "Invalid memory managing type" << std::endl <<
            "{executable} -{type}" << std::endl <<
            "{type} = f/b/w (first/best/worst)" << std::endl;   
        }
    }else{
        testAll();
    }
}

void test(std::string typeName){
    std::cout << "----------" << typeName << "----------" << std::endl;
    runAllTests();

    // struct rusage r_usage;
    // getrusage(RUSAGE_SELF, &r_usage);
    // std::cout << r_usage.ru_maxrss << std::endl;
}

void testAll(){
    std::cout << "----------FIRST FIT----------" << std::endl;
    MemoryManager::setMethod(FIRST);
    runAllTests();


    std::cout << std::endl << std::endl << "----------BEST FIT----------" << std::endl;
    MemoryManager::setMethod(BEST);
    runAllTests();

    std::cout << std::endl << std::endl << "----------WORST FIT----------" << std::endl;
    MemoryManager::setMethod(WORST);
    runAllTests();
}

void runAllTests(){
    testMethod1();
    testMethod2();
    testMethod3();
    //testMethod4(manager);
}

//tests creating a large memory location, small one, then deleleting both
//and then making another memory allocation of the size of the small one
void testMethod1(){
    MemoryManager::reset();
    std::cout << std::endl << "||TEST METHOD 1||" << std::endl;
    void * test = MemoryManager::alloc(10);
    void * test2 = MemoryManager::alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    MemoryManager::dealloc(test);
    MemoryManager::dealloc(test2);
    char* num = (char*)MemoryManager::alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
}

//tests creating a small memory location, large one, then deleleting both
//and then making another memory allocation of the size of the small one
void testMethod2(){
    MemoryManager::reset();
    std::cout << std::endl << "||TEST METHOD 2||" << std::endl;
    void * test = MemoryManager::alloc(sizeof(int));
    void * test2 = MemoryManager::alloc(4);
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    MemoryManager::dealloc(test);
    MemoryManager::dealloc(test2);
    char* num = (char*)MemoryManager::alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
}

//tests creating a large memory location, small one, then deleleting both
//and then making another memory allocation of the size of the small one
//then again making another allocation of size (large-small)
void testMethod3(){
    MemoryManager::reset();
    std::cout << std::endl << "||TEST METHOD 3||" << std::endl;
    void * test = MemoryManager::alloc(sizeof(int)*2);
    void * test2 = MemoryManager::alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    MemoryManager::dealloc(test);
    MemoryManager::dealloc(test2);
    char* num = (char*)MemoryManager::alloc(sizeof(int));
    char* num2 = (char*)MemoryManager::alloc(sizeof(int));
    char* num3 = (char*)MemoryManager::alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
    std::cout << (void*)num2 << std::endl;
    std::cout << (void*)num3 << std::endl;
}