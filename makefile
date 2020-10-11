#CFLAGS=-g -fno-diagnostics-color
CC=cc
LIBS=-lexpat

fb2ms: fb2ms.o handler.o element.o util.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o fb2ms

fb2ms.o: fb2ms.c handler.h element.h
	$(CC) $(CFLAGS) -c fb2ms.c

element.o: element.c element.h
	$(CC) $(CFLAGS) -c element.c

handler.o: handler.c handler.h element.h
	$(CC) $(CFLAGS) -c handler.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c
	

clean:
	rm -f fb2ms *.o
