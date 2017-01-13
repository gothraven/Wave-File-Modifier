#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
	li->fct.opt_string = f;
	li->next = l;
	return li;
}

option_t* opt_unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int)){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = OptUnsignedInt;
	li->fct.opt_unsigned_int = f;
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
                                        case OptUnsignedInt: nb1 = atoi(argv[i+1]);
                                                             cursor->fct.opt_unsigned_int((unsigned int)nb1); 
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

void lunchOptions(int argc, char** argv){

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
        opt = opt_unsignedInt(opt, "-vp",optVolumeUp);
        opt = opt_unsignedInt(opt, "-vd",optVolumeDown);
        opt = opt_void(opt, "-help",optHelp); 

	process_arguments(opt, argc, argv);

	free(opt);

}
