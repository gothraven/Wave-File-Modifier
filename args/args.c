#include "args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

option_t* opt_2unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int)){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = Opt2UnsignedInt;
	li->fct.opt_2unsigned_int = f;
	li->next = l;
	return li;
}

option_t* opt_3unsignedInt(option_t* l, const char* kw, void (*f)(unsigned int,unsigned int,unsigned int)){
	list li = (list)malloc(sizeof(option_t));
	li->keyword = kw;
	li->spec = Opt3UnsignedInt;
	li->fct.opt_3unsigned_int = f;
	li->next = l;
	return li;
}



void process_arguments(option_t* l, int argc, char* *argv){
	int i;
	unsigned int nb1, nb2, nb3;
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
					case Opt2UnsignedInt: nb1 = (unsigned int) atoi(argv[i+1]);
										  nb2 = (unsigned int) atoi(argv[i+2]);
										  cursor->fct.opt_2unsigned_int(nb1,nb2);
										  i++;
					break;
					case Opt3UnsignedInt: nb1 = (unsigned int) atoi(argv[i+1]);
										  nb2 = (unsigned int) atoi(argv[i+2]);
										  nb3 = (unsigned int) atoi(argv[i+3]);
										  cursor->fct.opt_3unsigned_int(nb1,nb2,nb3);
										  i+=2;
					break;
					default: printf("Arguments not found");
							 exit(EXIT_FAILURE);
				}
			}
			cursor = cursor->next;
		}
	}
}
