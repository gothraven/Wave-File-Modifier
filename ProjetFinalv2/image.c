

#include "image.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

void ascii_print(image* im, channel_t cl, const char* ct){
  unsigned int i, j;
  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      switch (cl){
	case R: printf("%c", ct[(strlen(ct) * PIX(im,j,i).R) / 256]); break;
	case G: printf("%c", ct[(strlen(ct) * PIX(im,j,i).G) / 256]); break;
	case B: printf("%c", ct[(strlen(ct) * PIX(im,j,i).B) / 256]); break;
	case A: printf("%c", ct[(strlen(ct) * PIX(im,j,i).A) / 256]); break;
      }
    }
    printf("\n");
  }
}

void freeIm(image* im){
  if(im == NULL) return;
  free(im->img);
  free(im);
}

//viderLigne permet de verifier la présence ou non d'un "#" indiquant
//un commentaire, afin de ne pas en tenir compte.
void viderLigne(FILE* fdesc){
  char vide[101];
  unsigned char c;
  while((c = fgetc(fdesc)) == '#') {
    fgets(vide,100,fdesc);
  }
  fseek(fdesc,-1,SEEK_CUR);
}

void load_image_PBM(image* im, FILE* fdesc){
  char chaine[3];
  unsigned int cptL = 0;
  unsigned int cptH = 0;
  unsigned char c;

  while(fscanf(fdesc, "%s", chaine) > 0){
    c = (unsigned char) atoi(chaine);
    if(c == 1){
        PIX(im,cptH,cptL).R = 0;
        PIX(im,cptH,cptL).G = 0;
        PIX(im,cptH,cptL).B = 0;
    }
    if(c == 0){
        PIX(im,cptH,cptL).R = 255;
        PIX(im,cptH,cptL).G = 255;
        PIX(im,cptH,cptL).B = 255;
    }
    cptH++;
    if(cptH == im->width){
        cptH = 0;
        cptL++;
    }
  }
}

void load_image_PBM_bin(image* im, FILE* fdesc){

  unsigned char c;

  unsigned int cptL = 0;
  unsigned int cptH = 0;
  int cpt = 7;

  bool b = false;

  unsigned int larg = im->width /8;

  unsigned int cptLarg = 0;

  unsigned char reste;
  int nbReste = 0;

  if((im->width)%8 == 0)reste =0;
  else reste = 8 - ((im->width)%8);

  unsigned int dec = 0;
  unsigned char d = 0;

  fseek(fdesc,1,SEEK_CUR);

  while(!feof(fdesc) && cptH < im->width && cptL < im->heigth){
        fread(&c, sizeof(char), 1, fdesc);

        if(b == true){
          nbReste = 0;
          b = false;
        } 

        while(cpt >= nbReste){
           d = c;
           dec = ((d >> cpt) & 1) * 255;
           PIX(im,cptH,cptL).R = 255 - dec;
           PIX(im,cptH,cptL).G = 255 - dec;
           PIX(im,cptH,cptL).B = 255 - dec;
           cpt--;
           
           cptH++;

           if(cptH == im->width){
             cptH = 0;
             cptL++;
           }
        }

        cpt = 7;

        cptLarg++;
        if(cptLarg == larg){
          cptLarg = 0;
          nbReste = reste;
          b = true;        
        } 
  }
}

void load_image_PGM_bin(image* im, FILE* fdesc){
  
  char intensite[10];
  unsigned char c;
  
  fscanf(fdesc, "%s",intensite);

  viderLigne(fdesc);
  
  unsigned int cptL = 0;
  unsigned int cptH = 0;

  int inten = atoi(intensite);

  fseek(fdesc,1,SEEK_CUR);
  
  if(inten < 256){
     while(!feof(fdesc) && cptH < im->width && cptL < im->heigth){
         fread(&c, sizeof(char), 1, fdesc);
         
         int nbs = c;
	
         PIX(im,cptH,cptL).R = nbs;
         PIX(im,cptH,cptL).G = nbs;
         PIX(im,cptH,cptL).B = nbs;
         cptH++;
         if(cptH == im->width){
             cptH = 0;
             cptL++; 
         }
     }
  }
}

