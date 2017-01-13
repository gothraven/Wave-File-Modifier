#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "wave.h"


unsigned char buffer2[2];
unsigned char buffer4[4];
/*
 * To creat a new wave from scratch
 * result is satisfying
 * copied and used
 */
Wave_t* wave_new(uint32_t f,uint16_t p,uint16_t c,uint32_t B){

	uint16_t b = c * (p/8);
	uint32_t r = f * b;
	uint64_t D = B * b;

	Wave_t * newWave = malloc(sizeof(Wave_t));
	newWave->header = malloc(sizeof(WAVE));
	newWave->data = malloc(D);

	/* Les constantes symboliques */
	strncpy(newWave->header->riff, "RIFF", 4);
	strncpy(newWave->header->wave, "WAVE", 4);
	strncpy(newWave->header->fmt, "fmt ", 4);
	strncpy(newWave->header->Ndata, "data", 4);

	newWave->header->taille = ( D + T_ENTETE ) - 8;
	newWave->header->subTaille1 = 16;
	newWave->header->formatAudio = 1; 
	newWave->header->nombreCanaux = c;
	newWave->header->freqEch = f;
	newWave->header->align = b;
	newWave->header->ByteRate = r;
	newWave->header->bitsParEch = p;
	newWave->header->subTaille2 = D;

	return newWave;

}


/*
 * To load a wave file from the hard drive needed for options
 * result is satisfying
 * copied and used
 */
Wave_t* wave_load_opt(Wave_t * wave, const char* fname){
	FILE * ptr; 
	ptr = fopen(fname, "rb");
	wave = malloc(sizeof(Wave_t));
	if (ptr == NULL) {
		fprintf(stderr,"Thie file \" %s \" doesn't exist.\n", fname);
                return NULL;
	}else{
		headerWave_load_fast(wave,ptr);
		load_data(wave,ptr);
		return wave;
	}
}

/*
 * To load a wave file from the hard drive
 * result is satisfying
 * copied and used
 */
Wave_t* wave_load(Wave_t * wave, const char* fname){
	FILE * ptr; 
	ptr = fopen(fname, "rb");
	wave = malloc(sizeof(Wave_t));
	if (ptr == NULL) {
		fprintf(stderr,"cannot open file %s\n", fname);
		printf("your file doesn't exist, would you please try again ..\n");
		char name[100];
		printf("file name? :");   
		scanf("%s",name);
		strcat(name,".wav");
		return wave = wave_load(wave,name);
		free(name);

	}else{
		headerWave_load_fast(wave,ptr);
		load_data(wave,ptr);
		return wave;
	}
}

/*
 * fonctions that loads the wave header one by one
 * result is satisfying
 * copied and used
 */
bool load_riff(Wave_t * wave,FILE * ptr){
	return (fread(wave->header->riff,sizeof(wave->header->riff),1,ptr)==1);
}

bool load_taille(FILE * ptr){
	return (fread(buffer4,sizeof(buffer4),1,ptr)==1);
}

bool load_wave(Wave_t * wave,FILE * ptr){
	return (fread(wave->header->wave,sizeof(wave->header->wave),1,ptr)==1);
}

bool load_fmt(Wave_t * wave,FILE * ptr){
	return (fread(wave->header->fmt,sizeof(wave->header->fmt),1,ptr)==1);
}

bool load_subTaille1(FILE * ptr){
	return (fread(buffer4,sizeof(buffer4),1,ptr)==1);
}

bool load_formatAudio(FILE * ptr){
	return (fread(buffer2,sizeof(buffer2),1,ptr)==1);
}

bool load_nombreCanaux(FILE * ptr){
	return (fread(buffer2,sizeof(buffer2),1,ptr)==1);
}

bool load_freqEch(FILE * ptr){
	return (fread(buffer4,sizeof(buffer4),1,ptr)==1);
}

bool load_ByteRate(FILE * ptr){
	return (fread(buffer4,sizeof(buffer4),1,ptr)==1);
}

bool load_align(FILE * ptr){
	return (fread(buffer2,sizeof(buffer2),1,ptr)==1);
}

bool load_bitsParEch(FILE * ptr){
	return (fread(buffer2,sizeof(buffer2),1,ptr)==1);
}

