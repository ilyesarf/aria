#include "header.h"
#include "menu_save/menuSave.h"
#include "menu_option/menuOption.h"
#include "menu_best_score/menuBestScore.h"
#include "menu_joueur/menuJoueur.h"
#include "menu_enigme/menuEnigme.h"
#include "menu_principal/menuPrincipal.h"

SDL_Surface* init_screen() {
    SDL_Surface *screen;  
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (!screen){
        printf("Error init screen !!\n");
        exit(1);
    }
    SDL_WM_SetCaption("JEU", NULL);
    return screen;
}

void init_audio() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

SDL_Surface* load_image(char *filename) {
    SDL_Surface *image = IMG_Load(filename);
    if (image == NULL) {
        printf("Error opening image: %s!! \n\n", filename);
        exit(1);
    }
    return image;
}

Mix_Chunk* load_sound(char *filename) {
    Mix_Chunk *hoverSound = Mix_LoadWAV(filename);
    if (hoverSound == NULL) {
        printf("Error opening audio file: %s!!\n\n", filename);
        exit(1);
    }
    return hoverSound;
}

TTF_Font* load_font(char *filename) {
    TTF_Font* font = TTF_OpenFont(filename, 40);
    if (font == NULL) {
        printf("Error opening font file: %s!!\n\n", filename);
        exit(1);
    }
    return font;
}

void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface) {
        SDL_Rect textLocation = {x, y, 0, 0};
        SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
        SDL_FreeSurface(textSurface);
    }
}

void renderButton(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button button){
    if (button.selected){
        if (button.hoverImage != NULL) {
            SDL_BlitSurface(button.hoverImage, NULL, screen, &(SDL_Rect){button.rect.x-50, button.rect.y-50, 0, 0});
        }
    } else{
        if(button.normalImage != NULL){
            SDL_BlitSurface(button.normalImage, NULL, screen, &(SDL_Rect){button.rect.x-50, button.rect.y-50, 0, 0});
        }
    }
}

void init_menus(Menu *menus){
    int working_menus[7] = {MENU_SAVE, MENU_NEW_LOAD_SAVE, MENU_BEST_SCORE, MENU_PLAYER, MENU_PRINCIPAL, MENU_OPTION ,MENU_ENIGME};
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
}
Mix_Music* load_music(const char* filename) {
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Failed to load music '%s': %s\n", filename, Mix_GetError());
    }
    return music;
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

int main() {
    SDL_Surface *screen; 
    screen = init_screen();
    init_audio();
    TTF_Init();
    SDL_EnableUNICODE(1); // Enable Unicode translation for keyboard input
    SDL_Surface *background;
    background = load_image("./assets/game/background.png"); 
    TTF_Font *font;
    font = load_font("./assets/fonts/font.ttf");
    SDL_Color textColor = {255, 255, 255, 255}; // white text
    Mix_Chunk *hoverSound;
    Mix_Music *musique = load_music("./assets/music/30-hours.mp3");;
    hoverSound = load_sound("./assets/sounds/beep.wav"); 
    int volume = 50; // Initial volume (50%)

    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(volume); // Set initial volume


    Menu menus[N_MENUS];
    init_menus(menus);

    int menuState = MENU_PRINCIPAL; // Changed initial menuState to MENU_PRINCIPAL

    while (menuState != QUIT_GAME) {
        SDL_Event event;
        //SDL_PollEvent(&event);
        //printf("menu state: %d\n", menuState); 
        
        if (menuState == MAIN_GAME) {
            printf("MAIN GAME...\n");
            menuState = -1;
        } else {
            menus[menuState].render(background, screen, font, textColor, menus[menuState].buttons, menus[menuState].n_btns);
            menus[menuState].handleEvent(&menuState, event, menus[menuState].buttons, menus[menuState].n_btns, hoverSound);
            //printf("menu state: %d\n", menuState);
        }
        SDL_Flip(screen); // Ensure the screen is updated
        //SDL_Delay(16);
    }

    cleanup(hoverSound, musique, background, font, menus);
    return 0;
}
