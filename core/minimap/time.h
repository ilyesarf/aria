#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string.h>



typedef struct  {
    SDL_Surface* textSurface;
    SDL_Rect positionText;
    char txt[20];
    SDL_Color couleurTxt;
    TTF_Font* police;
}Text;// structure text besh tekteb l temps 


typedef struct
{
    int tempsdebut;
    int mm; 
    int ss;
    Text temps;
    
} Time;



void inittemps(Time *t);// initialisaton lel temps l koulo 
void inittempsenig(Time *t);
void Timer(int *tempsdebut);// tehseb l temps 
int initTexttime(Text* T);//initialisation du text  khw eli besh nektbo bih
int initTexttimeenig(Text* T);
int loadFonttime(Text* T, char* angelina);//tloadi l font mtaa l text
void update_time(Time* T);//mise a jour du temps 
void update_timeenig(Time* T);
void displaytime(Time T,SDL_Surface *screen);//afficher le temps aal back
void freeTexttime(Text T);//besh ykoun fema optimisation 


#endif