bool load_Ndata(Wave_t * wave,FILE * ptr){
	return (fread(wave->header->Ndata,sizeof(wave->header->Ndata),1,ptr)==1);
}

bool load_subTaille2(FILE * ptr){
	return (fread(buffer4,sizeof(buffer4),1,ptr)==1);
}

/*
 * To load a the data of a wave file
 * result is satisfying
 * copied and used
 */
bool load_data(Wave_t * wave, FILE * ptr){
	wave->data = malloc(wave->header->subTaille2*sizeof(char));
	if(wave->data == NULL){
		fprintf(stderr,"Not enough memory space to load the wave data...\n");
	}
	if(fread(wave->data,sizeof(unsigned char),wave->header->subTaille2,ptr)!=1) return false;
	return true;
}

/*
 * To just load a file it doesn't matter if its a wave or not
 * result is satisfying
 * copied and used
 */
bool headerWave_load_fast(Wave_t * wave, FILE * ptr){
	wave->header = malloc(sizeof(WAVE));

	if(wave->header == NULL){
		fprintf(stderr,"Not enough memory space to load the wave header...\n"); 
		return false;
	}

	if(!fread(wave->header,T_ENTETE,1,ptr)) return false;

	return true;

}

/*
 * To load a wave file and make sure that its a wave file and not something else
 * result is satisfying
 * copied and used
 */
bool headerWave_load(Wave_t * wave, FILE * ptr){
	wave->header = malloc(sizeof(WAVE));

	if(wave->header == NULL){
		fprintf(stderr,"Not able to malloc a place to load your file...\n"); 
		return false;
	}

	if(!load_riff(wave,ptr)) return false;

	if(!load_taille(ptr)) return false;
	wave->header->taille = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);

	if(!load_wave(wave,ptr)) return false;

	if(!load_fmt(wave,ptr)) return false;

	if(!load_subTaille1(ptr)) return false;
	wave->header->subTaille1 = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);

	if(!load_formatAudio(ptr)) return false;
	wave->header->formatAudio = buffer2[0] | (buffer2[1]<<8);
	if(wave->header->formatAudio != 1) return false;

	if(!load_nombreCanaux(ptr)) return false;
	wave->header->nombreCanaux = buffer2[0] | (buffer2[1]<<8);

	if(!load_freqEch(ptr)) return false;
	wave->header-> freqEch = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);

	if(!load_ByteRate(ptr)) return false;
	wave->header-> ByteRate = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);

	if(!load_align(ptr)) return false;
	wave->header->align = buffer2[0] | (buffer2[1]<<8);

	if(!load_bitsParEch(ptr)) return false;
	wave->header->bitsParEch = buffer2[0] | (buffer2[1]<<8);

	if(!load_Ndata(wave,ptr)) return false;

	if(!load_subTaille2(ptr)) return false;
	wave->header->subTaille2 = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);

	return true;
}

/*
 * To save a wave file
 * result is satisfying
 * copied and used
 */
bool wave_save(const char* fname, Wave_t * wave){
	FILE *fp = fopen(fname, "wb+");
	if(fp == NULL){
		fprintf(stderr,"cannot open file %s in mode w+\n", fname);
		return false;
	}

	fwrite(wave->header, T_ENTETE, 1, fp);
	fwrite(wave->data,sizeof(char), wave->header->subTaille2, fp);
	fclose(fp);
	return true;
}

/*
 * To print the informations of a wave file
 * result is satisfying
 * copied and used
 */
