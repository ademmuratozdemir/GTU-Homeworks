CC=gcc
CFLAGS=-I.

# Derlenecek hedefler
all: PideShop HungryVeryMuch

# PideShop hedefi
PideShop: PideShop.o
	$(CC) -o PideShop PideShop.o

PideShop.o: PideShop.c
	$(CC) -c PideShop.c $(CFLAGS)

# HungryVeryMuch hedefi
HungryVeryMuch: HungryVeryMuch.o
	$(CC) -o HungryVeryMuch HungryVeryMuch.o

HungryVeryVeryMuch.o: HungryVeryMuch.c
	$(CC) -c HungryVeryMuch.c $(CFLAGS)

# Temizlik yapmak için 'make clean' komutu
clean:
	rm -f *.o PideShop HungryVeryMuch

# Yeniden derlemek için 'make rebuild' komutu
rebuild: clean all
