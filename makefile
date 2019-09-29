.default: all

all: allocator

clean:
	rm -f allocator *.o

allocator: memory_manager.o memory_chunk.o memory_allocator.o method.o
	g++ -pthread -Wall -Werror -std=c++17 -O -o $@ $^

%.0: %.cpp
	g++ -pthread -Wall -Werror -std=c++17 -O -c $^