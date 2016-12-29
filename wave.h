#ifndef WAVE_H
#define WAVE_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
typedef struct Wave { 
char riff[4]; 
int32_t taille; 
char wave[4]; 
char fmt[4]; 
int32_t subTaille1; 
int16_t formatAudio; 
int16_t nombreCanaux; 
int32_t freqEch; 
int32_t ByteRate; 
int16_t align; 
int16_t bitsParEch; 
char Ndata[4]; 
int32_t subTaille2; 
void *data; 
}Wave_t;

#endif
