CPP_FLAGS= -std=c++11 -Werror -Wall

all: bin/main

bin/main: bin/main.o  bin/huffman.o bin/ios.o bin/pQueue.o
	g++ $(CPP_FLAGS) bin/main.o bin/huffman.o bin/ios.o bin/pQueue.o \
	-o bin/main

bin/huffman.o: src/huffman.h src/huffman.cpp src/pQueue.h src/ios.h src/tree.h
	g++ $(CPP_FLAGS) -c src/huffman.cpp -o bin/huffman.o

bin/ios.o: src/ios.cpp src/ios.h src/tree.h
	g++ $(CPP_FLAGS) -c src/ios.cpp -o bin/ios.o

bin/main.o: src/main.cpp src/huffman.h src/pQueue.h src/ios.h src/tree.h
	g++ $(CPP_FLAGS) -c  src/main.cpp -o bin/main.o

bin/pQueue.o: src/pQueue.h src/pQueue.cpp src/tree.h
	g++ $(CPP_FLAGS) -c src/pQueue.cpp -o bin/pQueue.o
	
clean:
	rm  -f bin/*
.PHONY: clean all
