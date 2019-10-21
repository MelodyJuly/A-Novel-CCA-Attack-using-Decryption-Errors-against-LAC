PREFLAG = -fPIC -O3 -mavx2 -fomit-frame-pointer  -funroll-loops -std=c11
PREFLAGSO = -shared -O3 -mavx2 -fomit-frame-pointer -funroll-loops -std=c11
OBJ = Lacv2.o
CC = gcc

all: Lacv2

Lacv2: Lacv2.c random.h
	${CC} ${PREFLAG} -c Lacv2.c -o Lacv2.o
	${CC} ${PREFLAGSO} Lacv2.o -o Lacv2.so
