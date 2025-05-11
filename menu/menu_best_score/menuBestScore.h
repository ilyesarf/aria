#ifndef MENUBESTSCORE_H
#define MENUBESTSCORE_H


#include <stdio.h>
#include <unistd.h>


#include "../header.h"

typedef struct {
    char name[50];
    int score;
} ScoreEntry;

void initMenuBestScore(Menu *menus);
void initMenuBestScoreButtons(Button *buttons);
void renderMenuBestScore(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventBestScore(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void renderInputBox(SDL_Surface *screen, TTF_Font *font, const char *inputText);
void handleInput(SDL_Event event, char *inputText, int maxLength);
void loadScores(ScoreEntry scores[]);
void saveScores(ScoreEntry scores[]);
void addScore(ScoreEntry scores[], const char *name, int score);
void cleanupMenuBestScore(Menu *menu) ;
#endif
