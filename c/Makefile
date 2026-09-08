CC=gcc
CFLAGS=-Wall -Wextra -Werror -O0 -g -std=c11
CDIRS=-Iexercises/basics

build:
	$(CC) $(CFLAGS) $(CDIRS) main.c exercises/basics/str_reverser.c -o app

run: build
	./app

clean:
	rm -f app
