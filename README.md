# README #

### Les membres du groupes ###

* LARANJEIRO Frederic " U21303792 "

* ZAGHBANE Safiy Errahmane " U21515927 "

### Les options utilisées? ###

* -h ou -help : pour voir toutes les options que vous pouvez utiliser.

### Un compte rendu ###

* Toutes les fonctions demandées sur le projet sont faites et fonctionnent correctement.
 
* Notre programe fonctionne seulement avec les fichiers dont la précision est 8, 16 ou 24 bits.

* D'apres les recherches qu'on a realisées sur les fichiers wave, on a découvert que les fichiers dont la précision est de 16,24 ou 32 bits, l'amplitude est stockée comme un float
par conséquent la fonction wave_get ne va jamais retourner la vraie amplitude car elle la coupe quand elle la retourne comme un int64_t, donc il subsite un bruit quand vous essayez de modifer un fichier de 16 bits ou 24 bits.

* On a découvert aprés avoir testé plusieurs fois notre programme, que les fichier wave ne fonctionnera pas s'il y a que 3 cannaux.

* On a testé notre programme plusieurs fois, il risque d'y avoir des bugs que nous avons malheuresement pas decelés.

### la deuxieme version du projet ###

* On a realisé une deuxieme version du projet en y incorporant cette fois-ci une animation lors du lancement de l'application et un menu graphique (utilisant <ncurses.h>), mais les fonctions sont identiques dans les deux versions du projet.