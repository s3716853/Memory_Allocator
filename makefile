.default: test

allocator: memory_manager.o memory_chunk.o memory_allocator.o method.o memory_list.o
	g++ -pthread -std=c++17 -O -o $@ $^

test: memory_manager.o memory_chunk.o memory_allocator.o method.o memory_list.o
	g++ -pthread -Wall -Werror -std=c++17 -O -o $@ $^

debug: memory_manager.o memory_chunk.o memory_allocator.o method.o memory_list.o
	g++ -g -pthread -Wall -Werror -std=c++17 -O -o $@ $^

clean:
	rm -f allocator test debug *.o

%.0: %.cpp
	g++ -pthread -std=c++17 -O -c $^