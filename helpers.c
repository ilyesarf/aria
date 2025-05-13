#include "helpers.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

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

SDL_Surface *load_image(char *filename) {
    SDL_Surface *image = IMG_Load(filename);
    if (image == NULL) {
        printf("Error opening image: %s\n", IMG_GetError());
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

Mix_Music* load_music(const char* filename) {
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Failed to load music '%s': %s\n", filename, Mix_GetError());
    }
    return music;
}