void wave_info(Wave_t * wave){

	printf("information : \n\n");
	printf("RIFF : %s\n",wave->header->riff);
	printf("taille : %u\n",wave->header->taille);
	printf("WAVE : %s\n",wave->header->wave);
	printf("fmt_ : %s\n",wave->header->fmt);
	printf("subTaille1 : %u\n",wave->header->subTaille1);
	printf("formatAudio : %u\n",wave->header->formatAudio);
	printf("nombreCanaux : %u\n",wave->header->nombreCanaux);
	printf("freqEch : %u\n",wave->header->freqEch); 
	printf("ByteRate : %u\n",wave->header->ByteRate);
	printf("align : %u\n",wave->header->align);
	printf("bitsParEch : %u\n",wave->header->bitsParEch);
	printf("Ndata : %s\n",wave->header->Ndata);
	printf("subTaille2 : %u\n",wave->header->subTaille2);
	uint32_t nb_Blocs = (wave->header->subTaille2) / (wave->header->nombreCanaux * wave->header->bitsParEch/8); //number of blocs
	printf("nb_Blocs = %u\n",nb_Blocs);
	float seconds = (float)(wave->header->subTaille2) / (wave->header->ByteRate);
	printf("the time in seconds => %f \n",seconds);
	printf("file time h:m:s:ms => %s\n",seconds_to_time(seconds));

}

/*
 * To convert seconds into a time
 * result is satisfying
 * copied and used
 */
char* seconds_to_time(float raw_seconds) {
	char *hms;
	int hours, hours_residue, minutes, seconds, milliseconds;
	hms = (char*) malloc(100);

	sprintf(hms, "%f", raw_seconds);

	hours = (int) raw_seconds/3600;
	hours_residue = (int) raw_seconds % 3600;
	minutes = hours_residue/60;
	seconds = hours_residue % 60;
	milliseconds = 0;

	char *pos;
	pos = strchr(hms, '.');
	int ipos = (int) (pos - hms);
	char decimalpart[15];
	memset(decimalpart, ' ', sizeof(decimalpart));
	strncpy(decimalpart, &hms[ipos+1], 3);
	milliseconds = atoi(decimalpart);     


	sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
	return hms;
}

/*
 * To reverse the way we play a wave file
 * result is satisfying
 * copied and used
 */
void wave_reverse(Wave_t * wave){
	uint32_t nb_oct = wave->header->subTaille2;
	unsigned char reverse[nb_oct];

	int j=0;
	for(uint32_t i =nb_oct; i>0; i--){
		reverse[j]=wave->data[i];
		j++;
	}
	for(uint32_t i =0; i<nb_oct; i++){
		wave->data[i]=reverse[i];
	}
}

/*
 * To get a time 
 * result is satisfying
 * copied and used
 */
void get_time(Wave_t * wave, Time_t * time){

	ask_time(time);
	while(!verify_time(wave,time)){
		printf("the time chosen is invalid\n");
		ask_time(time);
	}
	printf("the time chosen is %d:%d:%d\n",time->heure,time->min,time->sec);

}

/*
 * To ask for the time hours after minutes then seconds 
 * result is satisfying
 * copied and used
 */
void ask_time(Time_t * time){

	printf("heure :");
	scanf("%d",&(time->heure));
	printf("minute :");
	scanf("%d",&(time->min));
	printf("second :");
	scanf("%d",&(time->sec));

}

/*
 * To verify a time given if its valid or not
 * result is satisfying
 * copied and used
 */
bool verify_time(Wave_t * wave,Time_t * time){
	return (time2oct(wave,time) > 0 ) && (time2oct(wave,time) < wave->header->subTaille2);
}

/*
 * To convert a time given into octs 
 * result is satisfying
 * copied and used
 */
uint32_t time2oct(Wave_t * wave,Time_t * time){

	uint32_t result;
	float sec = (time->heure)*3600 + (time->min)*60 + time->sec;
	return result = (uint32_t)(sec * wave->header->ByteRate);

}

/*
 * To crop a wave file
 * result is satisfying
 * copied and used
 */
void wave_crop(Wave_t * wave, uint32_t start, uint32_t end){

	uint32_t swip;
	if(start>end){
		swip = end;
		end = start;
		start=swip;
	}

	uint32_t nb_oct_rest = end - start;
	unsigned char crop[nb_oct_rest];

	uint32_t j=start;
	for(uint32_t i = 0; i<nb_oct_rest; i++){
		crop[i] = wave->data[j];
		j++;
	}
	wave->data = crop;
	wave->header->subTaille2 = nb_oct_rest;
}

/*
 * To prepare for the wave_crop by asking for the time when u will crop
 * result is satisfying
 * copied and used
 */
