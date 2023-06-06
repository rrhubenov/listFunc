
CC=clang++
CXXFLAGS=-pedantic -Wall -Werror -g -std=c++11

list_func: main.o
	$(CC) -g -o run main.cpp expressions.cpp utils.cpp std.cpp values.cpp exceptions.cpp interpreter.cpp

clean:
	rm main.o run
