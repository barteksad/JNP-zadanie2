all: maptel.o

maptel.o: maptel.cc maptel.h
	g++ -c -DNDEBUG -Wall -Wextra -O2 -std=c++17 maptel.cc -o maptel.o