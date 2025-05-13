#include "header.h"
#include "menu_save/menuSave.h"
#include "menu_option/menuOption.h"
#include "menu_best_score/menuBestScore.h"
#include "menu_joueur/menuJoueur.h"
#include "menu_enigme/menuEnigme.h"
#include "menu_principal/menuPrincipal.h"

const int working_menus[7] = {MENU_SAVE, MENU_NEW_LOAD_SAVE, MENU_BEST_SCORE, MENU_PLAYER, MENU_PRINCIPAL, MENU_OPTION ,MENU_ENIGME};


void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface) {
        SDL_Rect textLocation = {x, y, 0, 0};
        SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
        SDL_FreeSurface(textSurface);
    }
}

void renderButton(SDL_Surface *screen, SDL_Surface *butImage, TTF_Font *font, SDL_Color textColor, Button button){
    if (!butImage){
        printf("renderButton: ButImage = NULL\n");
        exit(1);
    }
    if (button.selected){
        SDL_BlitSurface(butImage, NULL, screen, &(SDL_Rect){button.rect.x-50, button.rect.y-50, 0, 0});
        renderText(screen, button.text, font, (SDL_Color){255,0,0,0}, button.rect.x+10, button.rect.y+10);
    } else{
        SDL_BlitSurface(butImage, NULL, screen, &(SDL_Rect){button.rect.x-50, button.rect.y-50, 0, 0});
        renderText(screen, button.text, font, textColor, button.rect.x+10, button.rect.y+10);
    }
}

void init_menus(Menu *menus){
    
    for (int i = 0; i < 7; i++) {
        if (working_menus[i] == MENU_SAVE) {
            initMenuSave(menus);
        } else if (working_menus[i] == MENU_NEW_LOAD_SAVE) {
            initMenuChooseSave(menus);
        } else if (working_menus[i] == MENU_BEST_SCORE) {
            initMenuBestScore(menus);
        } else if (working_menus[i] == MENU_PLAYER) {
            initMenuJoueur(menus);
        } else if (working_menus[i] == MENU_PRINCIPAL) {
            initMenuPrincipal(menus);
        } else if (working_menus[i] == MENU_OPTION) {
            initMenuOption(menus);
        } else if (working_menus[i] == MENU_ENIGME) {
            initMenuEnigme(menus);
        }
    }
    
    //initMenuSave(menus);
}



void cleanupMenus(Menu *menus) {
    cleanupMenuPrincipal(&menus[MENU_PRINCIPAL]);
    cleanupMenuSave(&menus[MENU_SAVE]);
    cleanupMenuOption(&menus[MENU_OPTION]);
    cleanupMenuBestScore(&menus[MENU_BEST_SCORE]);
    cleanupMenuJoueur(&menus[MENU_PLAYER]);
    cleanupMenuEnigme(&menus[MENU_ENIGME]);
    // Add cleanup calls for other menus as needed
}

void cleanup(Mix_Chunk *hoverSound, Mix_Music *musique, SDL_Surface *background, TTF_Font *font, Menu *menus) {
    cleanupMenus(menus); // Free all menus
    Mix_FreeChunk(hoverSound);
    hoverSound = NULL;
    Mix_FreeMusic(musique);
    musique = NULL;
    SDL_FreeSurface(background);
    background = NULL;
    TTF_CloseFont(font);
    font = NULL;
    TTF_Quit();
    SDL_Quit();
}

int menu(SDL_Surface *screen, SDL_Surface *background, TTF_Font *font, SDL_Color textColor, SDL_Surface *butImage, Mix_Chunk *hoverSound, Mix_Music *musique, int menuState, Save save, Menu *menus) {
    while (menuState != QUIT_GAME && menuState != MAIN_GAME) {
        SDL_Event event;
        //SDL_PollEvent(&event);
        //printf("menu state: %d\n", menuState); 
        

        menus[menuState].render(background, butImage, screen, font, textColor, menus[menuState].buttons, menus[menuState].n_btns);
        menus[menuState].handleEvent(&menuState, save, event, menus[menuState].buttons, menus[menuState].n_btns, hoverSound);

        SDL_Flip(screen); // Ensure the screen is updated
        //SDL_Delay(16);
    }

    cleanup(hoverSound, musique, background, font, menus);
    return 0;
}
