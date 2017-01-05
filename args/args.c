#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "args.h"

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
	li->fct.opt_str = f;
	li->next = l;
	return li;
}

option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(uint32_t,uint32_t)){
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

void process_arguments(option_t* l, int argc, char* *argv){
	int i;
	uint32_t nb1, nb2;
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
					case OptString: cursor->fct.opt_str(argv[i+1]);
					break;
                                        case OptFloat: flo = (float) atof(argv[i+1]);
					               cursor->fct.opt_unsigned_int(flo);
						       i++;
					break;
					case Opt2UnsignedInt: nb1 = (uint32_t) atoi(argv[i+1]);
							      nb2 = (uint32_t) atoi(argv[i+2]);
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

void lunchOptions(){

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
  opt = opt_float(opt, "-s", wave_scale);
  opt = opt_float(opt, "-scale", wave_scale);

  process_arguments(opt, argc, argv);

  free(opt);

}
