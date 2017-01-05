#ifndef ARGS_H
#define ARGS_H

#define list option_t*
#define NOOPTION ((option_t*)NULL)

typedef struct option_s{
  const char* keyword;
  enum { OptVoid, OptString, Opt2UnsignedInt, OptFloat} spec;
  union {
    void (*opt_void)();
    void (*opt_string)(const char*);
    void (*opt_2unsigned_int)(uint32_t,uint32_t);
    void (*opt_float)(float);
  }fct;
  struct option_s* next;
}option_t;

option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
option_t* opt_float(option_t* l, const char* kw, void (*f)(float));
option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(uint32_t,uint32_t));
void process_arguments(option_t* l, int argc, char* *argv);

#endif
