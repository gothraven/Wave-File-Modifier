#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include "args.h"
#include "wave.h"
#include "animation.h"

#define ENTER 10
#define ESCAPE 27

bool exist = false;

Wave_t* wave = NULL;

WINDOW *messagebar;

//NEEDED FONCTIONS
int get_number(){
 char * choice = malloc(11* sizeof(char)); 
 int result = getnstr(choice,10);
 return result = atoi(choice);
}

float get_floatnumber(){
 char * choice = malloc(11* sizeof(char)); 
 float result = getnstr(choice,10);
 return result = atof(choice);
}

char * get_name(){
  char * name = malloc(20*sizeof(char));
  getnstr(name,19);
  return name;
}

//MENU FONCTIONS
void Nouveau(WINDOW * messagebar){
    	if(exist){
		wprintw(messagebar,"There is already a wave file being used, don't you want save it? (y)es, (n)o\n");
		wrefresh(messagebar);
                char key = getch();
		while(key != 'Y' && key != 'y' && key !='N' && key !='n'){ 
			key = getch();
		}
		if(key=='Y' || key=='y'){
                        wclear(messagebar);
			wprintw(messagebar,"How you want name your file?");
                        wrefresh(messagebar);
			char * name = get_name();   
			strcat(name,".wav");
			wave_save(name,wave);
			wprintw(messagebar," %s [ Your file is saved ]",name);
                        wrefresh(messagebar);
                        sleep(1);
                        wclear(messagebar);
		}
	} 
	wclear(messagebar);
	wprintw(messagebar,"frequance: ");
        wrefresh(messagebar);
        uint32_t f = get_number();
	wprintw(messagebar,"%u",f);
        wrefresh(messagebar);
	wprintw(messagebar,", precision: ");
        wrefresh(messagebar);
        uint16_t p = get_number();
	wprintw(messagebar,"%u",p);
        wrefresh(messagebar);
	wprintw(messagebar,", canals: ");
        wrefresh(messagebar);
        uint16_t c = get_number();
	wprintw(messagebar,"%u",c);
        wrefresh(messagebar);
	wprintw(messagebar,", seconds: ");
        wrefresh(messagebar);
        int sec = get_number();
	wprintw(messagebar,"%u",sec);
        wrefresh(messagebar);
        uint16_t b = c*(p/8);
	uint32_t r = f * b;
        uint32_t B = (uint32_t)((sec * r)/( c * p/8)); //get_number();
	wave = wave_new(f,p,c,B);
	exist = true;
        sleep(1);
        wclear(messagebar);
	wprintw(messagebar,"[Your file is created]\n");
    
}    

void Ouvrir(WINDOW * messagebar){
    	if(exist){
		wprintw(messagebar,"There is already a wave file being used, don't you want save it? (y)es, (n)o\n");
		wrefresh(messagebar);
                char key = getch();
		while(key != 'Y' && key != 'y' && key !='N' && key !='n'){ 
			key = getch();
		}
		if(key=='Y' || key=='y'){
                        wclear(messagebar);
			wprintw(messagebar,"How you want name your file?");
                        wrefresh(messagebar);
			char * name = get_name();   
			strcat(name,".wav");
			wave_save(name,wave);
			wprintw(messagebar," %s [ Your file is saved ]",name);
                        wrefresh(messagebar);
                        sleep(1);
                        wclear(messagebar);
		}
	}
	wclear(messagebar);
	wprintw(messagebar,"file name : ");
        wrefresh(messagebar);
        char * name = get_name();   
        strcat(name,".wav");
        FILE * ptr; 
	ptr = fopen(name, "rb");
        if (ptr == NULL) {
         wprintw(messagebar," %s doesn't existe!!",name);
         wrefresh(messagebar);
        }else{
	wave = wave_load(wave,name); 
        wprintw(messagebar," %s [ Your file is Loaded ]",name);
	exist = true;
        }
}

