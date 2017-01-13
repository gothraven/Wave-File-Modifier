#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include"menu.h"

/*
 * to clear the screen
 * result is satisfying
 */
void clrscr(){
	system("@cls||clear");
}

/*
 * to ask for a choice
 * result is satisfying
 */
int ask_choice(void){
	int ch;
	ch = getchar();
	while('\n'!=getchar());

	return ch;
}

/*
 * the fonction that creats the menu
 * result is satisfying
 */

Menu_t* createMenu(const char * text){
	Menu_t * Menu = malloc(sizeof(Menu_t));
	Menu-> root = NULL;
	Menu-> desc = malloc(strlen(text));
	strcpy(Menu->desc,text);
	Menu->cmpt =0;
	return Menu;
}

/*
 * the fonction that adds a fonction to a menu
 * result is satisfying
 */
bool addMenuAction(Menu_t* m, const char* text, void(*f)()){
	if (m->cmpt != 9){
		m->items[m->cmpt] = malloc(sizeof(Type_t));
		m->items[m->cmpt]->item = malloc(sizeof(Item_t));
		m->items[m->cmpt]->item->action = malloc(sizeof(Action_t));
		m->items[m->cmpt]->item->action->descr = malloc(strlen(text)+1);
		m->items[m->cmpt]->tag=act;
		strcpy(m->items[m->cmpt]->item->action->descr,text);
		m->items[m->cmpt]->item->action->fonc = f;
		m->cmpt++;
		return true;
	}else{
		return false;
	}
}

/*
 * the fonction that add a menu to another to become its submenu
 * result is satisfying
 */
bool addSubMenu(Menu_t* m, Menu_t* sm){
	if(m->cmpt!=9){
		sm->root =  m;
		m->items[m->cmpt]=malloc(sizeof(Type_t));
		m->items[m->cmpt]->item=malloc(sizeof(Item_t));
		m->items[m->cmpt]->item->sous_menu = malloc(sizeof(sm));
		m->items[m->cmpt]->item->sous_menu = sm;
		m->items[m->cmpt]->tag=ssm;
		m->cmpt++;
		return true;
	}else{
		return false;
	}
}

/*
 * the fonction that lunches the menu
 * result is satisfying
 */
void useMenu(Menu_t *m){
	int choix;
	printf("\n");
	printf("%s\n\n",m->desc);
	for(int i=0; i<m->cmpt; i++){
		if(m->items[i]->tag==act){

			printf("  %d - %s\n",i+1,m->items[i]->item->action->descr);
		}else  if(m->items[i]->tag==ssm){
			printf("  %d - %s\n",i+1,m->items[i]->item->sous_menu->desc);
		}
	}
	printf("\nchoix?\n");
	scanf("%d",&choix);
	while(choix<0 || choix>m->cmpt){
		scanf("%d",&choix);
	}
	clrscr(); 
	if(m->items[choix-1]->tag==ssm){
		useMenu(m->items[choix-1]->item->sous_menu);
	}else  if(m->items[choix-1]->tag==act){
		m->items[choix-1]->item->action->fonc();
	}
}

/*
 * the fonction that deletes the menu
 * result is satisfying
 */
void deleteMenu(Menu_t *m){
	free(m);
}

void f(){
	printf("you choosed something");
}

/*
 * the fonction that makes it returns to the main menu
 * result is satisfying
 */
void returnn(){

}


