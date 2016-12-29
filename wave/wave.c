#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>


#define T_ENTETE 44

unsigned char buffer2[2];
unsigned char buffer4[4];

typedef struct{
	/* Le mot "RIFF" */
	char riff[4];

	/* La taille du fichier S = D + 44 - 8
	 * Ou subTaille2 + 44 - 8 */
	uint32_t taille;

	/* Le mot "WAVE" */
	char wave[4];

	/* Le mot "fmt " */
	char fmt[4];

	/* 16 ce qui est la taille de riffe + taille + wave + fmt */
	uint32_t subTaille1;

	/* Format du fichier */
	uint16_t formatAudio;

	/* Nombres de canaux = c*/
	uint16_t nombreCanaux;

	/* Fréquence d'échantillonnage */
	uint32_t freqEch;

	/* ByteRate = r
	 * Nombre d'ocets par seconde
	 ** Calcul :
	 *===> freqEch * nombreCanaux * bitsParEch / 8 */
	uint32_t ByteRate;

	/* Alignement = b
	 * Nombre d'octets par échantillon
	 * Englobe tous les canaux !
	 ** Calcul :
	 *===> nombreCanaux * bitsParEch / 8 */
	uint16_t align;

	/* Bits par échantillon = p */
	uint16_t bitsParEch;

	/* Le mot "data" et la
	 * taille des données */
	char Ndata[4];

	/* Taille des données = D*/
	uint32_t subTaille2;

	//  void * data;
	/* A allouer dynamiquement.
	 * Contiendra les données */
}WAVE;

typedef struct{

	WAVE * header;
	char * data;

}Wave_t;

typedef struct{

	int sec;
	int min;
	int heure;
}Time_t;

void clrscr(){
	system("@cls||clear");
}

char* seconds_to_time(float raw_seconds);
bool headerWave_load(Wave_t * wave, FILE * ptr);
bool load_data(Wave_t* wave, FILE * ptr);
uint32_t time2oct(Wave_t * wave,Time_t * time);
void ask_time(Time_t * time);
bool verify_time(Wave_t * wave,Time_t * time);
int64_t wave_get(Wave_t * wave, uint32_t i, uint16_t j);

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

Wave_t* wave_load(Wave_t * wave, const char* fname){
	FILE * ptr; 
	ptr = fopen(fname, "rb");
	if (ptr == NULL) {
		fprintf(stderr,"cannot open file %s\n", fname);
		printf("your file doesn't exist, would you please try again ..\n");
		char * name = malloc(50*sizeof(char));
		printf("file name? :");
		scanf("%s",name);
		return wave = wave_load(wave,name);
		free(name);

	}else{

		printf("Opening file..\n\n");
		headerWave_load(wave,ptr);
		load_data(wave,ptr);
		return wave;

	}
}


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

bool load_data(Wave_t * wave, FILE * ptr){

	wave->data = malloc(wave->header->subTaille2*sizeof(char));
	if(wave->data == NULL){
		printf("erreur /// malloc");
	}
	if(fread(wave->data,sizeof(unsigned char),wave->header->subTaille2,ptr)!=1) return false;
	return true;

}

bool headerWave_load(Wave_t * wave, FILE * ptr){
	wave->header = malloc(sizeof(WAVE));
	if(!fread(wave->header,T_ENTETE,1,ptr)) return false;
	/*  if(!load_riff(wave,ptr)) return false; //RIFF
	//printf("riff = %s\n",wave->header->riff);

	if(!load_taille(ptr)) return false; //TAILLE 
	wave->header->taille = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
	//printf("taille = %u\n",wave->header->taille);

	if(!load_wave(wave,ptr)) return false;
	//printf("wave = %s\n",wave->header->wave);

	if(!load_fmt(wave,ptr)) return false;
	//printf("fmt = %s\n",wave->header->fmt);

	if(!load_subTaille1(ptr)) return false;
	wave->header->subTaille1 = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
	//printf("subtaille1 = %u\n",wave->header->subTaille1);

	if(!load_formatAudio(ptr)) return false;
	wave->header->formatAudio = buffer2[0] | (buffer2[1]<<8);
	//printf("formatAudio = %u\n",wave->header->formatAudio);

	if(!load_nombreCanaux(ptr)) return false;
	wave->header->nombreCanaux = buffer2[0] | (buffer2[1]<<8);
	//printf("nombreCanaux = %u\n",wave->header->nombreCanaux);

	if(!load_freqEch(ptr)) return false;
	wave->header-> freqEch = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
	//printf("freqEch = %u\n",wave->header->freqEch);

	if(!load_ByteRate(ptr)) return false;
	wave->header-> ByteRate = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
	//printf("ByteRate = %u\n",wave->header->ByteRate);

	if(!load_align(ptr)) return false;
	wave->header->align = buffer2[0] | (buffer2[1]<<8);
	//printf("align = %u\n",wave->header->align);

	if(!load_bitsParEch(ptr)) return false;
	wave->header->bitsParEch = buffer2[0] | (buffer2[1]<<8);
	//printf("bitsParEch = %u\n",wave->header->bitsParEch);

	if(!load_Ndata(wave,ptr)) return false;
	//printf("Ndata = %s\n",wave->header->Ndata);

	if(!load_subTaille2(ptr)) return false;
	wave->header->subTaille2 = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
	//printf("subTaille2 = %u\n",wave->header->subTaille2);
	 */
	return true;
}


