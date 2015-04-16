CC = gcc

.PHONY: all default clean

default: myecho myenv isset myexec

all: myecho myenv isset myexec

myecho: myecho.c

myenv: myenv.c

isset: isset.c

myexec: myexec.c

clean:
	rm -f myecho
	rm -f myenv
	rm -f isset
	rm -f myexec
