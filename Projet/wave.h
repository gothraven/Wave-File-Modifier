#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>


#define T_ENTETE 44

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
	unsigned char * data;
}Wave_t;

typedef struct{
	int sec;
	int min;
	int heure;
}Time_t;


Wave_t* wave_new(uint32_t f,uint16_t p,uint16_t c,uint32_t B);
Wave_t* wave_load(Wave_t * wave, const char* fname);
Wave_t* wave_load_opt(Wave_t * wave, const char* fname);

bool load_riff(Wave_t * wave,FILE * ptr);
bool load_taille(FILE * ptr);
bool load_wave(Wave_t * wave,FILE * ptr);
bool load_fmt(Wave_t * wave,FILE * ptr);
bool load_subTaille1(FILE * ptr);
bool load_formatAudio(FILE * ptr);
bool load_nombreCanaux(FILE * ptr);
bool load_freqEch(FILE * ptr);
bool load_ByteRate(FILE * ptr);
bool load_align(FILE * ptr);
bool load_bitsParEch(FILE * ptr);
bool load_Ndata(Wave_t * wave,FILE * ptr);
bool load_subTaille2(FILE * ptr);

bool load_data(Wave_t * wave, FILE * ptr);
bool headerWave_load_fast(Wave_t * wave, FILE * ptr);
bool headerWave_load(Wave_t * wave, FILE * ptr);
bool wave_save(const char* fname, Wave_t * wave);
void wave_info(Wave_t * wave);
char* seconds_to_time(float raw_seconds);
void wave_reverse(Wave_t * wave);

void get_time(Wave_t * wave, Time_t * time); ///may be delete
void ask_time(Time_t * time);
bool verify_time(Wave_t * wave,Time_t * time);
uint32_t time2oct(Wave_t * wave,Time_t * time);
void wave_crop(Wave_t * wave, uint32_t start, uint32_t end);
void wave_crop_sec(Wave_t * wave);
int64_t wave_get(Wave_t * wave, uint32_t i, uint16_t j);
bool isBigE();
void wave_set(Wave_t * wave, uint32_t i, uint16_t j, int64_t ampli);
void add_signal(Wave_t * wave, double (*s)(double));
void wave_scale(Wave_t * wave, double s);
void wave_delete(Wave_t * wave);
Wave_t ** wave_split(Wave_t * wave, int* pc);
Wave_t * wave_merge_deux(Wave_t* wave1,Wave_t* wave2);
Wave_t * wave_merge_all(Wave_t ** waveTab,int c);
int tailleTab(Wave_t**waveTab);
Wave_t * wave_merge(Wave_t ** waveTab,int c);
Wave_t * wave_doublicat(Wave_t * wave,int c);
void wave_canal(Wave_t** wave, uint16_t c);
void get_canal(Wave_t * wave,int c,const char * fname);
Wave_t* change_canal(Wave_t * wave,uint16_t c);
void change_precision(Wave_t* wave,uint16_t p);
void wave_volume(Wave_t* wave,float v);
Wave_t * wave_copy(Wave_t * wave);
uint32_t find_biggest_wave(Wave_t ** waveTab);
Wave_t * wave_concat(Wave_t ** waveTab);


#endif
