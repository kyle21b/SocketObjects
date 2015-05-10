CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -lpthread

OBJS = Classes.o Object.o SocketObject.o SocketObjectRuntime.o Util.o csapp.o 
CLASSES = Counter.o ResettableCounter.o CircularCounter.o

RESETTABLE = main1.o
CIRCULAR = main.o

all: circular resettable

circular: $(OBJS) $(CLASSES) $(CIRCULAR)
	$(CC) $(OBJS) $(CIRCULAR) $(CLASSES) -o circular

resettable: $(OBJS) $(CLASSES) $(RESETTABLE)
	$(CC) $(OBJS) $(CLASSES) $(RESETTABLE) -o reset

Classes.o: Classes.c
	$(CC) $(CFLAGS) -c Classes.c
Counter.o: Counter.c
	$(CC) $(CFLAGS) -c Counter.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
Object.o: Object.c
	$(CC) $(CFLAGS) -c Object.c
SocketObject.o: SocketObject.c SocketObject_Private.h
	$(CC) $(CFLAGS) -c SocketObject.c
SocketObjectRuntime.o: SocketObjectRuntime.c
	$(CC) $(CFLAGS) -c SocketObjectRuntime.c
csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c
Util.o: Util.c
	$(CC) $(CFLAGS) -c util.c
CircularCounter.o: CircularCounter.c
	$(CC) $(CFLAGS) -c CircularCounter.c
ResettableCounter.o: ResettableCounter.c
	$(CC) $(CFLAGS) -c ResettableCounter.c

clean:
	rm -f *~ *.o circular reset_server

