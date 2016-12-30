
#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdbool.h>

#define PIX(im,i,j) ((im)->img[(j)*(im)->width+(i)])


typedef struct {
  unsigned char R;
  unsigned char G;
  unsigned char B;
  unsigned char A;
}pixel;

typedef struct {
  unsigned int width;
  unsigned int heigth;
  pixel* img;
}image;

typedef enum { R, G, B, A } channel_t;

void ascii_print(image* im, channel_t cl, const char* ct);
void freeIm(image* im);
void viderLigne(FILE* fdesc);
void load_image_PBM(image* im, FILE* fdesc);
void load_image_PBM_bin(image* im, FILE* fdesc);
void load_image_PGM_bin(image* im, FILE* fdesc);
void load_image_PPM_bin(image* im, FILE* fdesc);
void load_image_PGM(image* im, FILE* fdesc);
void load_image_PPM(image* im, FILE* fdesc);
image* load_image(const char* fname);
bool save_image_PBM(image* im, FILE* fdesc);
bool save_image_PBM_bin(image* im, FILE* fdesc);
bool save_image_PGM(image* im, FILE* fdesc);
bool save_image_PGM_bin(image* im, FILE* fdesc);
bool save_image_PPM(image* im, FILE* fdesc);
bool save_image_PPM_bin(image* im, FILE* fdesc);
bool save_image(const char* fname, image* im, const char* magic_number);
image* copie(image* im);
bool grayscale(image* im);
bool NBseuil(image* im,unsigned int seuil);
bool NBmoyenne(image* im);
int compare(void const *a , void const *b);
bool NBmediane(image* im);
bool hflip(image* im);
bool vflip(image* im);
bool pivod(image* im);
bool pivog(image* im);
bool crop(image* im, unsigned int i, unsigned int j, unsigned int w, unsigned int h);
image* resizeWgrand(image* im, unsigned int w, unsigned int h, unsigned int nW);
image* resizeHgrand(image* im, unsigned int w, unsigned int h, unsigned int nH);
image* resizeWpetit(image* im, unsigned int w, unsigned int h, unsigned int nW);
image* resizeHpetit(image* im, unsigned int w, unsigned int h, unsigned int nH);
bool resize(image* im, unsigned int w, unsigned int h);
bool negatif(image* im);

#endif





