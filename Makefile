EXEC=App.exe
SOURCES=menu.c args.c wave.c mylib.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc 
CFLAGS=-W -Wall -std=c99 
 
.PHONY: default clean
 
default: $(EXEC)
 
menu.o: menu.c menu.h
args.o: args.c args.h
mylib.o: mylib.c mylib.h
main.o: main.c args.h mylib.h wave.h menu.h
 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