bool wave_save(const char* fname, Wave_t * wave){

	FILE *fp = fopen(fname, "wb+");
	if(fp == NULL){
		fprintf(stderr,"cannot open file %s in mode w+\n", fname);
		return false;
	}

	fwrite(wave->header, T_ENTETE, 1, fp);
	fwrite(wave->data,sizeof(char), wave->header->subTaille2, fp);
	printf("ampli saved %ld\n",wave_get(wave,0,0));
	fclose(fp);
	return true;
}


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

	// get the decimal part of raw_seconds to get milliseconds
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

void wave_reverse(Wave_t * wave){
	uint32_t nb_oct = wave->header->subTaille2;
	unsigned char reverse[nb_oct];
	
	int j=0;
	for(int i =nb_oct; i>0; i--){
		reverse[j]=wave->data[i];
		j++;
	}
        for(int i =0; i<nb_oct; i++){
		wave->data[i]=reverse[i];
	}
}

void get_time(Wave_t * wave, Time_t * time){

	ask_time(time);
	while(!verify_time(wave,time)){
		printf("the time chosen is invalid\n");
		ask_time(time);
	}
	printf("the time chosen is %d:%d:%d\n",time->heure,time->min,time->sec);

}

void ask_time(Time_t * time){

	printf("heure :");
	scanf("%d",&(time->heure));
	printf("minute :");
	scanf("%d",&(time->min));
	printf("second :");
	scanf("%d",&(time->sec));

}

bool verify_time(Wave_t * wave,Time_t * time){
	return (time2oct(wave,time) > 0 ) && (time2oct(wave,time) < wave->header->subTaille2);
}

uint32_t time2oct(Wave_t * wave,Time_t * time){

	uint32_t result;
	float sec = (time->heure)*3600 + (time->min)*60 + time->sec;
	return result = (uint32_t)(sec * wave->header->ByteRate);

}

void wave_crop(Wave_t * wave, uint32_t start, uint32_t end){

	uint32_t swip;
	if(start>end){
		swip = end;
		end = start;
		start=swip;
	}

	uint32_t nb_oct = wave->header->subTaille2;
	unsigned char donnerr[nb_oct];
	unsigned char * donner = donnerr;
	uint32_t nb_oct_rest = end - start;
	unsigned char crop[nb_oct_rest];
	donner = wave->data;

	int j=start;
	for(int i = 0; i<nb_oct_rest; i++){
		crop[i] = donner[j];
		j++;
	}
	wave->data = crop;
	wave->header->subTaille2 = nb_oct_rest;
}

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

int64_t wave_get(Wave_t * wave, uint32_t i, uint16_t j){
	uint32_t nb_oct = wave->header->subTaille2;
	uint16_t octParBloc = wave->header->align;
	uint32_t nb_Blocs = (wave->header->subTaille2) / (wave->header->nombreCanaux * wave->header->bitsParEch/8);
	uint16_t octParCan = (wave->header->bitsParEch/8);
	int64_t ampli;

	if((i)<0 || (i)>nb_Blocs-1){
		printf("erreur, Bloc doens't exist\n");
		ampli = 0;
	}else if(j<0 || j>wave->header->nombreCanaux){
		printf("erreur, Canal doesn't exist\n");
		ampli = 0;
	}else{

		uint64_t place = (i)*(octParBloc) + (j)*(octParCan);


		if(octParCan==1){
			ampli = wave->data[place];
		}else if(octParCan==2){
			ampli = wave->data[place] | (wave->data[place+1]<<8);
		}else if(octParCan==3){
			ampli = wave->data[place] | (wave->data[place+1]<<8) | (wave->data[place+2]<<16);
		}else if(octParCan==4){				
			ampli = ampli = wave->data[place] | (wave->data[place+1]<<8) | (wave->data[place+2]<<16) | (wave->data[place+2]<<24);
		}else{
			printf("problem happned while getting the ampli\n");
		}
	}       
	return ampli;

}

bool isBigE(){
	int a = 1;
	char * p = (char*)&a;
	return (p[0]==0);
}

