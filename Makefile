
all: build

build: Dictionary.o Main.o
	g++ -O3 -lm Main.o Dictionary.o -o tema1

.cpp.o:
	g++ -O3 -funroll-loops -c -Wall -c $? 

clean: 
	-rm -f *.o tema1
run:
	./tema1
	
.PHONY: all clean

