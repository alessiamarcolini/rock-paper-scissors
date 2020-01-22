.PHONY: build help clean bin 

CC = gcc
CFLAGS = -std=gnu90

default: help

objects = bin/utils.o bin/day bin/match bin/quarterFinals bin/quarter bin/main

bin: $(objects)

build:
	@make dummy_clean
	@mkdir -p bin
	@make bin
	@chmod +x bin/*
	@ln -s bin/main run
	@echo "Build complete"

bin/main: src/main.c bin/utils.o
	$(CC) $(CFLAGS) -o bin/main src/main.c bin/utils.o

bin/day: src/day.c bin/utils.o
	$(CC) $(CFLAGS) -o bin/day src/day.c bin/utils.o

bin/match: src/match.c 
	$(CC) $(CFLAGS) -o bin/match src/match.c

bin/quarterFinals: src/quarterFinals.c
	$(CC) $(CFLAGS) -o bin/quarterFinals src/quarterFinals.c

bin/quarter: src/quarter.c
	$(CC) $(CFLAGS) -o bin/quarter src/quarter.c

bin/utils.o: src/utils.c
	$(CC) $(CFLAGS) -c -o bin/utils.o src/utils.c
	@echo "Compiling utils"

dummy_clean:
	@rm -f run

clean:
	@rm -rf bin/
	@rm -f run
	@echo "Clean complete"

help:
	@echo "Made by Alessia Marcolini and Leonardo Berrighi"
	@echo "To start, run 'make build' and then './run'"