void load_image_PPM_bin(image* im, FILE* fdesc){
  
  char intensite[10];
  unsigned char c;
  
  fscanf(fdesc, "%s",intensite );

  viderLigne(fdesc);

  unsigned int cptL = 0;
  unsigned int cptH = 0;
  int cpt = 0;

  int inten = atoi(intensite);

  fseek(fdesc,1,SEEK_CUR);
  
  if(inten < 256){
     while(!feof(fdesc) && cptH < im->width && cptL < im->heigth){
	       fread(&c, sizeof(char), 1, fdesc);
	 
         int nbs = c;

         if(cpt == 0)PIX(im,cptH,cptL).R = nbs;
         if(cpt == 1)PIX(im,cptH,cptL).G = nbs;
         if(cpt == 2){
            PIX(im,cptH,cptL).B = nbs;
            cptH++;
         }
         if(cptH == im->width){
             cptH = 0;
             cptL++;
         }
         cpt++;
         if(cpt == 3) cpt = 0;
     }
  }
}

void load_image_PGM(image* im, FILE* fdesc){
  char intensite[10];
  fscanf(fdesc,"%s", intensite);

  viderLigne(fdesc);

  char c[3];

  unsigned int cptL = 0;
  unsigned int cptH = 0;
  unsigned char nbs;

  //int inten = atoi(intensite);

  while(fscanf(fdesc,"%s", c ) > 0){

    nbs = (unsigned char) atoi(c);

    PIX(im,cptH,cptL).R = nbs;
    PIX(im,cptH,cptL).G = nbs;
    PIX(im,cptH,cptL).B = nbs;
    cptH++;
    if(cptH == im->width){
        cptH = 0;
        cptL++;
    }
  }
}



void load_image_PPM(image* im, FILE* fdesc){
  char intensite[10];
  fscanf(fdesc,"%s",intensite);

  viderLigne(fdesc);

  char c[3];

  unsigned int cptL = 0;
  unsigned int cptH = 0;
  unsigned int cpt = 0;

  while(fscanf(fdesc, "%s", c) > 0){

    int nbs = atoi(c);

    if(cpt == 0) PIX(im,cptH,cptL).R = nbs;
        
    if(cpt == 1) PIX(im,cptH,cptL).G = nbs;
       
    if(cpt == 2){
        PIX(im,cptH,cptL).B = nbs;
        cptH++;
    }

    cpt++;
    if(cpt == 3) cpt = 0;

    if(cptH == im->width){
        cptH = 0;
        cptL++;
    }
  }
}



image* load_image(const char* fname){
  FILE* fdesc = fopen(fname , "r");
  if(fdesc == NULL){
    fprintf(stderr, "Impossible d'ouvrir le fichier\n");
    exit(EXIT_FAILURE);
  }

  char c, v;

  fread(&c, sizeof(char), 1 , fdesc);
  fread(&c, sizeof(char), 1 , fdesc);

  fread(&v, sizeof(char), 1 , fdesc);
  
  viderLigne(fdesc);

  unsigned int larg;
  unsigned int haut;

  fscanf(fdesc, "%d" ,&larg );
  
  fscanf(fdesc, "%d" , &haut);

  
  viderLigne(fdesc);
  

  image* im = (image*) malloc(sizeof(image));
  im->width = larg;
  im->heigth = haut;

  pixel* pix = (pixel*) malloc(larg * haut * sizeof(pixel));
  

  im->img = pix;

  //En fontion du format d'image, la focntion load_image va appeler
  //Une sous fonction qui va traiter l'image avec le bon format

  switch (c){
    case '1': load_image_PBM(im,fdesc);break;
    case '2': load_image_PGM(im,fdesc);break;
    case '3': load_image_PPM(im,fdesc);break;
    case '4': load_image_PBM_bin(im,fdesc);break;
    case '5': load_image_PGM_bin(im,fdesc);break;
    case '6': load_image_PPM_bin(im,fdesc);break;
    default: return NULL;
  }
  
  fclose(fdesc);

  return im;
}