void wave_crop_sec(Wave_t * wave){
	Time_t * time_start = malloc(sizeof(Time_t));
	Time_t * time_end = malloc(sizeof(Time_t));
	printf("give us the starting time\n");
	printf("the time in format h:m:s\n\n");
	get_time(wave,time_start);
	printf("give us the ending time\n");
	printf("the time in format h:m:s\n");
	get_time(wave,time_end);

	wave_crop(wave,time2oct(wave,time_start),time2oct(wave,time_end));
}

/*
 * To get a sample which exists in the bloc i and the colon j
 * result is not satisfying
 * not copied and not being used
 */
int64_t wave_get(Wave_t * wave, uint32_t i, uint16_t j){
	uint16_t octParBloc = wave->header->align;
	uint32_t nb_Blocs = (wave->header->subTaille2)/(wave->header->nombreCanaux * wave->header->bitsParEch/8);
	uint16_t octParCan = (wave->header->bitsParEch/8);
	int64_t ampli;

	if((i)>nb_Blocs-1){
		printf("erreur, Bloc doens't exist\n");
		ampli = 0;
	}else if(j>wave->header->nombreCanaux){
		printf("erreur, Canal doesn't exist\n");
		ampli = 0;
	}else{

		uint64_t place = (i)*(octParBloc)+(j)*(octParCan);
             

		if(octParCan==1){
			ampli =wave->data[place];
		}else if(octParCan==2){
			ampli = wave->data[place] | (wave->data[place+1]<<8);
		}else if(octParCan==3){
			ampli = wave->data[place] | (wave->data[place+1]<<8) | (wave->data[place+2]<<16);
		}else{
			printf("problem happned while getting the ampli\n");
		}
	}       
	return ampli;

}

/*
 * To check if the computer is a little or big Endian
 * result is satisfying
 * copied and used
 */
bool isBigE(){
	int a = 1;
	char * p = (char*)&a;
	return (p[0]==0);
}

/*
 * To add a mathimathic fonction graphe into a wave file data
 * result is satisfying
 * not copied and not being used
 */
void wave_set(Wave_t * wave, uint32_t i, uint16_t j, int64_t ampli){
	uint16_t octParBloc = wave->header->align;
	uint32_t nb_Blocs = (wave->header->subTaille2) / (wave->header->nombreCanaux * wave->header->bitsParEch/8);
	uint16_t octParCan = (wave->header->bitsParEch/8);
	if((i)>nb_Blocs-1){
		printf("erreur, Bloc %d doens't exist\n",i);
		ampli = 0;
	}else if(j>wave->header->nombreCanaux){
		printf("erreur, Canal doesn't exist\n");
		ampli = 0;
	}else{

		uint64_t place = (i)*(octParBloc) + (j)*(octParCan);


		if(octParCan==1){
			int8_t a;
			if(ampli>127){
				a=127;
			}else if(ampli<-128){
				a=-128;
			}else{
				a=ampli;
			}
			wave->data[place] = a;
		}else if(octParCan==2){
			int16_t a;
			if(ampli>32767){
				a=32767;
			}else if(ampli<-32768){
				a=-32768;
			}else{
				a=ampli;
			}
			if(isBigE()){
				char * p = (char*)&a;
				wave->data[place] = p[0] | p[1]<<8;
			}else{ 
				char * p = (char*)&a;
				wave->data[place] = p[0];
				wave->data[place+1] = p[1];
			}
		}else if(octParCan==3){
			int32_t a;
			if(ampli>8388607){
				a=8388607;
			}else if(ampli<-8388608){
				a=-8388608;
			}else{
				a=ampli;
			}
			if(isBigE()){
				char * p = (char*)&a;
				wave->data[place] = p[0] | p[1]<<8 | p[2]<<16;
			}else{ 
				char * p = (char*)&a;
				wave->data[place] = p[0];
				wave->data[place+1] = p[1];
				wave->data[place+2] = p[2];
			}
		}else{
			printf("problem happned while getting the ampli\n");
		}
	}

}

/*
 * To add a mathimathic fonction graphe into a wave file data
 * result is satisfying
 * copied and being used
 */