void Sauvgarder(WINDOW * messagebar){
    	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
			wprintw(messagebar,"How you want name your file?");
                        wrefresh(messagebar);
			char * name = get_name();   
			strcat(name,".wav");
			wave_save(name,wave);
			wprintw(messagebar," %s [ Your file is saved ]",name);
                        wrefresh(messagebar);
	}
}

void Sauvgarderbits8Mono(WINDOW * messagebar){
    	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"How you want name your file?");
                wrefresh(messagebar);
		char * name = get_name();   
                strcat(name,".wav");
		wave_canal(&wave,1);
		change_precision(wave,8);
		wave_save(name,wave);
		wprintw(messagebar," %s [ Your file is converted & saved ]",name);
                wrefresh(messagebar);
	}
}

void Sauvgarderbits16Stero(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"How you want name your file?");
                wrefresh(messagebar);
		char * name = get_name();   
                strcat(name,".wav");
		wave_canal(&wave,2);
		change_precision(wave,16);
		wave_save(name,wave);
		wprintw(messagebar," %s [ Your file is converted & saved ]",name);
                wrefresh(messagebar);
	}
}

void Sauvgarderbits24_6Canaux(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"How you want name your file?");
                wrefresh(messagebar);
		char * name = get_name();   
                strcat(name,".wav");
		wave_canal(&wave,6);
		change_precision(wave,24);
		wave_save(name,wave);
		wprintw(messagebar," %s [ Your file is converted & saved ]",name);
                wrefresh(messagebar);
	}
}

void Information(Wave_t * wave,WINDOW * messagebar){
    if(!exist){
        wprintw(messagebar,"there is no file being used ...");
    }else{
        move(4,0);
	printw("RIFF : %c%c%c%c\n",wave->header->riff[0],wave->header->riff[1],wave->header->riff[2],wave->header->riff[3]);
	printw("Taille : %u oct\n",wave->header->taille);
	printw("WAVE : %c%c%c%c\n",wave->header->wave[0],wave->header->wave[1],wave->header->wave[2],wave->header->wave[3]);
	printw("fmt_ : %c%c%c%c\n",wave->header->fmt[0],wave->header->fmt[1],wave->header->fmt[2],wave->header->fmt[3]);
	printw("SubTaille1 : %u\n",wave->header->subTaille1);
	printw("Audio format : %u PCM\n",wave->header->formatAudio);
	printw("Cannals number: %u\n",wave->header->nombreCanaux);
	printw("Frequence : %u\n",wave->header->freqEch); 
	printw("ByteRate : %u\n",wave->header->ByteRate);
	printw("Align : %u\n",wave->header->align);
	printw("Precision : %u\n",wave->header->bitsParEch);
	printw("Ndata : %c%c%c%c\n",wave->header->Ndata[0],wave->header->Ndata[1],wave->header->Ndata[2],wave->header->Ndata[3]);
	printw("SubTaille2 : %u\n",wave->header->subTaille2);
	uint32_t nb_Blocs = (wave->header->subTaille2) / (wave->header->nombreCanaux * wave->header->bitsParEch/8);
	printw("Number of Blocs = %u\n",nb_Blocs);
	float seconds = (float)(wave->header->subTaille2) / (wave->header->ByteRate);
	printw("The time in seconds => %f \n",seconds);
	printw("File time h:m:s:ms => %s\n",seconds_to_time(seconds));
        refresh();

   }
}

void Quit(WINDOW * messagebar){
    	if(exist){
		wprintw(messagebar,"There is already a wave file being used, don't you want save it? (y)es, (n)o\n");
		wrefresh(messagebar);
                char key = getch();
		while(key != 'Y' && key != 'y' && key !='N' && key !='n'){ 
			key = getch();
		}
		if(key=='Y' || key=='y'){
                        wclear(messagebar);
			wprintw(messagebar,"How you want name your file?");
                        wrefresh(messagebar);
			char * name = get_name();   
			strcat(name,".wav");
			wave_save(name,wave);
			wprintw(messagebar," %s [ Your file is saved ]",name);
                        wrefresh(messagebar);
                        sleep(1);
                        wclear(messagebar);
		}
	}
}

