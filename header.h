#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define N_BTNS 4

typedef struct {
    SDL_Rect rect;
    char *text;
    int selected;
} Button;

//SDL_Surface* init_screen();
SDL_Surface* init_screen() {
    
    SDL_Surface *screen;  
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Menu Sauvegarde", NULL);

    return screen;
}