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

bool exist = false;

Wave_t* wave = NULL;

/*
 * To exit the programe but make sure that the used want save his file or not before exiting
 * result is satisfying
*/
void quit() {
	if(exist){
		printf("There is already a wave file being used, don't you want save it? (y)es, (n)o\n");
		char key ='u';
		while(key != 'Y' && key != 'y' && key !='N' && key !='n'){ 
			scanf("%c",&key);
		}
		if(key=='Y' || key=='y'){
			printf("How you want name your file?");
			char name[100];   
			scanf("%s",name);
			strcat(name,".wav");
			wave_save(name,wave);
			printf("Saving ...\n");
			sleep(1);
			clrscr();
		}
	}
	cont = false;
}

/*
 * Opens and loads a wave file from the hard drave but make sur to ask the user
 * to save in cas there was already file being used
 * result is satisfying
*/
void Ouvrir(){
	if(exist){
		printf("There is already a wave file being used, don't you want save it? (y)es, (n)o\n");
		char key ='u';
		while(key != 'Y' && key != 'y' && key !='N' && key !='n'){ 
			scanf("%c",&key);
		}
		if(key=='Y' || key=='y'){
			printf("How you want name your file?");
			char name[100];   
			scanf("%s",name);
			strcat(name,".wav");
			wave_save(name,wave);
			printf("Saving ...\n");
			sleep(1);
			clrscr();
		}
	}
	char chaine[30];
	printf("Entrez le nom du fichier que vous voulez ouvrir: ");
	char name[100];   
	scanf("%s",name);
	strcat(name,".wav");
	wave = wave_load(wave,name); 
	printf("Loading ...\n");
	sleep(1);
	clrscr();
	exist = true;
}

/*
 * Opens and loads a wave file from the hard drave but make sur to ask the user
 * to save in cas there was already file being used
 * result is satisfying
*/
void Sauvgarder(){
   if(!exist){
     printf("There is no file being used to be saved ...");
   }else{
   printf("How you want name your file?");
   char name[100];   
   scanf("%s",name);
   strcat(name,".wav");
   wave_save(name,wave);
   printf("Saving ...\n");
   sleep(1);
   clrscr();
   }
}

int main(){

	//animation();

	Menu_t * MENU = Prepare_Menu();
	addMenuAction(MENU,"Quitter",quit);

	while(cont) useMenu(MENU);

	return EXIT_SUCCESS;
}



