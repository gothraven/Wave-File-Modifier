#ifndef ARGS_H
#define ARGS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define list option_t*
#define NOOPTION ((option_t*)NULL)

typedef struct option_s{
	const char* keyword;
	enum { OptVoid, OptString, Opt2UnsignedInt, OptUnsignedInt, OptFloat} spec;
	union {
		void (*opt_void)();
		void (*opt_string)(const char*);
                void (*opt_unsigned_int)(unsigned int);
		void (*opt_2unsigned_int)(unsigned int,unsigned int);
		void (*opt_float)(float);
	}fct;
	struct option_s* next;
}option_t;

option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
option_t* opt_unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int));
option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int));
option_t* opt_float(option_t* l, const char* kw, void (*f)(float));
void process_arguments(option_t* l, int argc, char* *argv);
void lunchOptions();

void optOpen(const char *fname);
void optSave(const char *fname);
void optInfo();
void optReverse();
void optCrop(unsigned int a,unsigned int b);
void optScale(float f);
void optVolumeUp(unsigned int v);
void optVolumeDown(unsigned int v);
void optHelp();


#endif
