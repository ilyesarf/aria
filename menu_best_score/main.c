#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[]) {
    SDL_Surface *screen = create_window(1920,1080);
    SDL_Surface* image = load_image("backg.png");
    SDL_Surface* texte=NULL;
    SDL_Surface* button=load_image("button.png");
    SDL_Rect posbutton = {50, 80};
    SDL_Rect posbutton2 = {40, 120};
    SDL_Rect posbutton3 = {680, 120};
    SDL_Color textColor = {0, 0, 0};
    Mix_Music* music = load_music("palestine.mp3");
    TTF_Font *font = load_font("KiwiSoda.ttf", 80);
    ScoreEntry scores[MAX_SCORES];
    SDL_Event event;

    // Initialisation SDL
    initSDL(&screen);
    SDL_EnableUNICODE(1);
    loadScores(scores);
    Mix_PlayMusic(music, -1);
    int running = 1;int valider=1;int menustate=7;
    char inputText[50] ="";
    
    
    while (running) {
        
        while ((valider))
        {
            SDL_BlitSurface(image,NULL,screen,NULL);
            SDL_BlitSurface(button,NULL,screen,&posbutton);
            displayText(screen,"valider",700,650);
            renderInputBox(screen, font, inputText);
            handleEvents(&running, screen, button, 650, 500,&valider,inputText,700, 700, 1380, 700,&menustate);
            SDL_Flip(screen);
        }
        SDL_BlitSurface(image,NULL,screen,NULL);
        renderBestScores(screen, scores,font);
        SDL_BlitSurface(button,NULL,screen,&posbutton2);
        displayText(screen,"Quitter",700,700);
        SDL_BlitSurface(button,NULL,screen,&posbutton3);
        displayText(screen,"Retour",1380,700);
        handleEvents(&running, screen, button, 650, 500,&valider,inputText,600, 550, 1100, 550,&menustate);
        SDL_Flip(screen);
    }

    saveScores(scores);
    cleanUp(screen);
    return 0;
}