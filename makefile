CC=gcc

CFLAGS=-std=gnu11 -Wall -Wextra -pedantic-errors -Wformat -O -g

TARGET=shafa.exe

$(TARGET): shafa.o module_t.o module_f.o module_d.o module_c.o RWCod.o RWShafa.o SFCodes.o
	$(CC) $(CFLAGS) -I. shafa.o module_t.o module_f.o module_d.o module_c.o RWCod.o RWShafa.o SFCodes.o \
	-o $(TARGET)

shafa.o: shafa.c
	gcc -c shafa.c

module_t.o: ./module_t/module_t.c ./module_t/module_t.h
	gcc -c ./module_t/module_t.c

module_f.o: ./module_f/module_f.c ./module_f/module_f.h
	gcc -c ./module_f/module_f.c

module_d.o: ./module_d/module_d.c ./module_d/module_d.h
	gcc -c ./module_d/module_d.c

module_c.o: ./module_c/module_c.c ./module_c/module_c.h
	gcc -c ./module_c/module_c.c

RWCod.o: ./module_c/headers/RWCod.h ./module_c/src/RWCod.c
	gcc -c ./module_c/src/RWCod.c

RWShafa.o: ./module_c/headers/RWShafa.h ./module_c/src/RWShafa.c
	gcc -c ./module_c/src/RWShafa.c

SFCodes.o: ./module_c/headers/SFCodes.h ./module_c/src/SFCodes.c
	gcc -c ./module_c/src/SFCodes.c

clean:
	rm *.o 