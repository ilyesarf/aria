#include<stdio.h>
#include"SDL/SDL.h"
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "time.h"
#include "map.h"
#include <string.h>

int main()
{	
		int continuer=1,done=1;
	SDL_Surface *screen,*backg,*backgm;
	SDL_Rect backg_pos,backgm_pos;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	screen=SDL_SetVideoMode(1500,1000,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(screen==NULL)
	{
		printf("unable to set video mode:%s\n",SDL_GetError());
		return 1;
	}
	
	backg = SDL_LoadBMP("backgg1.bmp");
	backg_pos.x = 0;
	backg_pos.y = 0;
	
//initialisation du temps et de map et du perso 

	Time temps;
	map m;
	personne p;
	
	inittemps(&temps);
	initialiser_map (&m,screen,&p) ;
	initialiserperso(&p);
	
	SDL_Event event;
	
	while (continuer)
	{	
	SDL_PollEvent(&event);
	switch(event.type)
        {

        case SDL_QUIT:
            continuer=0;
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
            case SDLK_k:{
		while(done){
		
		}}
                break ;
	case SDLK_c:
		
                break ;
	
	    
	}}	
		
		update_time(&temps);
		deplacer_perso(&p,event);

		perso_map(&m,&p);
		SDL_BlitSurface(backg,NULL,screen,&backg_pos);
		
		displaytime(temps,screen);
		
		affiche_map(&m,screen,&p);
		afficher_perso(screen,p);
		SDL_Flip(screen);
		SDL_Delay(100);
	}
	
	freeTexttime(temps.temps);
	free_perso(p);
	SDL_Quit();

	return 1;
}
