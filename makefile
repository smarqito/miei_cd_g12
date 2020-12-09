CC=gcc

CFLAGS=-std=gnu11 -Wall -Wextra -pedantic-errors -Wformat -O -g

TARGET=shafa.exe

$(TARGET): shafa.o module_t.o
	$(CC) $(CFLAGS) -I. shafa.o module_t.o \
	-o $(TARGET)

shafa.o: shafa.c
	gcc -c shafa.c

module_t.o: ./module_t/module_t.c ./module_t/module_t.h
	gcc -c ./module_t/module_t.c