bool save_image_PBM(image* im, FILE* fdesc){
  unsigned i, j;
  for(i = 0; i < im->heigth ; i++){
    for(j = 0; j < im->width; j++){
      if(PIX(im,j,i).R < 128){
        fprintf(fdesc, "1 ");
      } else {
        fprintf(fdesc, "0 ");
      }
    }
    fprintf(fdesc, "\n");
  }

  return true;
}

bool save_image_PBM_bin(image* im, FILE* fdesc){
  unsigned int i, j;

  unsigned int nbs = 0;
  int nbReste = 0;

  int cpt = 7;

  for(i = 0; i < im->heigth ; i++){
    for(j = 0; j < im->width; j++){
    
        if(PIX(im,j,i).R < 128){
          nbs += pow(2,cpt);
        }
        cpt--;
        if(cpt == nbReste - 1){
          cpt = 7;
          char c = (char) nbs;
          fwrite(&c,sizeof(char),1,fdesc);
          nbs = 0;
        } 
    }
  }

  return true;
}


bool save_image_PGM(image* im, FILE* fdesc){
  unsigned int i, j;

  fprintf(fdesc, "255\n");

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      int moy = (PIX(im,j,i).R + PIX(im,j,i).G + PIX(im,j,i).B)/3;
      fprintf(fdesc, "%d ", moy);
    }
    fprintf(fdesc, "\n");
  }

  return true;
}

bool save_image_PGM_bin(image* im, FILE* fdesc){
  fprintf(fdesc, "255\n");

  unsigned int i, j;

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      int moy = (PIX(im,j,i).R + PIX(im,j,i).G + PIX(im,j,i).B)/3;
      unsigned char b = (unsigned char) moy;
      fwrite(&b,sizeof(unsigned char),1,fdesc);
    }
  }

  return true;
}

bool save_image_PPM(image* im, FILE* fdesc){
  unsigned int i, j;

  fprintf(fdesc, "255\n");

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      fprintf(fdesc, "%d ", PIX(im,j,i).R);
      fprintf(fdesc, "%d ", PIX(im,j,i).G);
      fprintf(fdesc, "%d ", PIX(im,j,i).B);
    }
    fprintf(fdesc, "\n");
  }

  return true;
}

bool save_image_PPM_bin(image* im, FILE* fdesc){
  unsigned int i, j;
  int nb;
  unsigned char b;

  fprintf(fdesc, "255\n");

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      nb = PIX(im,j,i).R;
      b = (unsigned char) nb;
      fwrite(&b,sizeof(unsigned char),1, fdesc);
      nb = PIX(im,j,i).G;
      b = (unsigned char) nb;
      fwrite(&b,sizeof(unsigned char),1, fdesc);
      nb = PIX(im,j,i).B;
      b = (unsigned char) nb;
      fwrite(&b,sizeof(unsigned char),1, fdesc);
    }
  }

  return true;
}


bool save_image(const char* fname, image* im, const char* magic_number){
  FILE* fdesc = fopen(fname, "w");
  if(fdesc == NULL){
    fprintf(stderr, "Impossible de sauvegarder le fichier\n");
    exit(EXIT_FAILURE);
  }

  fprintf(fdesc,"%s\n", magic_number);

  fprintf(fdesc, "%d %d\n", im->width, im->heigth);

  bool b;

  switch(magic_number[1]){
    case '1' : b = save_image_PBM(im,fdesc);break;
    case '2' : b = save_image_PGM(im,fdesc);break;
    case '3' : b = save_image_PPM(im,fdesc);break;
    case '4' : b = save_image_PBM_bin(im,fdesc);break;
    case '5' : b = save_image_PGM_bin(im,fdesc);break;
    case '6' : b = save_image_PPM_bin(im,fdesc);break;
    default: return false;
  }

  fclose(fdesc);

  return b;

}


