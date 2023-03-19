CC=g++  
CXXFLAGS = -std=c++0x
CFLAGS=-I
# skiplist.o:skiplist.cpp
# 	g++ -c skiplist.cpp
all:
	g++ -o main main.cpp --std=c++17 -pthread
#	rm -f ./*.o

clean: 
	rm -f ./*.o
	rm -f main
