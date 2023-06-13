CC?=cc
LIBS=-lexpat
OBJS=fb2ms.o handler.o element.o util.o
PREFIX?=/usr/local

fb2ms: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o fb2ms

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

install: fb2ms
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f fb2ms $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/fb2ms

clean:
	rm -f fb2ms $(OBJS)
