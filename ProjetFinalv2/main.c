#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "menu.h"
#include "args.h"


bool cont = true;
bool charge = false; //indique si une image est chargée en mémoire
image* im = NULL; //image courante
char nomFichier[30]; //nom du fichier d'origine de l'image courante

//permet de mettre dans "ext" l'extension de la chaine "str"
void trouverExtension(const char* str, char* ext){
  if(strchr(str,46) != NULL){
    strcpy(ext,strchr(str,46));  
  } else {
    ext = NULL;
  }
}

void SauvegarderPBM_bin(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  char chaine[30];
  printf("Entrez un nouveau nom de fichier ou appuyez sur entrer pour conserver '%s'\nAttention dans ce dernier cas le contenu de '%s' sera écrasé : ",nomFichier,nomFichier);
  lire(chaine,30);
  NBseuil(im,128);
  if(strcmp(chaine, "\0") == 0){
    save_image(nomFichier,im,"P4");
  } else {
    strcpy(nomFichier,chaine);
    save_image(chaine,im,"P4");
  }
}

void SauvegarderPBM(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  char chaine[30];
  printf("Entrez un nouveau nom de fichier ou appuyez sur entrer pour conserver '%s'\nAttention dans ce dernier cas le contenu de '%s' sera écrasé : ",nomFichier,nomFichier);
  lire(chaine,30);
  NBseuil(im,128);
  if(strcmp(chaine, "\0") == 0){
    save_image(nomFichier,im,"P1");
  } else {
    strcpy(nomFichier,chaine);
    save_image(chaine,im,"P1");  
  }
}

void SauvegarderPGM_bin(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  char chaine[30];
  printf("Entrez un nouveau nom de fichier ou appuyez sur entrer pour conserver '%s'\nAttention dans ce dernier cas le contenu de '%s' sera écrasé : ",nomFichier,nomFichier);
  lire(chaine,30);
  grayscale(im);
  if(strcmp(chaine, "\0") == 0){
    save_image(nomFichier,im,"P5");
  } else {
    strcpy(nomFichier,chaine);
    save_image(chaine,im,"P5");
  }
}

void SauvegarderPGM(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  char chaine[30];
  printf("Entrez un nouveau nom de fichier ou appuyez sur entrer pour conserver '%s'\nAttention dans ce dernier cas le contenu de '%s' sera écrasé : ",nomFichier,nomFichier);
  lire(chaine,30);
  grayscale(im);
  if(strcmp(chaine, "\0") == 0){
    save_image(nomFichier,im,"P2");
  } else {
    strcpy(nomFichier,chaine);
    save_image(chaine,im,"P2");
  }
}

void SauvegarderPPM_bin(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  char chaine[30];
  printf("Entrez un nouveau nom de fichier ou appuyez sur entrer pour conserver '%s'\nAttention dans ce dernier cas le contenu de '%s' sera écrasé : ",nomFichier,nomFichier);
  lire(chaine,30);
  if(strcmp(chaine, "\0") == 0){
    save_image(nomFichier,im,"P6");
  } else {
    strcpy(nomFichier,chaine);
    save_image(chaine,im,"P6");
  }
}

void SauvegarderPPM(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  char chaine[30];
  printf("Entrez un nouveau nom de fichier ou appuyez sur entrer pour conserver '%s'\nAttention dans ce dernier cas le contenu de '%s' sera écrasé : ",nomFichier,nomFichier);
  lire(chaine,30);
  if(strcmp(chaine, "\0") == 0){
    save_image(nomFichier,im,"P3");
  } else {
    strcpy(nomFichier,chaine);
    save_image(chaine,im,"P3");
  }
}

//Les fonctions se terminant par "Opt" sont lancées par le système d'option
void SauvegardeOpt(const char* str){
  strcpy(nomFichier,str);
  char ext[10];
  trouverExtension(str,ext);
  if(strcmp(ext,".pbm") == 0){
    NBseuil(im,128);
    save_image(str,im,"P4");
  } else if(strcmp(ext,".pgm") == 0){
    grayscale(im);
    save_image(str,im,"P5");
  } else if(strcmp(ext,".ppm") == 0){
    save_image(str,im,"P6");
  } else {
    printf("L'extension du fichier n'est pas reconnue\n");
  }
}

void Ouvrir(){
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    char ch[30];
    char ext[10];
    do {
      printf("Entrez le nom du fichier avec son extention (pbm ,pgm ou ppm) : ");
      lire(ch,30);
      trouverExtension(ch,ext);
      printf("%s\n", ext);
    } while ((strcmp(ext,".pbm") != 0 && strcmp(ext,".pgm") != 0 && strcmp(ext,".ppm") != 0) || ext == NULL);
    SauvegardeOpt(ch);
    freeIm(im);
  }
  char chaine[30];
  printf("Entrez le nom du fichier que vous voulez ouvrir: ");
  lire(chaine,30);
  strcpy(nomFichier,chaine);
  im = copie(load_image(chaine));
  charge = true;
}

