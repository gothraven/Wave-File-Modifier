EXEC=App.exe
SOURCES=menu.c args.c wave.c main.c #animation.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc 
CFLAGS= -w -Wall
#INCF = -I $(HDR_PATH) -lm -lncurses
.PHONY: default clean
 
default: $(EXEC)
 
menu.o: menu.c menu.h
args.o: args.c args.h
#animation.o: animation.c animation.h
main.o: main.c args.h wave.h menu.h  
  #animation.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCF)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~

