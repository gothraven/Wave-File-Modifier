EXEC=App.exe
SOURCES=menu.c args.c wave.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc 
CFLAGS= -w -Wall -Werror -std=c99
INCF= -lm
.PHONY: default clean
 
default: $(EXEC)

menu.o: menu.c menu.h
args.o: args.c args.h
wave.o: wave.c wave.h
main.o: main.c args.h wave.h menu.h

%.o: %.c
	@echo "\033[33mCompiling \033[1m\033[39m$@\033[21m \033[33mfrom \033[1m\033[39m$<\033[21m \033[33m"
	@$(CC) -o $@ -c $< $(CFLAGS) $(INCF)
 
$(EXEC): $(OBJECTS)
	@echo " "
	@echo "compiling is done ===> \033[1m\033[34mApp.exe\033[21m \033[32mis created"
	@echo " "
	@$(CC) -o $@ $^ $(INCF)
 
clean:
	@echo " "
	@echo "\033[9m$(EXEC) $(OBJECTS) \033[29m"
	@echo " "
	@rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~

