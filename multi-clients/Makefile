CC?=gcc
AR?=ar

all:first static program capteur interface

first:
	mkdir -p ./build/lib ./build/include

static: src/info.c build/include/info.h
	$(CC) -c $< -o build/info.o
	$(AR) rcs build/lib/libinfo.a build/info.o


program: src/main.c build/include/info.h  static 
	$(CC) -o build/$@_static -static $< -I./build/include -L./build/lib -linfo

capteur: src/capteur.c 
	$(CC)  -o build/$@_static -static $< -I./build/include -L./build/lib    -l pthread
	./build/program_static > info.txt 

interface:src/interface.c 
	$(CC) -o build/$@_static -static $< -I./build/include -L./build/lib 

	
clean:
	rm -rf ./build/lib 

.PHONY: static program capteur interface

