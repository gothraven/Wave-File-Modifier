
#include "menu.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


menu* createMenu(const char* text){
  menu* m = (menu*) malloc(sizeof(menu));
  if(m == NULL) exit(EXIT_FAILURE);
  m->menu_p = NULL;
  m->nom = malloc((strlen(text) + 1) * sizeof(char));
  if(m->nom == NULL) exit(EXIT_FAILURE);
  strcpy(m->nom,text);
  m->nbItem = 0;
  return m;
}

void addMenuAction(menu* m, const char* text, void(*f)()){
  action ac;
  ac.nomAction = malloc((strlen(text) + 1) * sizeof(char));
  strcpy(ac.nomAction,text);
  ac.act = f;
  item it;
  it.kind = action_k;
  it.item_s.action_u = ac;
  m->items[m->nbItem] = it;
  m->nbItem++;
}

void addSubMenu(menu* m, menu* sm){
  if(sm->menu_p == NULL){
    item it;
    it.kind = menu_k;
    sm->menu_p = m;
    it.item_s.menu_u = sm;
    m->items[m->nbItem] = it;
    m->nbItem++;
  }
}

void deleteMenu(menu* m){
    if(m->menu_p == NULL){
        if(m->nbItem != 0){
            int i = 0;
            for(i = 0; i < m->nbItem; i++)
                deleteMenu(m->items[i].item_s.menu_u);
        }
    }
    free(m);
}

//Permet de vider le buffer après la saisie d'une chaine via les fonctions
//scanf ou fgets
void viderBuffer(){
  int c = 0;
  while(c != '\n' && c != EOF) c = getchar();
}

//Permet de lire une chaine de caratère entrez par l'utilisateur
bool lire(char* chaine, int len){
  char* position = NULL;
  if(fgets(chaine,len,stdin) != NULL){
    position = strchr(chaine,'\n');
    if(position != NULL){
      *position = '\0';
    } 
    else {
      viderBuffer();
    }
    return true;
  } else {
    viderBuffer();
    return false;
  }
}

void launchMenu(menu* m){
    int i, cn;
    bool p = false;
    char c;
    printf("%s\n\n", m->nom);
    for(i = 0; i < m->nbItem; i++){
      switch(m->items[i].kind){
        case action_k : printf("   %d - %s\n\n", i+1, m->items[i].item_s.action_u.nomAction); break;
        case menu_k : printf("   %d - %s\n\n", i+1, m->items[i].item_s.menu_u->nom); break;
      }
    }
    do {
        printf("Choice ?\n");
        c = getchar();
        cn = (int)c;
        if(cn == 112){
          p = true;
          cn = 1;
        } else {
          cn -= 48;
        }
        viderBuffer();
    } while(cn > m->nbItem || cn < 1);
    cn--;
    if(p && m->menu_p != NULL) {
        system("clear");
        launchMenu(m->menu_p);
    } else if (cn > -1 && !p) {
        switch(m->items[cn].kind){
            case action_k : system("clear");
                            m->items[cn].item_s.action_u.act();
                            break;
            case menu_k :   system("clear");
                            launchMenu(m->items[cn].item_s.menu_u); break;
        }
    } else {
        system("clear");
    }
}










