CC=gcc
CFLAGS=-Wall -Wextra -Werror -O0 -g -std=c11

build:
	$(CC) $(CFLAGS) main.c exercises/basics/number_formatter.c -o app

run: build
	./app

clean:
	rm -f app