Menu_t* Prepare_Menu(){
	clrscr();
	Menu_t* MENU;
	Menu_t* sm1;
	Menu_t* sm2;
	Menu_t* sm3;
	Menu_t* sm4;
	Menu_t* sm12;
	Menu_t* sm21;
	MENU = createMenu("Main Menu"); 

	sm1 = createMenu("File"); 
	if(!addMenuAction(sm1,"New",Nouveau)) printf("Problem making New\n"); 
	if(!addMenuAction(sm1,"Open",Ouvrir)) printf("Problem making Open\n");
	if(!addMenuAction(sm1,"Save",Sauvgarder)) printf("Problem making Save\n");

	sm12 = createMenu("Save as");  
	if(!addMenuAction(sm12,"8bits Mono 11.025kHz",bits8Mon)) printf("Problem making 8bits Mono 11.025kHz\n");
	if(!addMenuAction(sm12,"16bits Stereo 44,1kHz",bits16Stero)) printf("Problem making 16bits Stereo 44,1kHz\n");
	if(!addMenuAction(sm12,"24bits 5.1 192kHz",bits24_6Canaux)) printf("Problem making 24bits 5.1 192kHz\n");
	if(!addSubMenu(sm1,sm12)) printf("Problem making SAVE manu\n");

	if(!addMenuAction(sm1,"Information",Information)) printf("Problem making Information\n");
	if(!addMenuAction(sm1,"Return",returnn)) printf("Problem making Return\n");
	if(!addSubMenu(MENU,sm1)) printf("Problem adding sm1 to MENU \n");

	sm2 = createMenu("Duration and tempo");
	if(!addMenuAction(sm2,"Reverse",Inverser)) printf("Problem making Reverse\n");
	if(!addMenuAction(sm2,"Crop",Decouper)) printf("Problem making Crop\n");
	if(!addMenuAction(sm2,"Change Speed",Redimensionner)) printf("Problem making Change Speed\n");

	sm21 = createMenu("Add Signal");
	if(!addMenuAction(sm21,"Fonction cosinus",Addcos)) printf("Problem making Fonction cosinus\n");
	if(!addMenuAction(sm21,"Fonction arc cosinus",Addacos)) printf("Problem making Fonction arc cosinus\n");
	if(!addMenuAction(sm21,"Fonction arc cosinus hyperbolique",Addacosh)) printf("Problem making Fonction arc cosinus hyperbolique\n");

	if(!addMenuAction(sm21,"Fonction sinus",Addsin)) printf("Problem making sinus\n");
	if(!addMenuAction(sm21,"Fonction arc sinus",Addasin)) printf("Problem making arc sinus\n");
	if(!addMenuAction(sm21,"Fonction arc sinus hyperbolique",Addasinh)) printf("Problem making arc sinus hyperbolique\n");

	if(!addMenuAction(sm21,"Fonction tangente",Addtan)) printf("Problem making tangente\n");
	if(!addMenuAction(sm21,"Fonction arc tangente",Addatan)) printf("Problem making arc tangente\n");
	if(!addMenuAction(sm21,"Fonction arc tangente hyperbolique",Addtan)) printf("Problem making arc tangente hyperbolique\n");

	if(!addSubMenu(sm2,sm21)) printf("Problem adding sm21 to sm2\n");

	if(!addMenuAction(sm2,"Return",returnn)) printf("Problem making Return\n");
	if(!addSubMenu(MENU,sm2)) printf("Problem adding sm2 to MENU\n");

	sm3 = createMenu("Canals");
	if(!addMenuAction(sm3,"Add Canals",ajout_Canal)) printf("Problem making Add Canals\n");
	if(!addMenuAction(sm3,"Delete Canals",suprrime_Canal)) printf("Problem making Delete Canals\n");
	if(!addMenuAction(sm3,"Return",returnn)) printf("Problem making Return\n");
	if(!addSubMenu(MENU,sm3)) printf("Problem adding sm3 to MENU\n");

	sm4 = createMenu("Volume Height");
	if(!addMenuAction(sm4,"More volume",Augument_Volume)) printf("Problem making More volume\n");
	if(!addMenuAction(sm4,"Less volume",Baisser_Volume)) printf("Problem making Less volume\n");
	if(!addMenuAction(sm4,"Return",returnn)) printf("Problem making Return\n");
	if(!addSubMenu(MENU,sm4)) printf("Problem adding sm4 to MENU\n");

	return MENU;
}

