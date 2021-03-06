#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define list option_t*
#define NOOPTION ((option_t*)NULL)

typedef struct option_s{
	const char* keyword;
	enum { OptVoid, OptString, Opt2UnsignedInt, OptFloat} spec;
	union {
		void (*opt_void)();
		void (*opt_string)(const char*);
		void (*opt_2unsigned_int)(unsigned int,unsigned int);
		void (*opt_float)(float);
	}fct;
	struct option_s* next;
}option_t;

option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
option_t* opt_float(option_t* l, const char* kw, void (*f)(float));
option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int));
void process_arguments(option_t* l, int argc, char* *argv);
void lunchOptions();

void optOpen(const char *fname){
  printf("%s opned\n",fname);
}
void optSave(const char *fname){
  printf("%s saved\n",fname);
}
void optInfo(){
  printf("info\n");
}
void optReverse(){
  printf("reversed\n");
}
void optCrop(unsigned int a,unsigned int b){
  printf("cropped from %u to %u\n",a,b);
}
void optScale(float f){
  printf("scaled with %f\n",f);
}
void optHelp(){
 printf("help\n");
}


option_t* opt_void(option_t* l, const char* kw, void (*f)()){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = OptVoid;
	li->fct.opt_void = f;
	li->next = l;
	return li;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = OptString;
	li->fct.opt_string = f;
	li->next = l;
	return li;
}

option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int)){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = Opt2UnsignedInt;
	li->fct.opt_2unsigned_int = f;
	li->next = l;
	return li;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float)){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = OptFloat;
	li->fct.opt_float = f;
	li->next = l;
	return li;
}

void process_arguments(option_t* l, int argc, char ** argv){
	int i;
	unsigned int nb1, nb2;
	float flo;
	list cursor;
	printf("\n");
	for(i = 1; i < argc ; i+=2){
		cursor = l;
		while(cursor != NULL){
			if(strcmp(cursor->keyword, argv[i]) == 0){
				switch (cursor->spec){
					case OptVoid: cursor->fct.opt_void();
						      i--;
						      break;
					case OptString: cursor->fct.opt_string(argv[i+1]);
							break;
					case OptFloat: flo =(float) atof(argv[i+1]);
						       cursor->fct.opt_float(flo);
						       i+=2;
						       break;
					case Opt2UnsignedInt: nb1 =(unsigned int) atoi(argv[i+1]);
							      nb2 =(unsigned int) atoi(argv[i+2]);
							      cursor->fct.opt_2unsigned_int(nb1,nb2);
							      i++;
							      break;
					default: printf("Arguments not found");
						 exit(EXIT_FAILURE);
				}
			}
			cursor = cursor->next;
		}
	}
}

int main(int argc, char** argv){

	option_t* opt = NOOPTION;

	opt = opt_string(opt, "-i", optOpen);
	opt = opt_string(opt, "-open", optOpen);
	opt = opt_string(opt, "-o", optSave);
	opt = opt_string(opt, "-save", optSave);
	opt = opt_void(opt, "-p", optInfo);
	opt = opt_void(opt, "-print", optInfo);
	opt = opt_void(opt, "-r", optReverse);
	opt = opt_void(opt, "-reverse", optReverse);
	opt = opt_2unsignedInt(opt, "-c", optCrop);
	opt = opt_2unsignedInt(opt, "-crop", optCrop);
	opt = opt_float(opt, "-s",optScale);
	opt = opt_float(opt, "-scale",optScale);
        opt = opt_void(opt, "-h",optHelp);
        opt = opt_void(opt, "-help",optHelp); 

	process_arguments(opt, argc, argv);

	free(opt);
    return 0;
}
