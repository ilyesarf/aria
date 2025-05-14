#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

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


SDL_Surface *load_image(char *filename) {
    SDL_Surface *image = IMG_Load(filename);
    if (image == NULL) {
        printf("Error opening image: %s\n", IMG_GetError());
        exit(1);
    }
    return image;
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

void renderButton(SDL_Surface *screen, SDL_Surface *butImage, TTF_Font *font, SDL_Color textColor){
    if (!butImage){
        printf("renderButton: ButImage = NULL\n");
        exit(1);
    }
    SDL_BlitSurface(butImage, NULL, screen, &(SDL_Rect){150, 70, 0, 0});
    renderText(screen, "Test", font, (SDL_Color){255,0,0,0}, 110, 20+10);
}

int main(){
    SDL_Surface *screen; 
    screen = init_screen();
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        exit(1);
    }
    
    SDL_EnableUNICODE(1); // Enable Unicode translation for keyboard input

    SDL_Surface *background;
    background = load_image("../assets/game/background.png"); 

    TTF_Init();
    TTF_Font *font;
    font = load_font("../assets/fonts/font.ttf");
    SDL_Color textColor = {255, 255, 255, 255}; // white text
    
    SDL_Surface *butImage = NULL;
    butImage = load_image("../assets/buttons/butbase.png");
    
    if (!butImage){
        printf("renderButton: ButImage = NULL\n");
        exit(1);
    }
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_BlitSurface(butImage, NULL, screen, &(SDL_Rect){150, 70, 0, 0});
    renderText(screen, "Test", font, (SDL_Color){255,0,0,0}, 110, 20+10);
    SDL_Flip(screen);
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (event.button.x >= 150 && event.button.x <= 150 + 300 && event.button.y >= 70 && event.button.y <= 70 + 40) {
                        printf("Button clicked!\n");
                    }
                }
            }
        }
    }
    SDL_FreeSurface(background);
    SDL_FreeSurface(butImage);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}