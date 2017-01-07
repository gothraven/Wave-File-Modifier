#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>




typedef struct Menu Menu_t;

typedef struct Action_t{  // structure pour les actions
	char * descr ;
	void (*fonc)();
}Action_t;


typedef union Item_t { 
	Menu_t * sous_menu;
	Action_t * action;
}Item_t;

typedef enum Tag {
	act=0, ssm=1
}Tag_t;

typedef struct Type {
	Item_t *item;
	Tag_t tag;
}Type_t;

struct Menu{  // structure principal des menu
	Menu_t *root  ;
	char *desc;
	Type_t * items[9];
	int cmpt;
};

void clrscr();
int ask_choice(void);
Menu_t* createMenu(const char * text);
bool addMenuAction(Menu_t* m, const char* text, void(*f)());
bool addSubMenu(Menu_t* m, Menu_t* sm);
void useMenu(Menu_t *m);
void deleteMenu (Menu_t *m);
void returnn();
void f();

/*void Ouvrir();
void Sauvgarder();
void Information();
void Inverser();
void Decouper();
void Redimensionner();
void ajout_Canal();
void suprrime_Canal();
void Nouveau();
void Addcos();
void Addsin();
void Addtan();
void bits8Mon();
void bits16Stero();
void bits24_6Canaux();
void Augument_Volume();
void Baisser_Volume();
*/
//Menu_t* Prepare_Menu();

bool cont =true;
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
	printf("\nchoix?");
	scanf("%d",&choix);
	while(choix<0 || choix>m->cmpt){
		scanf("%d",&choix);
	}
	clrscr(); 
	if(m->items[choix-1]->tag==ssm){
		useMenu(m->items[choix-1]->item->sous_menu);
	}else  if(m->items[choix-1]->tag==act){
		//if(m->items[choix-1]->tag==act == returnn){
		//	useMenu(m->root);
		//}else{
			m->items[choix-1]->item->action->fonc();
		//}
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
	   int choix; printf("give a number:"); scanf("%d",&choix);
           printf("you choosed %d",choix);
}

/*
 * the fonction that makes it returns to the main menu
 * result is satisfying
 */
void returnn(){

}


int main(){
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
	addMenuAction(sm1,"New",f);
	addMenuAction(sm1,"Open",f);
	addMenuAction(sm1,"Save",f); 

	sm12 = createMenu("Save as");  
	addMenuAction(sm12,"8bits Mono 11.025kHz",f);
	addMenuAction(sm12,"16bits Stereo 44,1kHz",f);
	addMenuAction(sm12,"24bits 5.1 192kHz",f);
	addSubMenu(sm1,sm12);

	addMenuAction(sm1,"Information",f);
	addMenuAction(sm1,"return",returnn);
	addSubMenu(MENU,sm1);

	sm2 = createMenu("Duration and tempo");
	addMenuAction(sm2,"Reverse",f);
	addMenuAction(sm2,"Crop",f);
	addMenuAction(sm2,"Change Speed",f);
	
        sm21 = createMenu("Add Signal");
        addMenuAction(sm21,"Fonction cos()",f);
        addMenuAction(sm21,"Fonction sin()",f);
        addMenuAction(sm21,"Fonction tan()",f);
	addSubMenu(sm2,sm21);
       
        addMenuAction(sm2,"return",returnn);
	addSubMenu(MENU,sm2);

	sm3 = createMenu("Canals");
	addMenuAction(sm3,"Add Canals",f);
	addMenuAction(sm3,"Delete Canals",f);
	addMenuAction(sm3,"return",returnn);
	addSubMenu(MENU,sm3);

	sm4 = createMenu("Volume Height");
	addMenuAction(sm4,"More volume",f);
	addMenuAction(sm4,"Less volume",f);
	addMenuAction(sm4,"return",returnn);
	addSubMenu(MENU,sm4);


        while(cont) useMenu(MENU);

	return 0;
}