//Permet de réaliser une copie strictement identique de l'image en paramètre.
image* copie(image* im){
  image* imCopie = (image*) malloc(sizeof(image));
  unsigned int i, j;

  imCopie->width = im->width;
  imCopie->heigth = im->heigth;

  pixel* pix = (pixel*) malloc(im->width * im->heigth * sizeof(pixel));

  imCopie->img = pix;

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      PIX(imCopie,j,i).R = PIX(im,j,i).R;
      PIX(imCopie,j,i).G = PIX(im,j,i).G;
      PIX(imCopie,j,i).B = PIX(im,j,i).B;
      PIX(imCopie,j,i).A = PIX(im,j,i).A;
    }
  }

  return imCopie;
}

bool grayscale(image* im){
  unsigned int i, j;
  float lum;
  for(i = 0; i < im->width; i++){
    for(j = 0; j < im->heigth; j++){
      lum = ((float)  PIX(im,j,i).R) * 0.2126 + 
	    ((float)  PIX(im,j,i).G) * 0.7152 +
	    ((float)  PIX(im,j,i).B) * 0.0722;
      PIX(im,j,i).R = (unsigned int) lum;
      PIX(im,j,i).G = (unsigned int) lum;
      PIX(im,j,i).B = (unsigned int) lum;
    }
  }
  return true;
}

bool NBseuil(image* im, unsigned int seuil){

  unsigned int i , j;
  grayscale(im);
  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      if(PIX(im,j,i).R > seuil){
         
	       PIX(im,j,i).R = 255;
	       PIX(im,j,i).G = 255;
	       PIX(im,j,i).B = 255;
	       PIX(im,j,i).A = 255;
      } else {
	       PIX(im,j,i).R = 0;
	       PIX(im,j,i).G = 0;
	       PIX(im,j,i).B = 0;
	       PIX(im,j,i).A = 0;
      }
    }
  }
  return true;
}

bool NBmoyenne(image* im){
  unsigned long somme = 0;
  unsigned int i, j;
  grayscale(im);
  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      somme += (unsigned long) PIX(im,j,i).R;
    }
  }
  unsigned int moy = ((unsigned int) somme)/(im->heigth * im->width);
  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      if(PIX(im,j,i).R > moy){
         PIX(im,j,i).R = 255;
         PIX(im,j,i).G = 255;
         PIX(im,j,i).B = 255;
         PIX(im,j,i).A = 255;
      } else {
         PIX(im,j,i).R = 0;
         PIX(im,j,i).G = 0;
         PIX(im,j,i).B = 0;
         PIX(im,j,i).A = 0;
      }
    }
  }
  return true;
}

//Compare est utilisée uniquement dans la fonction NBmediane
//Il aurai été possible de la déclarer "static" mais cela amène a 
//un warning lors de la compilation
int compare(void const *a , void const *b){
  int const *pa = a;
  int const *pb = b;
  return *pa - *pb;
}

/*La fonction NBmediane calcul tout d'abord la médiane des niveau de gris. 
  Pour cela un tableau contenant l'ensemble des niveaux de gris est crée
  Il est ensuite trié par la fonction qsort (fonction des bibliothèques 
  standarts). On récupère enfin l'élement central du tableau qui est la médiane*/
bool NBmediane(image* im){
  grayscale(im);
  unsigned int* tab = malloc(im->width * im->heigth * sizeof(unsigned int));
  unsigned int cpt = 0;
  unsigned int i, j;

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      tab[cpt] = PIX(im,j,i).R;
      cpt++;
    }
  }

  qsort(tab,sizeof(tab) / sizeof(*tab),sizeof(*tab),compare);
  
  unsigned int med = tab[(im->width * im->heigth) /2];

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      if(PIX(im,j,i).R > med){
         PIX(im,j,i).R = 255;
         PIX(im,j,i).G = 255;
         PIX(im,j,i).B = 255;
         PIX(im,j,i).A = 255;
      } else {
         PIX(im,j,i).R = 0;
         PIX(im,j,i).G = 0;
         PIX(im,j,i).B = 0;
         PIX(im,j,i).A = 0;
      }
    }
  }

  free(tab);
  
  return true;
}

