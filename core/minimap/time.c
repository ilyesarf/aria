#include "time.h"

void Timer(int *tempsdebut)
{
    
   if( SDL_GetTicks() - *tempsdebut >= 1000 )
    {
        *tempsdebut = SDL_GetTicks() ;
      
      
    }
  
}

void inittemps(Time *t)
{   int test; 
	t->tempsdebut=SDL_GetTicks();
	t->mm=0;
	t->ss=0;
	test=initTexttime(&t->temps);
}
void inittempsenig(Time *t)
{   int test; 
	t->tempsdebut=16000;
	t->mm=0;
	t->ss=0;
	test=initTexttimeenig(&t->temps);
}
int initTexttime(Text* T)
{ int testload;
    T->couleurTxt.r = 0; 
    T->couleurTxt.g = 0; 
    T->couleurTxt.b = 0;

    strcpy(T->txt,"");
    T->positionText.x = 850;
    T->positionText.y = 20; 
    testload=loadFonttime(T,"angelina.TTF");
    T->textSurface=NULL;
    return testload;   
}
int initTexttimeenig(Text* T)
{ int testload;
    T->couleurTxt.r = 255; 
    T->couleurTxt.g = 255; 
    T->couleurTxt.b = 255;

    strcpy(T->txt, "");
    T->positionText.x = 0;
    T->positionText.y = 0; 
    testload=loadFonttime(T,"angelina.TTF");
    T->textSurface=NULL;
    return testload;   
}
int loadFonttime(Text* T, char* path)
{
    if(TTF_Init() == -1) {
        printf("Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        return -1;
    }
    T->police= TTF_OpenFont(path,60);//pathn hoa esm l font o 60 l taile 
    if (T->police == NULL) {
        printf("Unable to load Font: %s\n", SDL_GetError());
        return (-1);
    }
    return (0);
}

void update_time(Time* T)
{   int ts;
    Timer(&T->tempsdebut);
    ts=T->tempsdebut/1000;
    T->mm=ts/60;
    T->ss=ts % 60;
    if(T->mm<10&&T->ss<10)
       sprintf(T->temps.txt,"*** time :0%d:0%d  ***",T->mm,T->ss);
    else if(T->mm<10&&T->ss>10)
        sprintf(T->temps.txt,"*** time :0%d:%d  ***",T->mm,T->ss);
    else if(T->mm>10&&T->ss<10)
          sprintf(T->temps.txt,"*** time :%d:0%d  ***",T->mm,T->ss);
  
    T->temps.textSurface=TTF_RenderText_Solid(T->temps.police,T->temps.txt,T->temps.couleurTxt);//ttfrender tekho l police o txt o couleur taa l text eli besh taficchiih
}
void update_timeenig(Time* T)
{   int ts;
    
    Timer(&T->tempsdebut);
    ts=T->tempsdebut/1000;
    T->mm=ts/ 60;
    T->ss=ts % 60;
    if(T->mm<10&&T->ss<10)
       sprintf(T->temps.txt,"*** time :0%d:0%d  ***",T->mm,T->ss);
    else if(T->mm<10&&T->ss>10)
        sprintf(T->temps.txt,"*** time :0%d:%d  ***",T->mm,T->ss);
    else if(T->mm>10&&T->ss<10)
          sprintf(T->temps.txt,"*** time :%d:0%d  ***",T->mm,T->ss);
    /*else
         sprintf(T->temps.txt,"*** time :%d:%d  ***",T->mm,T->ss); */
    T->temps.textSurface=TTF_RenderText_Solid(T->temps.police,T->temps.txt,T->temps.couleurTxt);//ttfrender tekho l police o txt o couleur taa l text eli besh taficchiih
}
void displaytime(Time T,SDL_Surface *screen)
{
    
     
     SDL_BlitSurface(T.temps.textSurface,NULL,screen,&(T.temps.positionText));

}

void freeTexttime(Text T)
{
    TTF_CloseFont(T.police); 
    TTF_Quit();    
}

