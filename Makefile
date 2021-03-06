#Makefile
CPP = g++
FLAG = -std=c++11
SOURCE = main.cpp
OBJ = main.o
TXT = write.txt output.txt

all: SRI

SRI: main.o
	$(CPP) -o SRI -g $(SOURCE) $(FLAG)
main.o: main.cpp
	$(CPP) -c $(SOURCE) $(FLAG)
clean:
	rm -f SRI $(OBJ) $(TXT)