CC = gcc

.PHONY: all default clean

default: myecho myenv

all: myecho myenv

myecho: myecho.c

myenv: myenv.c

clean:
	rm -f myecho
	rm -f myenv