CC?=cc
LIBS=-lexpat
OBJS=fb2ms.o handler.o element.o util.o

fb2ms: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o fb2ms

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f fb2ms $(OBJS)
