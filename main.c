#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>
#include "menu.h"
#include "wave.h"
#include "args.h"
#include "animation.h"
//int argc, char** argv
bool cont = true;

void quit() {
  cont = false;
}

int main(){

 animation();

 Menu_t * MENU = Prepare_Menu();
 addMenuAction(MENU,"Quitter",quit);

while(cont) useMenu(MENU);

return EXIT_SUCCESS;
}



