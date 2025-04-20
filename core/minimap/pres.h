#ifndef PERS_H_INCLUDED
#define PERS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>



typedef struct
{
  	SDL_Surface *image_p;
  	SDL_Rect pos_perso;
}personne;

void initialiserperso(personne *p);
void deplacer_perso(personne *p,SDL_Event event);
void afficher_perso(SDL_Surface *screen,personne p);
void free_perso(personne p);


#endif
