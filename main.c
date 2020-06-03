/*
 * extract MP3 tag frames
 * *
 * Author: Gilles MENEZ
 * Created: 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <arpa/inet.h>

#include "util.h"
#include "id3_tagheader.h"
#include "id3_frame.h"


/*=====================================================*/
/* Pour comprendre comment parcourir un fichier mp3.

   On rend la position en fin de lecture : normalement 
   on devrait etre en fin de tag.
*/

off_t mp3_read(int fd){
  int nb_lu;
  int i;

  /* Tag Header analysis ---------------------*/
  tag_header th; /* on va recuperer le header du tag 
		    car on en aura besoin pour parcourir les frames */
  nb_lu = id3_read_tagheader(fd, &th);    
  if (nb_lu != 10) return nb_lu;

  /* Tag frames analysis ---------------------*/
  do {
    nb_lu = id3_read_frame(fd, th.tailletag);
  } while ( nb_lu > 0);

  /* Pour verifier ou on est a la fin de la lecture du tag ? -*/
  off_t fintag = lseek(fd, 0, SEEK_CUR);

#if DEBUG
  printf("\nFIN TAG : %d\n", fintag);
#endif
  
  return fintag;
}

/*=====================================================*/

int main(int argc, char *argv[]){
 FILE *f;
  char *filename;
  int nb_lu;
  int i;

  /* Verification de l'appel du programme ----*/
  if (argc != 2){ /* usage -- print usage message and exit */
    fprintf(stderr, "Usage: %s mp3file\n", argv[0]);
    exit(1);
  }
  else
    filename = argv[1];

  /* Ouverture du fichier --------------------*/
  if (! (f = fopen(filename, "r"))){
    perror(filename);
    exit(1);
  }
  
  int fd = fileno(f); /* Get descriptor from FILE * */
  
  /* 1) Parcours d'un fichier mp3 */
  mp3_read(fd);

  printf("\n");
  sync();

  return 0;
}

/*=====================================================*/
