EXEC=App.exe
SOURCES=menu.c args.c wave.c main.c animation.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc 
CFLAGS= -w -Wall
INCF= -lm -lncurses
.PHONY: default clean
 
default: $(EXEC)
 
menu.o: menu.c menu.h
args.o: args.c args.h
wave.o: wave.c wave.h
animation.o: animation.c animation.h
main.o: main.c args.h wave.h menu.h animation.h
%.o: %.c
	@echo "\033[32mCompiling $@ from $< \033[39m"
	@$(CC) -o $@ -c $< $(CFLAGS) $(INCF)
 
$(EXEC): $(OBJECTS)
	@echo "compiling done \033[34mApp.exe\033[39m created"
	@$(CC) -o $@ $^ $(INCF)
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~

