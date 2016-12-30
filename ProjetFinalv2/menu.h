#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

typedef struct {
  char* nomAction;
  void (*act)();
} action;

typedef enum {
  action_k,
  menu_k,
} item_kind;

typedef union {
  struct menu_* menu_u;
  action action_u;
} item_u;

typedef struct {
  item_kind kind;
  item_u item_s;
} item;

typedef struct menu_ {
  struct menu_* menu_p;
  char* nom;
  item items[9];
  int nbItem;
} menu;

menu* createMenu(const char* text);
void addMenuAction(menu* m, const char* text, void(*f)());
void addSubMenu(menu* m, menu* sm);
void deleteMenu(menu* m);
void viderBuffer();
bool lire(char* chaine, int len);
void launchMenu(menu* m);


#endif