void add_signal(Wave_t * wave, double (*s)(double)){
	double k=0;
	for(uint16_t i=0; i<(wave->header->subTaille2/wave->header->align); i+=wave->header->nombreCanaux){
		k++;
		for(uint16_t j=0; j<wave->header->nombreCanaux; j++){
			switch(wave->header->bitsParEch){
				case(8):wave_set(wave,i,j,(int64_t)((127.0)*(s(k))));break;
				case(16):wave_set(wave,i,j,(int64_t)((32767.0)*(s(k))));break;
				case(24):wave_set(wave,i,j,(int64_t)((8388607.0)*(s(k))));break;
				default : printf("problem\n"); break;
			}
		} 
	}   
}

/*
 * To make the file goes faster or slower depending on the factor passed
 * result is satisfying
 * copied and being used
 */
void wave_scale(Wave_t * wave, double s){
	wave->header->freqEch *=s ; 
	wave->header->ByteRate = wave->header->freqEch * wave->header->align;
}


/*
 * To delete a wave file from the momery
 * result is satisfying
 * copied and being used
 */
void wave_delete(Wave_t * wave){
	free(wave->header);
	free(wave->data);
	free(wave);
}

/*
 * To splite a wave file into a table of wave each one of them contains a chanal of the original wave
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not being used
 */
Wave_t ** wave_split(Wave_t * wave, int* pc){
	Wave_t ** waveTab = malloc(sizeof(Wave_t)*(wave->header->nombreCanaux+1));
	uint16_t i;
	uint32_t j;
	for(i=0; i<wave->header->nombreCanaux; i++){
		waveTab[i] = wave_new(wave->header->freqEch,wave->header->bitsParEch,1,wave->header->subTaille2/wave->header->align);
		for(j=0; j<(wave->header->subTaille2/wave->header->align); j++){
			wave_set(waveTab[i],j,0,wave_get(wave,j,i));   
		}
	}
	//waveTab[i] = NULL;
	*pc = wave->header->nombreCanaux;
	//wave_delete(wave);
	return waveTab;
}

/*
 * To merge two wave files into only one
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not being used
 */
Wave_t * wave_merge_deux(Wave_t* wave1,Wave_t* wave2){
	Wave_t * wave = wave_new(wave1->header->freqEch,wave1->header->bitsParEch,1,wave1->header->subTaille2/wave1->header->align);
	for(uint32_t i=0; i<wave1->header->subTaille2/wave1->header->align; i++){
		wave_set(wave,i,0,(wave_get(wave1,i,0)+wave_get(wave2,i,0))/2);
	}
	return wave;
}

/*
 * To merge all the wave files inside the table inside one wave that has c waves which is how many wave inside the table too
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not being used
 */
Wave_t * wave_merge_all(Wave_t ** waveTab,int c){
	Wave_t * wave = wave_new(waveTab[0]->header->freqEch,waveTab[0]->header->bitsParEch,c,waveTab[0]->header->subTaille2/waveTab[0]->header->align);
	for(uint32_t i=0; i<waveTab[0]->header->subTaille2/waveTab[0]->header->align; i++){
		for(int j=0; j<c; j++){
			wave_set(wave,i,j,wave_get(waveTab[j],i,0));
		}
	}
	for(int i=0; i<c; i++){
		wave_delete(waveTab[i]);
	}
	return wave;
}


/*
 * to find how much waves in a table
 * result is satisfying
 * not copied and not being used
 */
int tailleTab(Wave_t**waveTab){
	int i=0;
	while(waveTab[i] != NULL) i++;
	return i;
}

/*
 * to merge some canals when we want have less canals then we had
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not being used
 */
Wave_t * wave_merge(Wave_t ** waveTab,int c){
	Wave_t * wave = wave_new(waveTab[0]->header->freqEch,waveTab[0]->header->bitsParEch,c,waveTab[0]->header->subTaille2/waveTab[0]->header->align);
	int k = tailleTab(waveTab);
	for(uint32_t i=0; i<(waveTab[0]->header->subTaille2/waveTab[0]->header->align); i++){
		for(int j=0; j<k; j++){
			if( j < c-1){	                                                
				wave_set(wave,i,j,wave_get(waveTab[j],i,0));
			}else{
                                Wave_t * rest;
                                if(j=k-1){
                                  rest = waveTab[j];
                                }else{
                                  rest = wave_merge_deux(waveTab[j],waveTab[j+1]);
                                }
                                for(uint32_t m=0; m<(rest->header->subTaille2/rest->header->align); m++){
					wave_set(wave,m,c-1,wave_get(rest,m,0));
				}

			}
		}
	}
	return wave;
}


