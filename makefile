all: maptel.o

maptel.o: maptel.cc maptel.h
	g++ -c -DEBUG -Wall -Wextra -O2 -std=c++17 maptel.cc -o maptel.o