bool hflip(image* im){
  unsigned int i, j;

  image* imCopie = copie(im);

  unsigned int cptLargeur = im->width - 1;

  for(i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      PIX(im,j,i).R = PIX(imCopie,cptLargeur,i).R;
      PIX(im,j,i).G = PIX(imCopie,cptLargeur,i).G;
      PIX(im,j,i).B = PIX(imCopie,cptLargeur,i).B;
      PIX(im,j,i).A = PIX(imCopie,cptLargeur,i).A;
      cptLargeur--;
    }
    cptLargeur = im->width - 1;
  }

  freeIm(imCopie);

  return true;
}

bool vflip(image* im){
  unsigned int i, j;

  image* imCopie = copie(im);

  unsigned int cptHauteur = im->heigth - 1;

  for(j = 0; j < im->width; j++){
    for(i = 0; i < im->heigth; i++){
      PIX(im,j,i).R = PIX(imCopie,j,cptHauteur).R;
      PIX(im,j,i).G = PIX(imCopie,j,cptHauteur).G;
      PIX(im,j,i).B = PIX(imCopie,j,cptHauteur).B;
      PIX(im,j,i).A = PIX(imCopie,j,cptHauteur).A;
      cptHauteur--;
    }
    cptHauteur = im->heigth - 1;
  }

  freeIm(imCopie);

  return true;
}

bool pivod(image* im){
  unsigned int i, j;

  image* imPivot = (image*) malloc(sizeof(image));

  imPivot->width = im->heigth;
  imPivot->heigth = im->width;

  pixel* pix = (pixel*) malloc(im->width * im->heigth * sizeof(pixel));

  imPivot->img = pix;

  int cptH = (int) im->heigth - 1;
  unsigned int cptL = 0;

  for(i = 0; i < imPivot->heigth; i++){
    for(j = 0; j < imPivot->width; j++){
      PIX(imPivot,j,i).R = PIX(im,cptL,cptH).R;
      PIX(imPivot,j,i).G = PIX(im,cptL,cptH).G;
      PIX(imPivot,j,i).B = PIX(im,cptL,cptH).B;
      PIX(imPivot,j,i).A = PIX(im,cptL,cptH).A;
      cptH--; 
      if(cptH == -1) cptH = (int) im->heigth - 1;
    }
    cptL++;
  }

  *im = *imPivot;

  return true;
}

bool pivog(image* im){
  unsigned int i, j;

  image* imPivot = (image*) malloc(sizeof(image));

  imPivot->width = im->heigth;
  imPivot->heigth = im->width;

  pixel* pix = (pixel*) malloc(im->width * im->heigth * sizeof(pixel));

  imPivot->img = pix;

  unsigned int cptH = 0;
  unsigned int cptL = im->width - 1;

  for(i = 0; i < imPivot->heigth; i++){
    for(j = 0; j < imPivot->width; j++){
      PIX(imPivot,j,i).R = PIX(im,cptL,cptH).R;
      PIX(imPivot,j,i).G = PIX(im,cptL,cptH).G;
      PIX(imPivot,j,i).B = PIX(im,cptL,cptH).B;
      PIX(imPivot,j,i).A = PIX(im,cptL,cptH).A;
      cptH++;
      if(cptH == im->heigth) cptH = 0;
    }
    cptL--;
  }

  *im = *imPivot;

  return true;
}


bool crop(image* im, unsigned int i, unsigned int j, unsigned int w, unsigned int h){

  if(i + w > im->width || j + h > im->heigth){
    return false;
  }

  unsigned int k, l;

  image* imCrop = (image*) malloc(sizeof(image));

  imCrop->width = w;
  imCrop->heigth = h;

  pixel* pix = (pixel*) malloc(w * h * sizeof(pixel));
  
  imCrop->img = pix;

  unsigned int cptL = i;
  unsigned int cptH = j;

  for(k = 0; k < h; k++){
    for(l = 0; l < w; l++){
      PIX(imCrop,l,k).R = PIX(im,cptH,cptL).R;
      PIX(imCrop,l,k).G = PIX(im,cptH,cptL).G;
      PIX(imCrop,l,k).B = PIX(im,cptH,cptL).B;
      PIX(imCrop,l,k).A = PIX(im,cptH,cptL).A;
      cptH++;
      if(cptH == j + w) cptH = j;
    }
    cptL++;
  }

  *im = *imCrop;

  return true;
}

