#include "menuSauv.h"


SDL_Surface* init_screen() {
    
    SDL_Surface *screen;  
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Menu Sauvegarde", NULL);

    return screen;
}

SDL_Surface* init_background(){
    SDL_Surface *background=NULL;
    background = IMG_Load("background.png"); // Use JPG or PNG
    if (background == NULL){
        printf("Error opening background image!!\n\n");
        exit(1);
    }
    return background;
}

Mix_Chunk* init_audio(){
    Mix_Chunk *hoverSound=NULL;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    hoverSound = Mix_LoadWAV("beep.wav");
    if (hoverSound == NULL){
        printf("Error opening audio file!!\n\n");
        exit(1);
    }

    return hoverSound;
}

TTF_Font* init_font(){
    TTF_Init();
    TTF_Font* font=NULL;
    font = TTF_OpenFont("font.ttf", 24); // Load font
    if (font == NULL){
        printf("Error opening font file !!\n\n");
        exit(1);
    }

    return font;
}

void init_buttons(Button *buttons){

    buttons[0].rect = (SDL_Rect){SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2, 100, 50}; // OUI
    buttons[0].text = "OUI";
    buttons[0].selected = 0;

    buttons[1].rect = (SDL_Rect){SCREEN_WIDTH/2+100, SCREEN_HEIGHT/2, 100, 50}; // NON
    buttons[1].text = "NON";
    buttons[1].selected = 0;

    // Create buttons for "Charger le jeu" and "Nouvelle Partie"
    buttons[2].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, 180, 200, 50} ;
    buttons[2].text = "Charger le jeu";
    buttons[2].selected = 0;

    buttons[3].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 100, 260, 200, 50};
    buttons[3].text = "Nouvelle Partie";
    buttons[3].selected = 0;
    
}

void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface) {
        SDL_Rect textLocation = {x, y, 0, 0};
        SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
        SDL_FreeSurface(textSurface);
    }
}

void renderMenuSauv(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    for (int i = 0; i < 2; i++) {
        SDL_Rect r = buttons[i].rect;
        Uint32 color = buttons[i].selected ? SDL_MapRGB(screen->format, 255, 0, 0) : SDL_MapRGB(screen->format, 255, 255, 255);
        SDL_FillRect(screen, &r, color);
        renderText(screen, buttons[i].text, font, textColor, r.x + 30, r.y + 15);
    }

    SDL_Flip(screen);
}

void renderMenuChooseSave(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons) {
    SDL_BlitSurface(background, NULL, screen, NULL); // Draw background


    // Render buttons
    for (int i = 2; i < 4; i++) {
        SDL_Rect r = buttons[i].rect;
        Uint32 color = buttons[i].selected ? SDL_MapRGB(screen->format, 255, 0, 0) : SDL_MapRGB(screen->format, 255, 255, 255);
        SDL_FillRect(screen, &r, color);
        renderText(screen, buttons[i].text, font, textColor, r.x + 30, r.y + 15);
    }

    SDL_Flip(screen);
}

void handleEvents(int *running, int *menuState, Button *buttons, Mix_Chunk *hoverSound) {
    SDL_Event event;
    int min_btns, max_btns;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) *running = 0;
        if (event.type == SDL_MOUSEMOTION) {
            if (*menuState==1){
                min_btns = 0;
                max_btns = 2;
            }else if(*menuState==2){
                min_btns = 2;
                max_btns = 4;
            }
            for (int i = min_btns; i < max_btns; i++) {
                if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                    event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                    buttons[i].selected = 1;
                } else {
                    buttons[i].selected = 0;
                }
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            for (int i = 0; i < 4; i++) {
                if (buttons[i].selected) {
                    if (i == 0) {
                        if(*menuState==1) Mix_PlayChannel(-1, hoverSound, 0);
                        *menuState =2; // OUI -> Next menu
                    }
                    else if (i==1){ 
                        if(*menuState==1) Mix_PlayChannel(-1, hoverSound, 0);
                        *running = 0; // NON -> Exit
                    }
                    else if (i==2) *menuState = 0; // Charger jeu
                    else if (i==3) *menuState = 3; //Nouvelle jeu
                }
            }
        }
    }
    if ((*menuState==2) && (event.type == SDL_KEYDOWN)){
        switch(event.key.keysym.sym){
            case SDLK_n:
                *menuState = 3;
                break;
            case SDLK_0:
                printf("0\n");
                break;

        }
    }
}


void cleanup(Mix_Chunk *hoverSound, SDL_Surface *background, TTF_Font *font) {
    Mix_FreeChunk(hoverSound);
    SDL_FreeSurface(background);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
