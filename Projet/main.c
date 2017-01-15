#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include "menu.h"
#include "wave.h"
#include "args.h"

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
 * Create a new wave file from scratch after asking the user to give the frequance, the precision and the numbre of channals
 * to save in cas there was already file being used
 * result is satisfying
 */
void Nouveau(){
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
	printf("frequance: "); uint32_t f; scanf("%u",&f);
	printf("precision: "); uint16_t p; scanf("%u",&p);
	printf("nombre de canaux: "); uint16_t c; scanf("%u",&c);
	Time_t * time = malloc(sizeof(Time_t));
	ask_time(time);
	uint16_t b = c*(p/8);
	uint32_t r = f * b;
	float sec = (time->heure)*3600 + (time->min)*60 + time->sec;
	uint32_t B = (uint32_t)((sec * r)/( c * p/8));
	wave = wave_new(f,p,c,B);
	exist = true;
	printf("Creating the new wave ...\n");
	sleep(1);
	clrscr();
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

/*
 * shows the informations concerning the wave file being used
 * result is satisfying
 */
void Information(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		wave_info(wave);
		printf("\n0 - return\n");
		printf("\nchoice?\n");
		int key; scanf("%d",&key);
		clrscr();
	}
}

/*
 * reverse the file and make sure that there is a file to be reversed
 * result is satisfying
 */
void Inverser(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		wave_reverse(wave);
		printf("Reversing ...\n");
		sleep(1);
		clrscr();
	}
}

/*
 * To crop a wave file but make sure that there is a file to be cropped
 * result is not so satisfying
 */
void Decouper(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		wave_crop_sec(wave);
		printf("Croping ...\n");
		sleep(1);
		clrscr();
	} 
}

/*
 * to make the file faster or  slower and making sure that there is a wave file being used
 * result is not so satisfying
 */
void Redimensionner(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("You want your sample to be played faster or slower? (1) faster, (2) slower\n");
		int choix; scanf("%d",&choix);
		while(choix!=1 && choix !=2) scanf("%d",&choix);
		if(choix==1){
			printf("How faster you want to be played? 2 times faster or more (playing it so fast may destroy your file\n");
			printf("choix?");
			double s; scanf("%lf",&s);
			wave_scale(wave,s);     
		}else{ 
			printf("How slower you want to be played? 2 times slower or more (playing it so slow may destroy your file\n");
			printf("choix?");
			double s; scanf("%lf",&s);
			s = 1/s;
			wave_scale(wave,s);
		}
		printf("Redimensionning ...\n");
		sleep(1);
		clrscr();
	}
}

/*
 * to add channels to wave file and making sure that there is already a file being used
 * result is not satisfying ( problem when making more canals with a mono wave file)
 */
void ajout_Canal(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("You file has %u channals, how many channals do you want to add?\n",wave->header->nombreCanaux);
		int choix; scanf("%d",&choix);
		while((choix + wave->header->nombreCanaux == 3) || (choix + wave->header->nombreCanaux > 6)){
			printf("After your choice the number of channals will be \" %d \" and a normal wave file can not  have that much channals\n",(choix + wave->header->nombreCanaux));
			printf("How much channals do you want add?");
			scanf("%d",&choix);
		}

		wave = change_canal(wave,(choix + wave->header->nombreCanaux));     
		printf("Adding channals ...\n");
		sleep(1);
		clrscr();
	}
}

/*
 * to delete some channals from a wave file and making sure that there is already a wave file being used
 * result is satisfying
 */
void suprrime_Canal(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("You file has %u channals, how many channals do you want to delete?\n",wave->header->nombreCanaux);
		int choix; scanf("%d",&choix);
		while((wave->header->nombreCanaux-choix) < 0){
			printf("After your choice the number of channals will be \" %d \" and a normal wave file can not  have that much channals\n",(wave->header->nombreCanaux - choix));
			printf("How much channals do you want delete?");
			scanf("%d",&choix);
		}

		wave = change_canal(wave,(wave->header->nombreCanaux-choix));     
		printf("Deleting channals ...\n");
		sleep(1);
		clrscr();
	}
}

/*
 * to add the graphe of the cos() fonction to a wave created
 * result is not satisfying
 */
