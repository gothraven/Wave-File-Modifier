
#ifndef ARGS_H
#define ARGS_H

#define list option_t* 
#define NOOPTION ((option_t*)NULL)

typedef struct option_s {
  const char* keyword;
  enum { OptVoid, OptUnsignedInt, OptString, Opt2UnsignedInt, Opt4UnsignedInt} spec;
  union {
    void (*opt_void)();
    void (*opt_unsigned_int)(unsigned int);
    void (*opt_str)(const char*);
    void (*opt_2unsigned_int)(unsigned int,unsigned int);
    void (*opt_4unsigned_int)(unsigned int,unsigned int,unsigned int, unsigned int);
  } fct;
  struct option_s* next;
} option_t;

option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int));
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int));
option_t* opt_4unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int,unsigned int,unsigned int));
void process_arguments(option_t* l, int argc, char* *argv);


#endif