CC=gcc

CFLAGS=-std=gnu11 -Wall -Wextra -pedantic-errors -Wformat -O -g

TARGET=shafa.exe

$(TARGET): module_t.o
	$(CC) $(CFLAGS) -I. module_t.o \
	-o $(TARGET)

module_t.o: module_t.c module_t.h
	gcc -c module_t.c