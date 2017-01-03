#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include"menu.h"

void clrscr(){
    system("@cls||clear");
}
// la fonction de c_reation des menu
Menu_t* createMenu(const char * text){
 Menu_t * Menu = malloc(sizeof(Menu_t));
 Menu-> root = NULL;
 Menu-> desc = malloc(strlen(text));
 strcpy(Menu->desc,text);
 Menu->cmpt =0;
 return Menu;
}

//fonction pour ajouter une fonction a un menu
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

//fonction pour ajouter des sous menu
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

//fonction pour lancer le menu
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
      if(m->items[choix-1]->tag==act == returnn){
        useMenu(m->root);
      }else{
     m->items[choix-1]->item->action->fonc();
      }
   }
}

//fonction pour vider le menu
void deleteMenu(Menu_t *m){
 free(m);
}

bool cont = true;

void quit() {
  cont = false;
}

void f(){
  printf("you choosed something");
}

//bool cont = true;

void returnn(){

}


Menu_t* Prepare_Menu(){
  clrscr();
  Menu_t* MENU;
  Menu_t* sm1;
  Menu_t* sm2;
  Menu_t* sm3;
  Menu_t* sm4;
 
  MENU = createMenu("Main"); 
  
  sm1 = createMenu("Fichier");
  addMenuAction(sm1,"Ouvrir",f);
  addMenuAction(sm1,"Enregistrer",f);
  addMenuAction(sm1,"Information",f);
  addMenuAction(sm1,"return",returnn);
  addSubMenu(MENU,sm1);
 
  sm2 = createMenu("Durée et tempo");
  addMenuAction(sm2,"Inverser",f);
  addMenuAction(sm2,"Découper",f);
  addMenuAction(sm2,"Redimensionner",f);
  addMenuAction(sm2,"return",returnn);
  addSubMenu(MENU,sm2);

  sm3 = createMenu("Canaux");
  addMenuAction(sm3,"ajouter",f);
  addMenuAction(sm3,"suprimer",f);
  addMenuAction(sm3,"return",returnn);
  addSubMenu(MENU,sm3);

  sm4 = createMenu("Hauteur");
  addMenuAction(sm4,"more volume",f);
  addMenuAction(sm4,"less volume",f);
  addMenuAction(sm4,"return",returnn);
  addSubMenu(MENU,sm4);
  
  return MENU;
}

void init_curses(){
    initscr();
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    init_pair(2,COLOR_BLUE,COLOR_WHITE);
    init_pair(3,COLOR_RED,COLOR_WHITE);
    //curs_set(0);
    //noecho();
    //keypad(stdscr,TRUE);
}

int main(){
 init_curses();
 bkgd(COLOR_PAIR(1));
 Menu_t* MENU = Prepare_Menu();
 addMenuAction(MENU,"Quitter",quit);

 while(cont) useMenu(MENU);


return 0;
}

