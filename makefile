# JB - 12/12/14

CC= gcc
CXX = g++
CFLAGS= -Wall -Wextra


compile: main.o
	$(CXX) $(CFLAGS) parser.o scanner.o parser_lib.o DictWrapper.o main.o -o parser -lre2
	
main.o: parser.o scanner.o parser_lib.o DictWrapper.o
	$(CXX) $(CFLAGS) -c main.cc -o main.o

DictWrapper.o: DictWrapper.cpp DictWrapper.h
	$(CXX) $(CFLAGS) -c DictWrapper.cpp -o DictWrapper.o	
	
parser_lib.o: parser_lib.cc parser_lib.h
	$(CXX) $(CFLAGS) -c parser_lib.cc -o parser_lib.o	
	
parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o
	
scanner.c: scanner.l parser.c
		flex --outfile=scanner.c scanner.l 
 
parser.c:	parser.y scanner.l
		bison -d parser.y -o parser.c

	
clean:
	rm -f parser scanner.c parser.c parser.h *.o