image* resizeWgrand(image* im, unsigned int w, unsigned int h, unsigned int nW){
  unsigned int i, j;
  image* imRes = (image*) malloc(sizeof(image));

  imRes->width = w;
  imRes->heigth = h;

  pixel* pix = (pixel*) malloc(h * w * sizeof(pixel));

  imRes->img = pix;

  unsigned int cptnW = 0;
  unsigned int cptW = 0;
  unsigned int cptH = 0;
  for(i = 0; i < imRes->heigth; i++){
   for(j = 0; j < imRes->width; j++){
     PIX(imRes,j,i).R = PIX(im,cptH,cptW).R;
     PIX(imRes,j,i).G = PIX(im,cptH,cptW).G;
     PIX(imRes,j,i).B = PIX(im,cptH,cptW).B;
     PIX(imRes,j,i).A = PIX(im,cptH,cptW).A;
     cptnW++;
     if(cptnW == nW ){
       cptH++;
       cptnW = 0;
     } 
     if(cptH == im->heigth) cptH = 0;
   }
   cptW++;
  }
  
  return imRes;
}

image* resizeHgrand(image* im, unsigned int w, unsigned int h, unsigned int nH){
  unsigned int i, j;
  
  image* imRes = (image*) malloc(sizeof(image));

  imRes->width = w;
  imRes->heigth = h;

  pixel* pix = (pixel*) malloc(h * w * sizeof(pixel));

  imRes->img = pix;

  unsigned int cptnH = 0;
  unsigned int cptW = 0;
  unsigned int cptH = 0;
  
  for(i = 0; i < imRes->heigth; i++){
     for(j = 0; j < imRes->width; j++){
	      PIX(imRes,j,i).R = PIX(im,cptH,cptW).R;
        PIX(imRes,j,i).G = PIX(im,cptH,cptW).G;
        PIX(imRes,j,i).B = PIX(im,cptH,cptW).B;
        PIX(imRes,j,i).A = PIX(im,cptH,cptW).A;
	      cptH++;
	      if(cptH == im->width) cptH = 0;
      }
      cptnH++;
     if(cptnH == nH){
       cptW++;
       cptnH = 0;
    }
  }
  
  return imRes;
}

image* resizeWpetit(image* im, unsigned int w, unsigned int h, unsigned int nW){
  unsigned int i, j;

  image* imRes = (image*) malloc(sizeof(image));

  imRes->width = w;
  imRes->heigth = h;

  pixel* pix = (pixel*) malloc(h * w * sizeof(pixel));

  imRes->img = pix;

  unsigned int cptH = 0;
  unsigned int cptW = 0;
  unsigned int cpt = 0;
  unsigned long somme = 0;
  unsigned int moy;

  for(i = 0; i < imRes->heigth; i++){
    for(j = 0; j < imRes->width; j++){

       cpt = 0; somme = 0;
       while(cpt < nW){
          somme += PIX(im,cptH + cpt,cptW).R;
          cpt++;
       }
       moy = somme/nW;
       PIX(imRes,j,i).R = moy;

       cpt = 0; somme = 0;
       while(cpt < nW){
          somme += PIX(im,cptH + cpt,cptW).G;
          cpt++;
       }
       moy = somme/nW;
       PIX(imRes,j,i).G = moy;

       cpt = 0; somme = 0;
       while(cpt < nW){
          somme += PIX(im,cptH + cpt,cptW).B;
          cpt++;
       }
       moy = somme/nW;
       PIX(imRes,j,i).B = moy;

       cpt = 0; somme = 0;
       while(cpt < nW){
          somme += PIX(im,cptH + cpt,cptW).A;
          cpt++;
       }
       moy = somme/nW;
       PIX(imRes,j,i).A = moy;

       cptH += nW;
       if(cptH == im->width) cptH = 0;
    }
    cptW++;
  }

  return imRes;
}

