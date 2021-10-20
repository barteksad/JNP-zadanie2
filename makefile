all: maptel_test1 maptel_test21 maptel_test22

maptel.o: maptel.cc maptel.h
	g++ -c -DEBUG -Wall -Wextra -O2 -std=c++17 maptel.cc -o maptel.o
maptel_test1.o: maptel_test1.c
	gcc -c -Wall -Wextra -O2 -std=c11 maptel_test1.c -o maptel_test1.o
maptel_test2.o: maptel_test2.cc
	g++ -c -Wall -Wextra -O2 -std=c++17 maptel_test2.cc -o maptel_test2.o
maptel_test1: maptel_test1.o maptel.o
	g++ maptel_test1.o maptel.o -o maptel_test1
maptel_test21: maptel_test2.o maptel.o
	g++ maptel_test2.o maptel.o -o maptel_test21
maptel_test22: maptel.o maptel_test2.o
	g++ maptel.o maptel_test2.o -o maptel_test22




clean:
	rm -f *.o line
