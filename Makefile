CC = gcc

.PHONY: all default clean

default: myecho myenv isset myexec myshell

all: myecho myenv isset myexec myshell

myecho: myecho.c

myenv: myenv.c

isset: isset.c

myexec: myexec.c

myshell: myshell.c
	gcc -Wall myshell.c -o myshell -lreadline

clean:
	rm -f myecho
	rm -f myenv
	rm -f isset
	rm -f myexec
	rm -f myshell
