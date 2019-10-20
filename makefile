CC=gcc
CFLAGS=-I -Wall
OUTPUT_FILE=shell
SOURCE_FILES=main.c util.c exec.c

compile:
	$(CC) -o $(OUTPUT_FILE) $(SOURCE_FILES) $(CFLAGS)
	
run: compile
	./$(OUTPUT_FILE)
	
clean:
	rm shell

all: compile run clean
