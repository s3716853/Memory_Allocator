.default: all
all: allocator
clean:
	rm -f allocator *.o
allocator: memory_chunk.o memory_allocator.o
	g++ -Wall -Werror -std=c++14 -O -o $@ $^
%.0: %.cpp
	g++ -Wall -Werror -std=c++14 -O -c $^