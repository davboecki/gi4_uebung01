CC = gcc

.PHONY: all default clean

default: myecho myenv isset

all: myecho myenv isset

myecho: myecho.c

myenv: myenv.c

isset: isset.c

clean:
	rm -f myecho
	rm -f myenv
	rm -f isset