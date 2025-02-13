CC = gcc
CFLAGS = -Wall -pthread -std=c99

all: MWCp

MWCp: 200104004110_main.o manager.o worker.o buffer.o
	$(CC) $(CFLAGS) -o MWCp 200104004110_main.o manager.o worker.o buffer.o

200104004110_main.o: 200104004110_main.c manager.h worker.h buffer.h
	$(CC) $(CFLAGS) -c 200104004110_main.c

manager.o: manager.c manager.h buffer.h
	$(CC) $(CFLAGS) -c manager.c

worker.o: worker.c worker.h buffer.h
	$(CC) $(CFLAGS) -c worker.c

buffer.o: buffer.c buffer.h
	$(CC) $(CFLAGS) -c buffer.c

clean:
	rm -f *.o MWCp