void Inverser(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wave_reverse(wave);
		wprintw(messagebar,"[ Your file is reversed ]");
                wrefresh(messagebar);
	}
}

void Decouper(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
                wprintw(messagebar,"Start bloc : ");
                wrefresh(messagebar);
                uint32_t start = get_number();
	        wprintw(messagebar,"%u ",start);
                wprintw(messagebar,",End bloc : ");
                wrefresh(messagebar);
                uint32_t end = get_number();
	        wprintw(messagebar,"%u ",end);
                wrefresh(messagebar);
		wave_crop(wave,start,end);
		wprintw(messagebar,"[ Your file is cropped ]");
                wrefresh(messagebar);
	} 
}

void Faster(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
                wprintw(messagebar,"How faster you want it to be : ");
                wrefresh(messagebar);
                float rate = get_floatnumber();
	        wprintw(messagebar,"%d ",(int)rate);
                wrefresh(messagebar);
		wave_scale(wave,rate);
		wprintw(messagebar,"[ Your file is %d time(s) faster ]",(int)rate);
                wrefresh(messagebar);
	} 
}

void Slower(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
                wprintw(messagebar,"How slower you want it to be : ");
                wrefresh(messagebar);
                float rate = get_floatnumber();
	        wprintw(messagebar,"%d ",(int)rate);
                wrefresh(messagebar);
                rate = 1/rate;
		wave_scale(wave,rate);
		wprintw(messagebar,"[ Your file is %d time(s) slower ]",(int)(1/rate));
                wrefresh(messagebar);
	} 
}

void Addcos(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,cos);
		wprintw(messagebar,"[ Cosinus function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addacos(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,acos);
		wprintw(messagebar,"[ Arc Cosinus function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addacosh(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,acosh);
		wprintw(messagebar,"[ Arc Cosinus Hyperbolique function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addsin(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,sin);
		wprintw(messagebar,"[ Sinus function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addasin(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,asin);
		wprintw(messagebar,"[ Arc sinus function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addasinh(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,asinh);
		wprintw(messagebar,"[ Arc Sinus Hyperbolique function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addtan(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,tan);
		wprintw(messagebar,"[ Tangente function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addatan(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,atan);
		wprintw(messagebar,"[ Arc Tangente function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void Addatanh(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		add_signal(wave,atanh);
		wprintw(messagebar,"[ Arc Tangente Hyperbolique function is generated in all of your file ]");
                wrefresh(messagebar);
	} 
}

void ajout_Canal(WINDOW * messagebar){
	int choix;
        if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"You file has %u channals, how many channals do you want to add? : ",wave->header->nombreCanaux);
                wrefresh(messagebar);
		choix = get_number();
                wprintw(messagebar,"%d ",choix);
		while((choix + wave->header->nombreCanaux == 3) || (choix + wave->header->nombreCanaux > 6)){
                        wclear(messagebar);
			wprintw(messagebar,"A normal wave file can not have %d canals",(choix + wave->header->nombreCanaux));
                        wrefresh(messagebar);
                        sleep(2);
                        wclear(messagebar);
			wprintw(messagebar,"How much channals do you want add? : ");
                        wrefresh(messagebar);
			choix = get_number();
                        wprintw(messagebar,"%d cannal(s)",choix);
                        wrefresh(messagebar);
                        sleep(1);
		}
		wave = change_canal(wave,(choix + wave->header->nombreCanaux));
                wclear(messagebar);
		wprintw(messagebar,"[ Adding %d Canals ]",choix);
		wrefresh(messagebar);
	}
}

void suprrime_Canal(WINDOW * messagebar){
	int choix;
        if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"You file has %u channals, how many channals do you want to delete? : ",wave->header->nombreCanaux);
                wrefresh(messagebar);
		choix = get_number();
                wprintw(messagebar,"%d ",choix);
		while((wave->header->nombreCanaux - choix == 3) || (wave->header->nombreCanaux - choix <= 0)){
                        wclear(messagebar);
			wprintw(messagebar,"A normal wave file can not have %d canals",(wave->header->nombreCanaux - choix));
                        wrefresh(messagebar);
                        sleep(2);
                        wclear(messagebar);
			wprintw(messagebar,"How much channals do you want delete? : ");
                        wrefresh(messagebar);
			choix = get_number();
                        wprintw(messagebar,"%d cannal(s)",choix);
                        wrefresh(messagebar);
                        sleep(1);
		}
		wave = change_canal(wave,(wave->header->nombreCanaux-choix));
                wclear(messagebar);
		wprintw(messagebar,"[ Deleting %d Canal(s) ]",choix);
		wrefresh(messagebar);
	}
}

void Augument_Volume(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"How many volume bars you want add? : ");
		wrefresh(messagebar);
                float rate = get_floatnumber();
	        wprintw(messagebar,"%d bar(s)",(int)rate);
                wrefresh(messagebar); 
		wave_volume(wave,rate);
                sleep(2);
                wclear(messagebar);
		wprintw(messagebar,"[ Adding %d volume bars ]",(int)rate);
		wrefresh(messagebar);
	}
}