void OuvrirOpt(const char* chaine){
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPPM_bin();
    freeIm(im);
  }
  strcpy(nomFichier,chaine);
  im = copie(load_image(chaine));
  charge = true;
}

void Information(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  printf("Nom du fichier courant : %s\n", nomFichier);
  printf("Largeur : %d pixels\n", im->width);
  printf("Hauteur : %d pixels\n", im->heigth);
  channel_t chan = G;
  image* imCopie = copie(im);
  while(im->width > 80) resize(im,im->width/2,im->heigth);
  while(im->heigth > 60) resize(im,im->width,im->heigth/2);
  ascii_print(im,chan," .-+=*&#");
  im = copie(imCopie);
}

void QuitPBM() {
  cont = false;
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPBM();
  }
}

void QuitPBM_bin() {
  cont = false;
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPBM_bin();
  }
}

void QuitPGM() {
  cont = false;
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPGM();
  }
}

void QuitPGM_bin() {
  cont = false;
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPGM_bin();
  }
}

void QuitPPM() {
  cont = false;
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPPM();
  }
}

void QuitPPM_bin() {
  cont = false;
  if(charge){
    printf("Sauvegarde de l'image courante\n");
    SauvegarderPPM_bin();
  }
}

void Negatif(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = negatif(im);
  if(b == false) printf("L'opération a échoué");
}

void NiveauDeGris(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = grayscale(im);
  if(b == false) printf("L'opération a échoué");
}

void NoirBlancSeuil(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  unsigned int seuil = 0;
  int s;
  do{
    printf("Entrez un seuil compris entre 0 et 255 : ");
    scanf("%d", &seuil);
    s = (int) seuil;
    viderBuffer();
  } while(s < 1 || s > 255);
  bool b = NBseuil(im,seuil);
  if(b == false) printf("L'opération a échoué\n");
}

void NoirBlancSeuilOpt(unsigned int seuil){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = NBseuil(im,seuil);
  if(b == false) printf("L'opération a échoué\n");
}

void NoirBlancMoyenne(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = NBmoyenne(im);
  if(b == false) printf("L'opération a échoué\n");
}

void NoirBlancMediane(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = NBmediane(im);
  if(b == false) printf("L'opération a échoué\n");
}

void Pivog(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = pivog(im);
  if(b == false) printf("L'opération a échoué\n");
}

void Pivod(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = pivod(im);
  if(b == false) printf("L'opération a échoué\n");
}

void Hflip(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = hflip(im);
  if(b == false) printf("L'opération a échoué\n");
}

void Vflip(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = vflip(im);
  if(b == false) printf("L'opération a échoué\n");
}

void Resize(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  unsigned int w, h;
  int s;
  do{
    printf("Entrez la largeur de la nouvelle image : ");
    scanf("%d", &w);
    s = (int) w;
    viderBuffer();
  } while(s < 0);
  do{
    printf("Entrez la hauteur de la nouvelle image : ");
    scanf("%d", &h);
    s = (int) h;
    viderBuffer();
  } while(s < 0);
  bool b = resize(im,w,h);
  if(b == false) printf("Les nouvelles dimensions que vous avez choisi sont incorrectes\n");
}

void ResizeOpt(unsigned int w, unsigned int h){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = resize(im,w,h);
  if(b == false) printf("Les nouvelles dimensions que vous avez choisi sont incorrectes\n");
}

void Crop(){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  unsigned int i, j, w, h;
  int s;
  do{
    printf("Entrez la coordonée i du coin haut/gauche de la nouvelle image : ");
    scanf("%d", &i);
    s = (int) i;
    viderBuffer();
  } while(s < 0);
  do{
    printf("Entrez la coordonée j du coin haut/gauche de la nouvelle image : ");
    scanf("%d", &j);
    s = (int) j;
    viderBuffer();
  } while(s < 0);
  do{
    printf("Entrez la largeur de la nouvelle image : ");
    scanf("%d", &w);
    s = (int) w;
    viderBuffer();
  } while(s < 0);
  do{
    printf("Entrez la hauteur de la nouvelle image : ");
    scanf("%d", &h);
    s = (int) h;
    viderBuffer();
  } while(s < 0);
  bool b = crop(im,i,j,w,h);
  if(b == false) printf("Les nouvelles dimensions que vous avez choisi sont incorrectes\n");
}

