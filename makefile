.default: test

allocator: memory_manager.o memory_chunk.o memory_allocator.o method.o memory_list.o
	g++ -pthread -std=c++17 -O -o $@ $^


debug: memory_manager.o memory_chunk.o memory_allocator.o method.o memory_list.o
	g++ -g -pthread -std=c++17 -O -o $@ $^

clean:
	rm -f allocator debug *.o

%.0: %.cpp
	g++ -std=c++17 -O -c $^