image* resizeHpetit(image* im, unsigned int w, unsigned int h, unsigned int nH){
  unsigned int i, j;

  image* imRes = (image*) malloc(sizeof(image));

  imRes->width = w;
  imRes->heigth = h;

  pixel* pix = (pixel*) malloc(h * w * sizeof(pixel));

  imRes->img = pix;

  unsigned int cptH = 0;
  unsigned int cptW = 0;
  unsigned int cpt = 0;
  unsigned long somme = 0;
  unsigned int moy;

  for(i = 0; i < imRes->heigth; i++){
    for(j = 0; j < imRes->width; j++){

       cpt = 0; somme = 0;
       while(cpt < nH){
          somme += PIX(im,cptH,cptW + cpt).R;
          cpt++;
       }
       moy = somme/nH;
       PIX(imRes,j,i).R = moy;

       cpt = 0; somme = 0;
       while(cpt < nH){
          somme += PIX(im,cptH,cptW + cpt).G;
          cpt++;
       }
       moy = somme/nH;
       PIX(imRes,j,i).G = moy;

       cpt = 0; somme = 0;
       while(cpt < nH){
          somme += PIX(im,cptH,cptW + cpt).B;
          cpt++;
       }
       moy = somme/nH;
       PIX(imRes,j,i).B = moy;

       cpt = 0; somme = 0;
       while(cpt < nH){
          somme += PIX(im,cptH,cptW + cpt).A;
          cpt++;
       }
       moy = somme/nH;
       PIX(imRes,j,i).A = moy;

       cptH++;
       if(cptH == im->width) cptH = 0;
    }
    cptW += nH;
  }

  return imRes;
}

bool resize(image* im, unsigned int w, unsigned int h){
  bool resWpetit = false; //Ces booléen sont mis a vrai si l'opération est a réaliser
  bool resWgrand = false; //par exemple resWpetit indique une redimention de la largeur
  bool resHpetit = false; //par un diviseur de la alrgeur d'origine
  bool resHgrand = false;

  unsigned int nH , nW;

  if(im->width % w != 0 && w % im->width != 0){
    return false;
  } else {
    if(im->width % w == 0 ){
      resWpetit = true;
      nW = im->width/w;
    } 
    else{
       resWgrand = true;
       nW = w/im->width;
    }
  }
  if(im->heigth % h != 0 && h % im->heigth != 0){
    return false;
  } else {
    if(im->heigth % h == 0){
      resHpetit = true;
      nH = im->heigth/h;
    } else {
      resHgrand = true;
      nH = h/im->heigth;
    } 
  }

  //On applique d'abord ou non la redimention de la largeur puis, 
  //on s'occupe de la hauteur si cela est nécessaire.
  //Cela se fait via l'appel de fonctions secondaires.

  if(resWgrand){
    *im = *(resizeWgrand(im,w,im->heigth,nW));
    if(resHgrand){
      *im = *(resizeHgrand(im,w,h,nH));
    }
    if(resHpetit){
      *im = *(resizeHpetit(im,w,h,nH));
    }
  }

  if(resWpetit){
    *im = *(resizeWpetit(im,w,im->heigth,nW));
     if(resHgrand){
      *im = *(resizeHgrand(im,w,h,nH));
    }
    if(resHpetit){
      *im = *(resizeHpetit(im,w,h,nH));
    }
  }
  
  return true;
}

bool negatif(image* im){
  unsigned int i, j;
  for( i = 0; i < im->heigth; i++){
    for(j = 0; j < im->width; j++){
      PIX(im,j,i).R = 255 - PIX(im,j,i).R;
      PIX(im,j,i).G = 255 - PIX(im,j,i).G;
      PIX(im,j,i).B = 255 - PIX(im,j,i).B;
      PIX(im,j,i).A = 255 - PIX(im,j,i).A;
    }
  }
  return true;
}






  
  