void Baisser_Volume(WINDOW * messagebar){
	if(!exist){
		wprintw(messagebar,"There is no file being used to be saved ...");
	}else{
		wprintw(messagebar,"How many volume bars you want delete? : ");
		wrefresh(messagebar);
                float rate = get_floatnumber();
	        wprintw(messagebar,"%d bar(s)",(int)rate);
                wrefresh(messagebar); 
                rate = 1/rate;
		wave_volume(wave,rate);
                sleep(2);
                wclear(messagebar);
		wprintw(messagebar,"[ Deleting %d volume bars ]",(int)(1/rate));
		wrefresh(messagebar);
	}
}
//GRAPHICS
void init_curses(){
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLUE);
	init_pair(2,COLOR_BLUE,COLOR_WHITE);
	init_pair(3,COLOR_RED,COLOR_WHITE);
	curs_set(0);
	noecho();
	keypad(stdscr,TRUE);
}

void draw_menubar(WINDOW *menubar){
	wbkgd(menubar,COLOR_PAIR(2));
	waddstr(menubar,"File");
	wattron(menubar,COLOR_PAIR(3));
	waddstr(menubar,"(A)");
	wattroff(menubar,COLOR_PAIR(3));

	wmove(menubar,0,9);
	waddstr(menubar,"Duration & tempo");
	wattron(menubar,COLOR_PAIR(3));
	waddstr(menubar,"(Z)");
	wattroff(menubar,COLOR_PAIR(3));

	wmove(menubar,0,29);
	waddstr(menubar,"Canals");
	wattron(menubar,COLOR_PAIR(3));
	waddstr(menubar,"(E)");
	wattroff(menubar,COLOR_PAIR(3));

	wmove(menubar,0,39);
	waddstr(menubar,"Volume Height");
	wattron(menubar,COLOR_PAIR(3));
	waddstr(menubar,"(R)");
	wattroff(menubar,COLOR_PAIR(3));
}