/* 
 * to add more canals  
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not being used
 */
Wave_t * wave_doublicat(Wave_t * wave,int c){
	Wave_t * w = wave_new(wave->header->freqEch,wave->header->bitsParEch,c,wave->header->subTaille2/wave->header->align);
	int nb_Can = wave->header->nombreCanaux;
	for(uint32_t i=0; i<(wave->header->subTaille2/wave->header->align); i++){
		for(int j=0; j<c; j++){
			if( j < nb_Can){	                                                
				wave_set(w,i,j,wave_get(wave,i,j));
			}else{
				wave_set(w,i,j,wave_get(wave,i,nb_Can));
			}
		}
	}
	return w;
}

/*
 * to change the wave's number of channels
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not being used
 */
void wave_canal(Wave_t** wave, uint16_t c){
	int p = 0;
	uint16_t nb_Can = (*wave)->header->nombreCanaux;
	if(nb_Can > c){
		Wave_t ** waveTab = wave_split(*wave,&p);
		(*wave) = wave_merge(waveTab,c);
	}else if(nb_Can < c){
		(*wave) = wave_doublicat(*wave,c);
	}
}

/*
 * to print the amplitude we have in a certain channel into a txt file
 * result is satisfying
 * not copied and not being used
 */
void get_canal(Wave_t * wave,int c,const char * fname){
	FILE * fp = fopen(fname,"wb+");
	for(uint32_t i=0; i<wave->header->subTaille2/wave->header->align; i++){
		fprintf(fp,"%ld\n",wave_get(wave,i,c));
	}
	fclose(fp);
}


/*
 * to change the wave's number of channels
 * (wave_get wave_set) are not used
 * result is satisfying
 * copied and being used
 */
Wave_t* change_canal(Wave_t * wave,uint16_t c){
	Wave_t * newWave = wave_new(wave->header->freqEch,wave->header->bitsParEch,c,(wave->header->subTaille2/wave->header->align));
	if(wave->header->nombreCanaux>c){
		uint32_t k =0;
		for(uint32_t i=0; i<wave->header->subTaille2; i+=wave->header->align){
			for(uint16_t j=0; j<c*(wave->header->bitsParEch/8); j++){
				newWave->data[k] = wave->data[i+j];
				k++; 
			}
		}
	}else{
		uint32_t k =0;
		for(uint32_t i=0; i<wave->header->subTaille2; i+=wave->header->align){
			for(uint16_t j=0; j<c*(wave->header->bitsParEch/8); j++){
				if(j < (wave->header->nombreCanaux)){
					newWave->data[k] = wave->data[i+j];
				}else{
					newWave->data[k] = wave->data[i+(wave->header->nombreCanaux)-1];
				}
				k++;    
			}

		}

	}
	return newWave;	
}

/*
 * to change the wave's precision of a file
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not bieng used
 */
