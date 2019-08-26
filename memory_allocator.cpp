#include <iostream>
#include <unistd.h>
#include "memory_manager.h"


void test(MemoryManager * manager);
void runAllTests(MemoryManager * manager);
void testMethod1(MemoryManager * manager);
void testMethod2(MemoryManager * manager);
void testMethod3(MemoryManager * manager);
void testMethod4(MemoryManager * manager);

int main(int argc, char *argv[]){
    
    MemoryManager * manager = new MemoryManager();
    test(manager);
}

void test(MemoryManager * manager){
    std::cout << "----------FIRST FIT----------" << std::endl;
    manager->setFirstFit();
    runAllTests(manager);

    std::cout << std::endl << std::endl << "----------BEST FIT----------" << std::endl;
    manager->setBestFit();
    runAllTests(manager);

    std::cout << std::endl << std::endl << "----------WORST FIT----------" << std::endl;
    manager->setWorstFit();
    runAllTests(manager);
}

void runAllTests(MemoryManager * manager){
    testMethod1(manager);
    testMethod2(manager);
    testMethod3(manager);
    testMethod4(manager);
}

//tests creating a large memory location, small one, then deleleting both
//and then making another memory allocation of the size of the small one
void testMethod1(MemoryManager * manager){
    manager->reset();
    std::cout << std::endl << "||TEST METHOD 1||" << std::endl;
    void * test = manager->alloc(10);
    void * test2 = manager->alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    manager->dealloc(test);
    manager->dealloc(test2);
    char* num = (char*)manager->alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
}

//tests creating a small memory location, large one, then deleleting both
//and then making another memory allocation of the size of the small one
void testMethod2(MemoryManager * manager){
    manager->reset();
    std::cout << std::endl << "||TEST METHOD 2||" << std::endl;
    void * test = manager->alloc(sizeof(int));
    void * test2 = manager->alloc(4);
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    manager->dealloc(test);
    manager->dealloc(test2);
    char* num = (char*)manager->alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
}

//tests creating a large memory location, small one, then deleleting both
//and then making another memory allocation of the size of the small one
//then again making another allocation of size (large-small)
void testMethod3(MemoryManager * manager){
    manager->reset();
    std::cout << std::endl << "||TEST METHOD 3||" << std::endl;
    void * test = manager->alloc(sizeof(int)*2);
    void * test2 = manager->alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    manager->dealloc(test);
    manager->dealloc(test2);
    char* num = (char*)manager->alloc(sizeof(int));
    char* num2 = (char*)manager->alloc(sizeof(int));
    char* num3 = (char*)manager->alloc(sizeof(int));
    std::cout << (void*)num << std::endl;
    std::cout << (void*)num2 << std::endl;
    std::cout << (void*)num3 << std::endl;
}

//
void testMethod4(MemoryManager * manager){
    manager->reset();
    std::cout << std::endl << "||TEST METHOD 4||" << std::endl;
    void * test = manager->alloc(40);
    void * test2 = manager->alloc(sizeof(int));
    std::cout << test << std::endl;
    std::cout << test2 << std::endl;
    manager->dealloc(test);
    manager->dealloc(test2);
    char* num = (char*)manager->alloc(sizeof(int));
    char* num2 = (char*)manager->alloc(sizeof(int));
    char* num3 = (char*)manager->alloc(20);
    std::cout << (void*)num << std::endl;
    std::cout << (void*)num2 << std::endl;
    std::cout << (void*)num3 << std::endl;
}