#include <SDL/SDL.h>//inclure SDL.h  pour permettre au programme d'être portable sur tous les systèmes.
#include <SDL/SDL_image.h>//.................//Pour manipuler des images ayants des types autre que bmp
#include <SDL/SDL_mixer.h>//.................//Pour manipuler de l’audio.
#include <SDL/SDL_ttf.h>//.................//Pour manipuler des textes
#include <stdio.h>
#include <unistd.h>

#include "menuBestScore.h"
#include "../header.h"

#define MAX_SCORES 10

static int inputActive = 1;
static char inputText[10] = "";

ScoreEntry scores[MAX_SCORES];

void initMenuBestScore(Menu *menus) {
    printf("Init Menu Best Score\n");
    menus[MENU_BEST_SCORE].n_btns = 3;
    menus[MENU_BEST_SCORE].buttons = malloc(menus[MENU_BEST_SCORE].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_BEST_SCORE].init_buttons = initMenuBestScoreButtons;
    menus[MENU_BEST_SCORE].render = renderMenuBestScore;
    menus[MENU_BEST_SCORE].handleEvent = handleEventBestScore;
    menus[MENU_BEST_SCORE].init_buttons(menus[MENU_BEST_SCORE].buttons);
}

void initMenuBestScoreButtons(Button *buttons) {
    buttons[0].normalImage = load_image("buttons/submit.png");
    buttons[0].hoverImage = load_image("buttons/submithov.png");
    buttons[0].rect = (SDL_Rect){SCREEN_WIDTH / 2 + 125, SCREEN_HEIGHT / 2 + 75, 75, 35}; // Valider
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("buttons/quit.png");
    buttons[1].hoverImage = load_image("buttons/quithov.png");
    buttons[1].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 50, 100, 50}; // Quitter
    buttons[1].selected = 0;

    buttons[2].normalImage = load_image("buttons/return.png");
    buttons[2].hoverImage = load_image("buttons/returnhov.png");
    buttons[2].rect = (SDL_Rect){SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 125, 100, 50}; // Retour
    buttons[2].selected = 0;
}

void renderMenuBestScore(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    if (inputActive) {
        renderInputBox(screen, font, inputText);
        renderButton(screen, font, textColor, buttons[0]);
    } else {
        for (int i = 1; i < n_btns; i++) {
            renderButton(screen, font, textColor, buttons[i]);
        }
    }

    SDL_Flip(screen);
}

void handleEventBestScore(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) *menuState = QUIT_GAME;
        if (event.type == SDL_MOUSEMOTION) {
            for (int i = 0; i < n_btns; i++) {
                if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                    event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                    buttons[i].selected = 1;
                } else {
                    buttons[i].selected = 0;
                }
            }
        }
    
        if (inputActive) {
            handleInput(event, inputText, sizeof(inputText));
            if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) || (event.type == SDL_MOUSEBUTTONDOWN && buttons[0].selected)) {
                printf("User entered: %s\n", inputText);  // Simulate action
                inputActive = 0; // Deactivate input box
                addScore(scores, inputText, 100); // Example score
                saveScores(scores);
            }
        } else {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < n_btns; i++) {
                        if (buttons[i].selected) {
                            if (i == 1) {
                                *menuState = MENU_PRINCIPAL; // Quitter -> Exit
                            } else if (i == 2) {
                                inputActive = 1; // Retour -> Exit
                            }
                        }
                    }
                }
            }
        }
        SDL_Delay(10);
    } 
}


void renderInputBox(SDL_Surface *screen, TTF_Font *font, const char *inputText) {
    SDL_Rect box = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 200, 50};  
    SDL_FillRect(screen, &box, SDL_MapRGB(screen->format, 255, 255, 255));  // White box

    SDL_Color textColor = {0, 0, 0, 0};  // Black color
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, inputText, textColor);
    if (textSurface) {
        SDL_Rect textPosition = {box.x + 10, box.y + 10, 0, 0};  // Offset text inside the box
        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);
        SDL_FreeSurface(textSurface);
    }

    SDL_Flip(screen);
}

void handleInput(SDL_Event event, char *inputText, int maxLength) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
            inputText[strlen(inputText) - 1] = '\0';  // Remove last character
        } 
        else {
            Uint16 unicode = event.key.keysym.unicode;  // Get Unicode character
            if (unicode >= 32 && unicode <= 126 && (int)strlen(inputText) < maxLength - 1) { 
                printf("char: %c\n", unicode);
                int len = strlen(inputText);
                inputText[len] = (char)unicode;  // Add character
                inputText[len + 1] = '\0';  // Null-terminate
            }
        }
    }
}

void saveScores(ScoreEntry scores[]) {
    FILE *file = fopen("scores.dat", "w");
    if (file) {
        fwrite(scores, sizeof(ScoreEntry), MAX_SCORES, file);
        fclose(file);
    }
}

void addScore(ScoreEntry scores[], const char *name, int score) {
    for (int i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i].score) {
            for (int j = MAX_SCORES - 1; j > i; j--) {
                scores[j] = scores[j - 1];
            }
            strncpy(scores[i].name, name, sizeof(scores[i].name));
            scores[i].score = score;
            break;
        }
    }
}

void loadScores(ScoreEntry scores[]) {
    FILE *file = fopen("scores.dat", "r");
    if (file) {
        fread(scores, sizeof(ScoreEntry), MAX_SCORES, file);
        fclose(file);
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(scores[i].name, "---");
            scores[i].score = 0;
        }
    }
}