void change_precision(Wave_t* wave,uint16_t p){
	Wave_t * newWave = wave_new(wave->header->freqEch,p,wave->header->nombreCanaux,(wave->header->subTaille2/wave->header->align));

	uint32_t k =0;
	uint32_t size = ((wave->header->subTaille2/wave->header->align)*wave->header->nombreCanaux);
	int32_t * d32bit = malloc(size);
	int16_t * d16bit = malloc(size);
	int8_t * d8bit = malloc(size);
	for(uint32_t i=0; i<wave->header->subTaille2; i+=wave->header->align){
		for(uint16_t j=0; j<wave->header->nombreCanaux*(wave->header->bitsParEch/8); j+=(wave->header->align/wave->header->nombreCanaux)){
			switch(wave->header->bitsParEch){
				case(8):
					switch(p){
						case(8):
							break;
						case(16):
							d8bit[k]=(wave->data[i+j] | wave->data[i+j+1]<<8)*((pow(2,16)-1)/(pow(2,8)-1));
							k++;
							break;
						case(24):
							d32bit[k]=(wave->data[i+j] | wave->data[i+j+1]<<8 | wave->data[i+j+1]<<24)*((pow(2,24)-1)/(pow(2,8)-1));
							k++;
							break;
						default: printf("erreur while converting\n"); 
					}
					break;
				case(16):
					switch(p){
						case(8):
							d8bit[k]=(wave->data[i+j] | wave->data[i+j+1]<<8)*((pow(2,8)-1)/(pow(2,16)-1));
							k++;
							break;
						case(16):
							break;
						case(24):
							d32bit[k]=(wave->data[i+j] | wave->data[i+j+1]<<8 | wave->data[i+j+1]<<24)*((pow(2,24)-1)/(pow(2,16)-1));
							k++;
							break;
						default: printf("erreur while converting\n"); 
					}
					break;
				case(24):
					switch(p){
						case(8):
							d8bit[k]=(wave->data[i+j] | wave->data[i+j+1]<<8)*((pow(2,8)-1)/(pow(2,24)-1));
							k++;
							break;
						case(16):
							d16bit[k]=(wave->data[i+j] | wave->data[i+j+1]<<8 | wave->data[i+j+1]<<24)*((pow(2,16)-1)/(pow(2,24)-1));
							k++;
							break;
						case(24):
							break;
						default: printf("erreur while converting\n"); 
					}
					break;
				default: printf("erreur while converting\n");
			}

		}
	}
	switch(newWave->header->bitsParEch){
		case(8): newWave->data = (unsigned char*)d8bit; break;
		case(16): newWave->data = (unsigned char*)d16bit; break;
		case(24): newWave->data = (unsigned char*)d32bit; break;
		default: printf("erreur when making data inside the wave\n");
	}
}
/*
 * to change the wave's volume
 * (wave_get wave_set) are used
 * result is not satisfying
 * not copied and not bieng used
 */
void wave_volume(Wave_t* wave,float v){
     for(uint32_t i=0; i<wave->header->subTaille2/wave->header->align; i++){
		for(int j=0; j<wave->header->nombreCanaux; j++){
			wave_set(wave,i,j,(uint64_t)(v*wave_get(wave,i,j)));
		}
	}
}
/*
 * to just test the wave_get and wave_set
 * result is not satisfying
 * not copied and not being used
 */
Wave_t * wave_copy(Wave_t * wave){
	Wave_t * w = wave_new(wave->header->freqEch,wave->header->bitsParEch,wave->header->nombreCanaux,wave->header->subTaille2/wave->header->align);
	for(uint32_t i=0; i<(wave->header->subTaille2/wave->header->align); i++){
		for(uint16_t j=0; j<wave->header->nombreCanaux; j++){                                           
			wave_set(w,i,j,wave_get(wave,i,j));
		}
	}
	return w;
}

/*
 * to find and returns the biggest wave file's number of samples
 * result is satisfying
 * not copied and not being used
 */

uint32_t find_biggest_wave(Wave_t ** waveTab){

	uint32_t result = 0;
	int i=0;
	while(i<tailleTab(waveTab)){
		uint32_t B = (waveTab[i]->header->subTaille2)/(waveTab[i]->header->align);
		if(result < B){
			result = B;
		}
		i++;
	}
	return result;
}


/*
 * to concate n wave files
 * result is not satisfying
 * not copied or or being used
 */
Wave_t * wave_concat(Wave_t ** waveTab){
	Wave_t * wave =wave_new(waveTab[0]->header->freqEch,waveTab[0]->header->bitsParEch,tailleTab(waveTab),find_biggest_wave(waveTab));
	uint32_t k=0;
	for(uint32_t i=0; i<wave->header->subTaille2; i+=wave->header->align){
		for(uint16_t j=0; j<(wave->header->nombreCanaux)*(wave->header->bitsParEch/8); j++){
			if(k<waveTab[1]->header->subTaille2){  
				wave->data[i+j] = waveTab[j]->data[k];
			}else{
				wave->data[i+j]=0;
			}
		}      
		k++;
	}
	return wave;
}

