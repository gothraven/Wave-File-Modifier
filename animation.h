#ifndef ANIMATION_H
#define ANIMATION_H

#include <curses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#define NUM_FRAMES 150
#define NUM_BLOBS 800
#define PERSPECTIVE 50.0


typedef struct {
	double x,y,z;
}spaceblob;


double prng();
void animation();

#endif
