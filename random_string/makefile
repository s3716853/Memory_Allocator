.default: all

all: string_gen

clean:
	rm -f allocator *.o

string_gen: random_string_gen.o
	g++ -Wall -Werror -std=c++17 -O -o $@ $^

%.0: %.cpp
	g++ -Wall -Werror -std=c++17 -O -c $^