void Addcos(){
	if(exist){
		add_signal(wave,cos); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

void Addacos(){
	if(exist){
		add_signal(wave,acos); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

void Addacosh(){
	if(exist){
		add_signal(wave,acosh); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

/*
 * to add the graphe of the sin() fonction to a wave created
 * result is not satisfying
 */
void Addsin(){
	if(exist){
		add_signal(wave,sin); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();  
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}   

void Addasin(){
	if(exist){
		add_signal(wave,asin); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

void Addasinh(){
	if(exist){
		add_signal(wave,asinh); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

/*
 * to add the graphe of the tan() fonction to a wave created
 * result is not satisfying
 */
void Addtan(){
	if(exist){
		add_signal(wave,cos); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}        

void Addatan(){
	if(exist){
		add_signal(wave,atan); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

void Addatanh(){
	if(exist){
		add_signal(wave,atanh); 
		printf("Adding signal ..\n");
		sleep(1);
		clrscr();
	}else{
		printf("There is no file being used to be saved ...\n");
	}
}

void bits8Mon(){
	int choix; choix=getchar(); printf("choix %c",choix);
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("How you want name your file?");
		char name[100];
		scanf("%s",name);
		strcat(name,".wav");
		wave_canal(&wave,1);
		change_precision(wave,8);
		wave_save(name,wave);
		printf("Saving ...\n");
		sleep(1);
		clrscr();
	}
}

void bits16Stero(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("How you want name your file?");
		char name[100];
		scanf("%s",name);
		strcat(name,".wav");
		wave_canal(&wave,2);
		change_precision(wave,16);
		wave_save(name,wave);
		printf("Saving ...\n");
		sleep(1);
		clrscr();
	}
}

void bits24_6Canaux(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("How you want name your file?");
		char name[100];
		scanf("%s",name);
		strcat(name,".wav");
		wave_canal(&wave,6);
		change_precision(wave,24);
		wave_save(name,wave);
		printf("Saving ...\n");
		sleep(1);
		clrscr();
	}
}

void Augument_Volume(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("How many volume bars you want add?");
		float choix; scanf("%f",&choix);   
		wave_volume(wave,choix);
		printf("Highering the volume ...\n");
		sleep(1);
		clrscr();
	}
}

void Baisser_Volume(){
	if(!exist){
		printf("There is no file being used to be saved ...");
	}else{
		printf("How many volume bars you want delete?");
		float choix; scanf("%f",&choix);   
		wave_volume(wave,(1/choix));
		printf("Lowering the volume ...\n");
		sleep(1);
		clrscr();
	}
}

void optHelp(){
	printf("  -i OR -open [filename].wav, to load and open a wave file.\n\n");
	printf("  -o OR -save [filename].wav, to save a wave file.\n\n");
	printf("  -p OR -print [void] , to show the wave's header informations.\n\n");
	printf("  -r OR -reverse [void], to reverse a wave file.\n\n");
	printf("  -c OR -crop [unsigned int] [unsigned int], to crop a wave file [start][end].\n\n");
	printf("  -s OR -scal [float], to make your sample goes faster or slower.\n\n");
        printf("  -vd AND -vp [unsigned int], to higher or lower the volume of a wave file.\n\n\n");
	printf(" NOTICE : you need to load and save your wave file everytime.\n\n");

}
void optOpen(const char *fname){
	wave = wave_load_opt(wave,fname);
        if(wave != NULL) exist = true;
}

void optSave(const char *fname){
	if(exist){
		wave_save(fname,wave);
	}else{
		printf("you didn't open any file\n");
	}
}

void optInfo(){
	if(exist){
		wave_info(wave);
	}else{
		printf("you didn't open any file\n");
	}
}
void optReverse(){
	if(exist){
		wave_reverse(wave);
	}else{
		printf("you didn't open any file\n");
	}
}

void optCrop(uint32_t a,uint32_t b){
	if(exist){
		wave_crop(wave,a,b);
	}else{
		printf("you didn't open any file\n");
	}
}

void optScale(float f){
	if(exist){
		wave_scale(wave,f);
	}else{
		printf("you didn't open any file\n");
	}
}

void optVolumeUp(unsigned int v){
	if(exist){
		wave_volume(wave,(float)v);
	}else{
		printf("you didn't open any file\n");
	}
}

void optVolumeDown(unsigned int v){
	if(exist){
                printf("1/v = %f\n",(1/(float)v));
		wave_volume(wave,(1/(float)v));
	}else{
		printf("you didn't open any file\n");
	}
}

int main(int argc, char** argv){
	if(argc != 1){
		lunchOptions(argc,argv); 
		return EXIT_SUCCESS;
	}
	Menu_t * MENU = Prepare_Menu();
	addMenuAction(MENU,"Quitter",quit);
	while(cont) useMenu(MENU);
	return EXIT_SUCCESS;
}
