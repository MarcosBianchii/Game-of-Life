CC=gcc
FLAGS=-Wall -Wextra -Werror -std=c99 -pedantic -g -lncursesw

all:
	$(CC) main.c $(FLAGS) -o main

clean:
	rm -f main
