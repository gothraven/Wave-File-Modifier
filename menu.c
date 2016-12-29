#include"menu.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>

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
     m->items[choix-1]->item->action->fonc();
     printf("--------------------------------\n");
     
   }
}

//fonction pour vider le menu
void deleteMenu(Menu_t *m){
 free(m);
}

void ouvrir() {
  printf("Ouvrir un ficher\n");
}

void enregistrer() {
  printf("enregistrer un ficher\n");
}

void f3() {
  printf("Functionality 3 is called\n");
}

bool cont = true;

void quit() {
  cont = false;
}

Menu_t* prepareMenu(){
  Menu_t* m;
  Menu_t* sm1;
  Menu_t* sm2;
 
  m = createMenu("Fichier");
 
  addMenuAction(m,"Ouvrir",ouvrir);
  addMenuAction(m,"Enregistrer",enregistrer);
  addMenuAction(m,"Information",f3);
  
 
  sm1 = createMenu("Durée et tempo");
  addMenuAction(sm1,"Inverser",ouvrir);
  addMenuAction(sm1,"Découper",enregistrer);
  addMenuAction(sm1,"Redimensionner",f3);
  addSubMenu(m,sm1);

  sm2 = createMenu("Canaux");
  addMenuAction(sm2,"....",ouvrir);
  addMenuAction(sm2,"....",ouvrir);
  addSubMenu(sm1,sm2);
  
  addMenuAction(m,"Quitter",quit);
 
  //while(cont){
  //useMenu(m);
  //}

  return m;
}

