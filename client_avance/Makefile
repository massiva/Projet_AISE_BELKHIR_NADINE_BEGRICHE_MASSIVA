CC?=gcc
AR?=ar

all: first capteur interface

first:
	mkdir -p ./build

capteur: src/capteur.c 
	$(CC)  -o build/$@  $<     
	

interface:src/interface.c 
	$(CC) -o build/$@   $<  -l pthread

	
clean:
	rm -rf ./build

.PHONY:  first capteur interface

