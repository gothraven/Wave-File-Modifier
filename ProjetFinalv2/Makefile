EXEC=app.exe
SOURCES=main.c image.c menu.c args.c -lm
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -g

.PHONY: default clean

default: $(EXEC)

args.o: args.c args.h
image.o: image.c image.h
menu.o: menu.c menu.h
main.o: main.c menu.h image.h args.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
