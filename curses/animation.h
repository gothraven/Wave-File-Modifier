#ifndef ANIMATION_H
#define ANIMATION_H

#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#define NUM_FRAMES 150
#define NUM_BLOBS 800
#define PERSPECTIVE 50.0
#define M_1_PI     0.318309886183790671538

typedef struct {
	double x,y,z;
}spaceblob;


double prng();
void animation();

#endif
