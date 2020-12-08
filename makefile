CC=gcc

CFLAGS=-std=gnu11 -Wall -Wextra -pedantic-errors -Wformat -O -g

TARGET=shafa.exe

$(TARGET): shafa.o
	$(CC) $(CFLAGS) -I. shafa.o \
	-o $(TARGET)