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
	SDL_Surface *screen,*backg,*backgm,*enig;
	SDL_Rect backg_pos,backgm_pos,enigpos;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	screen=SDL_SetVideoMode(1500,1000,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(screen==NULL)
	{
		printf("unable to set video mode:%s\n",SDL_GetError());
		return 1;
	}
	enig = IMG_Load("1.jpg");
	enigpos.x=0;
	enigpos.y=0;
	
	backg = SDL_LoadBMP("backgg1.bmp");
	backg_pos.x = 0;
	backg_pos.y = 0;
	
//initialisation du temps et de map et du perso 

	Time temps;
	Time tempsenig;
	map m;
	personne p;
	
	inittemps(&temps);
	inittempsenig(&tempsenig);
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
		update_timeenig(&tempsenig);
		SDL_BlitSurface(enig,NULL,screen,&enigpos);
		displaytime(tempsenig,screen);
		SDL_Flip(screen);
		if(tempsenig.ss==15)
		{done=0;
		}
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
	sauvegarder(300,"mouard","test.txt");
	char* nomjoueur;
	int score;
	meilleur ("test.txt",score,nomjoueur);
	freeTexttime(temps.temps);
	free_perso(p);
	SDL_Quit();

	return 1;
}
