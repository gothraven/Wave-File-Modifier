#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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
Menu_t* Prepare_Menu();


void Ouvrir();
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

#endif