void CropOpt(unsigned int i, unsigned int j, unsigned int w, unsigned int h){
  if(!charge){
    printf("Aucune image n'est chargée en mémoire\n");
    return;
  }
  bool b = crop(im,i,j,w,h);
  if(b == false) printf("Les nouvelles dimensions que vous avez choisi sont incorrectes\n");
}


int main(int argc, char** argv) {
  option_t* opt = NOOPTION;
  
  opt = opt_string(opt, "-i", OuvrirOpt);
  opt = opt_string(opt, "-open", OuvrirOpt);
  opt = opt_string(opt, "-load", OuvrirOpt);
  opt = opt_string(opt, "-o", SauvegardeOpt);
  opt = opt_string(opt, "-save", SauvegardeOpt);
  opt = opt_void(opt, "-p", Information);
  opt = opt_void(opt, "-print", Information);
  opt = opt_void(opt, "-n", Negatif);
  opt = opt_void(opt, "-negative", Negatif);
  opt = opt_void(opt, "-g", NiveauDeGris);
  opt = opt_void(opt, "-grayscale", NiveauDeGris);
  opt = opt_unsignedInt(opt, "-bw", NoirBlancSeuilOpt);
  opt = opt_unsignedInt(opt, "-black_white", NoirBlancSeuilOpt);
  opt = opt_void(opt, "-bwAvg", NoirBlancMoyenne);
  opt = opt_void(opt, "-bwMed", NoirBlancMediane);
  opt = opt_void(opt, "-RL", Pivog);
  opt = opt_void(opt, "-RR", Pivod);
  opt = opt_void(opt, "-RH", Hflip);
  opt = opt_void(opt, "-RV", Vflip);
  opt = opt_2unsignedInt(opt, "-r", ResizeOpt);
  opt = opt_2unsignedInt(opt, "-resize", ResizeOpt);
  opt = opt_4unsignedInt(opt, "-c", CropOpt);
  opt = opt_4unsignedInt(opt, "-crop", CropOpt);
 
  process_arguments(opt, argc, argv);

  free(opt);

  menu* m;
  menu* sm;
  menu* ssm;

  m = createMenu("Main menu");

  sm = createMenu("Fichier");
  addMenuAction(sm,"Ouvrir et sauvegarder",Ouvrir);
  ssm = createMenu("Sauvegarder");

  addMenuAction(ssm,"PBM binaire",SauvegarderPBM_bin);
  addMenuAction(ssm,"PBM ASCII",SauvegarderPBM);
  addMenuAction(ssm,"PGM binaire",SauvegarderPGM_bin);
  addMenuAction(ssm,"PGM ASCII",SauvegarderPGM);
  addMenuAction(ssm,"PPM binaire",SauvegarderPPM_bin);
  addMenuAction(ssm,"PPM ASCII",SauvegarderPPM);
  addSubMenu(sm,ssm);

  addMenuAction(sm,"Information",Information);

  ssm = createMenu("Quitter et sauvegarder (choisir l'extention)");
  addMenuAction(ssm,"PBM binaire", QuitPBM_bin);
  addMenuAction(ssm,"PBM ASCII", QuitPBM);
  addMenuAction(ssm,"PGM binaire", QuitPGM_bin);
  addMenuAction(ssm,"PGM ASCII", QuitPGM);
  addMenuAction(ssm,"PPM binaire", QuitPPM_bin);
  addMenuAction(ssm,"PPM ASCII", QuitPPM);
  addSubMenu(sm,ssm);
  addSubMenu(m,sm);

  sm = createMenu("Couleur");
  addMenuAction(sm,"Négatif",Negatif);
  addMenuAction(sm,"Niveau de gris",NiveauDeGris);
  ssm = createMenu("Noir et blanc");
  addMenuAction(ssm,"Seuil",NoirBlancSeuil);
  addMenuAction(ssm,"Moyenne",NoirBlancMoyenne);
  addMenuAction(ssm,"Médian",NoirBlancMediane);
  addSubMenu(sm,ssm);
  addSubMenu(m,sm);

  sm = createMenu("Transformation");
  addMenuAction(sm,"Pivoter à gauche de 90°",Pivog);
  addMenuAction(sm,"Pivoter à droite de 90°",Pivod);
  addMenuAction(sm,"Retourner horizontalement",Hflip);
  addMenuAction(sm,"Retourner verticalement",Vflip);
  addMenuAction(sm,"Redimensionner",Resize);
  addMenuAction(sm,"Découper",Crop);
  addSubMenu(m,sm);

  while(cont) launchMenu(m);

  deleteMenu(m);

  freeIm(im);

  return EXIT_SUCCESS;
}
