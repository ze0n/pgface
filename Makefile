# installation dir
bindir = /usr/local/bin
#fdebug = -g
fdebug = 
#CC = ccmalloc g++
CC = g++

all: pgface
pgface: _nc_iface.o _pg_iface.o _pg_face.o _kurs.o
	$(CC) $(fdebug) -I/usr/local/include -L/usr/local/lib -L. _nc_iface.o _pg_iface.o _pg_face.o _kurs.o -o pgface -lcdk -lcurses -lpq
_nc_iface.o: nciface.cpp nciface.h
	$(CC) $(fdebug) -I/usr/local/include -L/usr/local/lib -c nciface.cpp -o _nc_iface.o
_pg_iface.o: pgiface.cpp pgiface.h
	$(CC) $(fdebug) -I/usr/local/include -L/usr/local/lib -c pgiface.cpp -o _pg_iface.o
_pg_face.o: pgface.cpp pgface.h
	$(CC) $(fdebug) -I/usr/local/include -L/usr/local/lib -c pgface.cpp -o _pg_face.o
_kurs.o: kurs.cpp kurs.h
	$(CC) $(fdebug) -I/usr/local/include -L/usr/local/lib -c kurs.cpp -o _kurs.o
clean:
	rm -f pgface _nc_iface.o _pg_iface.o _pg_face.o _kurs.o
install: pgface
	cp pgface $(bindir)/pgface
	cp pgface.conf $(bindir)/pgface