void wave_set(Wave_t * wave, uint32_t i, uint16_t j, int64_t ampli){
	uint32_t nb_oct = wave->header->subTaille2;
	uint16_t octParBloc = wave->header->align;
	uint32_t nb_Blocs = (wave->header->subTaille2) / (wave->header->nombreCanaux * wave->header->bitsParEch/8);
	uint16_t octParCan = (wave->header->bitsParEch/8);

	if((i)<0 || (i)>nb_Blocs-1){
		printf("erreur, Bloc %d doens't exist\n",i);
		ampli = 0;
	}else if(j<0 || j>wave->header->nombreCanaux){
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
		}else if(octParCan==4){				
			int32_t a;
			if(ampli>2147483647){
				a=2147483647;
			}else if(ampli<-2147483648){
				a=-2147483648;
			}else{
				a=ampli;
			}
			if(isBigE()){
				char * p = (char*)&a;
				wave->data[place] = p[0] | p[1]<<8 | p[2]<<16 | p[3]<<24;
			}else{ 
				char * p = (char*)&a;
				wave->data[place] = p[0];
				wave->data[place+1] = p[1];
				wave->data[place+2] = p[2];
				wave->data[place+3] = p[3];
			}
		}else{
			printf("problem happned while getting the ampli\n");
		}
	}

}


void add_signal(Wave_t * wave, double (*s)(double)){
	double k;
	for(int i=0; i<(wave->header->subTaille2/wave->header->align); i+=wave->header->nombreCanaux){
		printf("i = %d\n",i);
		k = (i*440*M_PI);
		for(int j=0; j<wave->header->nombreCanaux; j++){
			switch(wave->header->bitsParEch){
				case(8):wave_set(wave,i,j,(int64_t)((127.0)*(s(k))));break;
				case(16):wave_set(wave,i,j,(int64_t)((32767.0)*(s(k))));break;
				case(24):wave_set(wave,i,j,(int64_t)((8388607.0)*(s(k))));break;
				case(32):wave_set(wave,i,j,(int64_t)((2147483647.0)*(s(k))));break;
				default : printf("problem\n"); break;
			}
		} 
	}   
}

void wave_scale(Wave_t * wave, double s){
  wave->header->freqEch *=s ; 
  wave->header->ByteRate = wave->header->freqEch * wave->header->align;
}

Wave_t ** wave_split(Wave_t * wave, int* pc){
  Wave_t ** waveTab = malloc(sizeof(Wave_t)*(wave->header->nombreCanaux));
  for(int i=0; i<wave->header->nombreCanaux; i++){
     waveTab[i] = new_wave(wave->header->freqEch,wave1->header->bitsParEch,1,wave->header->subTaille2/wave->header->align);
     for(int j=0; j<(wave->header->subTaille2/wave->header->align); j++){
     wave_set(waveTab[i],j,0,wave_get(wave,j,i));
     }
  }
  *pc = wave->header->nombreCanaux;
  wave_delete(wave);
  return waveTab;
}

Wave_t * wave_merge_deux(Wave_t* wave1,Wave_t* wave2){
    Wave_t * wave = new_wave(wave1->header->freqEch,wave1->header->bitsParEch,1,wave1->header->subTaille2/wave1->header->align);
    for(int i=0; i<wave1->header->subTaille2/wave1->header->align; i++){
     wave_set(wave,i,0,(wave_get(wave1,i,0)+wave_get(wave2,i,0))/2);
    }
}

Wave_t * wave_merge(Wave_t ** waveTab,int c){
  Wave_t * wave = new_wave(waveTab[0]->header->freqEch,waveTab[0]->header->bitsParEch,c,waveTab[0]->header->subTaille2/waveTab[0]->header->align);
  for(int i=0; i<waveTab[0]->header->subTaille2/waveTab[0]->header->align; i++){
  	for(int j=0; j<c; j++){
  		wave_set(wave,i,j,wave_get(waveTab[j],i,0));
  	}
    }
}

void wave_canal(Wave_t * wave, uint16_t c){

  Wave_t waveTAb[c] = wave_split(wave,&c);
  if(wave->header->nombreCanaux > c){
   wave = wave_merge(waveTab,c);
  }else if(wave->header->nombreCanaux < c){
   
  }
}

int main(void){

	Wave_t * wave = malloc(sizeof(Wave_t));
	wave = wave_load(wave,"file.wav");
	printf("file loaded succesfully\n");

	int skipe;
	printf("\nentre (1) to skip?");
	scanf("%d",&skipe);
	clrscr();

	//wave_info(wave);
	//Wave_t * wave1 = wave_new(44011,16,2,800000);
	wave_info(wave);
	printf("\nentre (1) to skip?");
	scanf("%d",&skipe);
	clrscr();

	//add_signal(wave1,cos);
        wave_scale(wave,0.5);
        wave_info(wave);
	//wave_reverse(wave);
	//wave_crop_sec(wave);
	//wave_canal(wave,2);
	//wave_data_print(wave);

	printf("\nentre (1) to skip and save?");
	scanf("%d",&skipe);
	clrscr();
	//printf("ampli is %lu\n",wave_get(wave,1522,1));


	if(wave_save("result5.wav",wave)){
		printf("succesfully saved\n");
	}else{
		printf("problem happened while saving\n");
	}


	return 0;
}