WINDOW ** draw_menu_ficher(int start_col){
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(7*sizeof(WINDOW *));

	items[0]=newwin(8,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,17,2,start_col+1);
	items[2]=subwin(items[0],1,17,3,start_col+1);
	items[3]=subwin(items[0],1,17,4,start_col+1);
	items[4]=subwin(items[0],1,17,5,start_col+1);
	items[5]=subwin(items[0],1,17,6,start_col+1);
	items[6]=subwin(items[0],1,17,7,start_col+1);

	wprintw(items[1],"New");
	wprintw(items[2],"Open");
	wprintw(items[3],"Save");
	wprintw(items[4],"Save As");
	wprintw(items[5],"Informations");
	wprintw(items[6],"Exit");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_menu_duree(int start_col){
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(5*sizeof(WINDOW *));

	items[0]=newwin(6,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,17,2,start_col+1);
	items[2]=subwin(items[0],1,17,3,start_col+1);
	items[3]=subwin(items[0],1,17,4,start_col+1);
	items[4]=subwin(items[0],1,17,5,start_col+1);

	wprintw(items[1],"Reverse");
	wprintw(items[2],"Crope");
	wprintw(items[3],"Change Speed");
	wprintw(items[4],"Add Signal");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_menu_canaux(int start_col){
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(3*sizeof(WINDOW *));

	items[0]=newwin(4,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,17,2,start_col+1);
	items[2]=subwin(items[0],1,17,3,start_col+1);

	wprintw(items[1],"Add Canals");
	wprintw(items[2],"Delete Canals");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_menu_hauteur(int start_col){
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(3*sizeof(WINDOW *));

	items[0]=newwin(4,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,17,2,start_col+1);
	items[2]=subwin(items[0],1,17,3,start_col+1);

	wprintw(items[1],"More Volume");
	wprintw(items[2],"Less Volume");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_submenu_save_as(int start_col){
	WINDOW **items;
	items=(WINDOW **)malloc(4*sizeof(WINDOW *));

	items[0]=newwin(5,24,5,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,21,6,start_col+1);
	items[2]=subwin(items[0],1,21,7,start_col+1);
	items[3]=subwin(items[0],1,21,8,start_col+1);

        wprintw(items[1],"8bits Mono 11.25kHz");
	wprintw(items[2],"16bits Stereo 44.1kHz");
        wprintw(items[3],"24bits 5.1 192kHz");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_submenu_add_signal(int start_col){
	WINDOW **items;
	items=(WINDOW **)malloc(4*sizeof(WINDOW *));

	items[0]=newwin(5,12,5,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,10,6,start_col+1);
	items[2]=subwin(items[0],1,10,7,start_col+1);
	items[3]=subwin(items[0],1,10,8,start_col+1);

        wprintw(items[1],"cosinus()");
	wprintw(items[2],"sinus()");
        wprintw(items[3],"tangente()");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_subsubmenu_speed(int start_col){
	WINDOW **items;
	items=(WINDOW **)malloc(3*sizeof(WINDOW *));

	items[0]=newwin(4,12,4,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,10,5,start_col+1);
	items[2]=subwin(items[0],1,10,6,start_col+1);


        wprintw(items[1],"Faster");
	wprintw(items[2],"Slower");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_subsubmenu_cos(int start_col){
	WINDOW **items;
	items=(WINDOW **)malloc(4*sizeof(WINDOW *));

	items[0]=newwin(5,29,6,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,27,7,start_col+1);
	items[2]=subwin(items[0],1,27,8,start_col+1);
	items[3]=subwin(items[0],1,27,9,start_col+1);

        wprintw(items[1],"cosinus()");
	wprintw(items[2],"arc cosinus()");
        wprintw(items[3],"arc cosinus hyperbolique()");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_subsubmenu_sin(int start_col){
	WINDOW **items;
	items=(WINDOW **)malloc(4*sizeof(WINDOW *));

	items[0]=newwin(5,29,7,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,27,8,start_col+1);
	items[2]=subwin(items[0],1,27,9,start_col+1);
	items[3]=subwin(items[0],1,27,10,start_col+1);

        wprintw(items[1],"sinus()");
	wprintw(items[2],"arc sinus()");
        wprintw(items[3],"arc sinus hyperbolique()");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_subsubmenu_tan(int start_col){
	WINDOW **items;
	items=(WINDOW **)malloc(4*sizeof(WINDOW *));

	items[0]=newwin(5,29,8,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,27,9,start_col+1);
	items[2]=subwin(items[0],1,27,10,start_col+1);
	items[3]=subwin(items[0],1,27,11,start_col+1);

        wprintw(items[1],"tangente()");
	wprintw(items[2],"arc tangente()");
        wprintw(items[3],"arc tangente hyperbolique()");

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW ** draw_menu(int start_col){
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(9*sizeof(WINDOW *));

	items[0]=newwin(10,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	items[1]=subwin(items[0],1,17,2,start_col+1);
	items[2]=subwin(items[0],1,17,3,start_col+1);
	items[3]=subwin(items[0],1,17,4,start_col+1);
	items[4]=subwin(items[0],1,17,5,start_col+1);
	items[5]=subwin(items[0],1,17,6,start_col+1);
	items[6]=subwin(items[0],1,17,7,start_col+1);
	items[7]=subwin(items[0],1,17,8,start_col+1);
	items[8]=subwin(items[0],1,17,9,start_col+1);

	for(i=1;i<9;i++){
		wprintw(items[i],"Item%d",i);
	}

	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

void delete_menu(WINDOW ** items,int count){
	int i;
	for (i=0;i<count;i++)
		delwin(items[i]);
	free(items);
}

int scroll_menu(WINDOW **items,int count,int menu_start_col){
	int key;
	int selected=0;
	while (1) {
		key=getch();
		if (key==KEY_DOWN || key==KEY_UP) {
			wbkgd(items[selected+1],COLOR_PAIR(2));
			wnoutrefresh(items[selected+1]);
			if (key==KEY_DOWN) {
				selected=(selected+1) % count;
			} else {
				selected=(selected+count-1) % count;
			}
			wbkgd(items[selected+1],COLOR_PAIR(1));
			wnoutrefresh(items[selected+1]);
			doupdate();
		} else if (key==ESCAPE) {
			return -1;
		} else if (key==ENTER) {
			return selected;
		}
	}
}

//OPTIONS PART//
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
	animation();
	int key;
	WINDOW *menubar;
	//WINDOW *messagebar;
	WINDOW **items[3];
	init_curses();
	bkgd(COLOR_PAIR(1));
	menubar=subwin(stdscr,1,80,0,0);
	messagebar=subwin(stdscr,1,79,23,1);
	draw_menubar(menubar);
	move(2,1);
	printw("Press A, Z, E or R to open the menus. ");
	printw("ESC quits.");
	refresh();

	do{
		int selected_item;
		WINDOW ** menu_items;
		key=getch();
		werase(messagebar);
		wrefresh(messagebar);
		if(key=='a' || key=='A'){
			menu_items=draw_menu_ficher(0);
			selected_item=scroll_menu(menu_items,6,0);
			delete_menu(menu_items,4);
			if(selected_item < 0){  
                            wprintw(messagebar,"You didn't select item...");
			}else if(selected_item == 0){ //NEW
                            Nouveau(messagebar);
			}else if(selected_item == 1){ //LOAD
                            Ouvrir(messagebar);
			}else if(selected_item == 2){ //SAVE
                            Sauvgarder(messagebar);
			}else if(selected_item == 3){ //SAVE AS//
                                menu_items=draw_submenu_save_as(19);
			        selected_item=scroll_menu(menu_items,3,0);
                                delete_menu(menu_items,4);
                                if(selected_item < 0){
                                    wprintw(messagebar,"You didn't select item...");
                                }else if(selected_item == 0){ //8bitsMono//
                                    Sauvgarderbits8Mono(messagebar);
                                }else if(selected_item == 1){ //16bitsStereo//
                                    Sauvgarderbits16Stero(messagebar);
                                }else if(selected_item == 2){ //24bits5.1//
                                    Sauvgarderbits24_6Canaux(messagebar);
                                }else{
                                    wprintw(messagebar,"ERROR");
                                }
			}else if(selected_item == 4){ //INFORMATIONS
				Information(wave,messagebar);
			}else if(selected_item == 5){ //EXIT
                                Quit(messagebar);
                                key=ESCAPE;
			}else{
				wprintw(messagebar,"ERROR");
			}
			touchwin(stdscr);
			refresh();
		}else if(key=='z' || key=='Z') {
			menu_items=draw_menu_duree(9);
			selected_item=scroll_menu(menu_items,4,11);
			delete_menu(menu_items,4);
			if(selected_item < 0){  
				wprintw(messagebar,"You didn't select item...");
			}else if(selected_item == 0){ //INVERSER//
				Inverser(messagebar);
			}else if(selected_item == 1){ //DECOUPER//
				Decouper(messagebar);
			}else if(selected_item == 2){
                                menu_items=draw_subsubmenu_speed(28);
			        selected_item=scroll_menu(menu_items,2,0);
                                delete_menu(menu_items,3);
                                if(selected_item < 0){
                                    wprintw(messagebar,"You didn't select item...");
                                }else if(selected_item == 0){ //FASTER//
                                    Faster(messagebar);
                                }else if(selected_item == 1){ //SLOWER//
                                    Slower(messagebar);
                                }else{
                                    wprintw(messagebar,"ERROR");
                                }
			}else if(selected_item == 3){ //ADD SIGNAL//
                                menu_items=draw_submenu_add_signal(28);
			        selected_item=scroll_menu(menu_items,3,0);
                                delete_menu(menu_items,4);
                                if(selected_item < 0){
                                    wprintw(messagebar,"You didn't select item...");
                                }else if(selected_item == 0){ //COS MENU//
                                    menu_items=draw_subsubmenu_cos(40);
			            selected_item=scroll_menu(menu_items,3,0);
                                    delete_menu(menu_items,4);
                                    if(selected_item < 0){
                                        wprintw(messagebar,"You didn't select item...");
                                    }else if(selected_item == 0){
                                        Addcos(messagebar);
                                    }else if(selected_item == 1){
                                        Addacos(messagebar);
                                    }else if(selected_item == 2){
                                        Addacosh(messagebar);
                                    }else{
                                        wprintw(messagebar,"ERROR");
                                    }
                                }else if(selected_item == 1){
                                    menu_items=draw_subsubmenu_sin(40);
			            selected_item=scroll_menu(menu_items,3,0);
                                    delete_menu(menu_items,4);
                                   if(selected_item < 0){
                                        wprintw(messagebar,"You didn't select item...");
                                    }else if(selected_item == 0){
                                        Addsin(messagebar);
                                    }else if(selected_item == 1){
                                        Addasin(messagebar);
                                    }else if(selected_item == 2){
                                        Addasinh(messagebar);
                                    }else{
                                        wprintw(messagebar,"ERROR");
                                    }
                                }else if(selected_item == 2){
                                    menu_items=draw_subsubmenu_tan(40);
			            selected_item=scroll_menu(menu_items,3,0);
                                    delete_menu(menu_items,4);
                                   if(selected_item < 0){
                                        wprintw(messagebar,"You didn't select item...");
                                    }else if(selected_item == 0){
                                        Addtan(messagebar);
                                    }else if(selected_item == 1){
                                        Addatan(messagebar);
                                    }else if(selected_item == 2){
                                        Addatanh(messagebar);
                                    }else{
                                        wprintw(messagebar,"ERROR");
                                    }
                                }else{
                                    wprintw(messagebar,"ERROR");
                                }
			}else{
				wprintw(messagebar,"ERROR");
			}
			touchwin(stdscr);
			refresh();
		}else if(key=='e' || key=='E') {
			menu_items=draw_menu_canaux(29);
			selected_item=scroll_menu(menu_items,2,20);
			delete_menu(menu_items,4);
			if(selected_item < 0){  
				wprintw(messagebar,"You didn't select item...");
			}else if(selected_item == 0){
				ajout_Canal(messagebar);
			}else if(selected_item == 1){
				suprrime_Canal(messagebar);
			}else{
				wprintw(messagebar,"ERROR");
			}
			touchwin(stdscr);
			refresh();
		}else if(key=='r' || key=='R' ) {
			menu_items=draw_menu_hauteur(39);
			selected_item=scroll_menu(menu_items,2,20);
			delete_menu(menu_items,9);
			if(selected_item < 0){  
				wprintw(messagebar,"You didn't select item...");
			}else if(selected_item == 0){
				Augument_Volume(messagebar);
			}else if(selected_item == 1){
				Baisser_Volume(messagebar);
			}else{
				wprintw(messagebar,"ERROR");
			}
			touchwin(stdscr);
			refresh();
		}
	}while (key!=ESCAPE);

	delwin(menubar);
	delwin(messagebar);
	endwin();

	return EXIT_SUCCESS;
}

