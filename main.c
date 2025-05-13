#include "header.h"

int main(){
    SDL_Surface *screen; 
    screen = init_screen();
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        exit(1);
    }
    
    SDL_EnableUNICODE(1); // Enable Unicode translation for keyboard input

    SDL_WM_SetCaption("ARIA", NULL);

    
}