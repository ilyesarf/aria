#ifndef MENUENIGME_H
#define MENUENIGME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../header.h"
#define MAX_QUESTIONS 10

typedef struct {
    char question[256];
    char options[3][256];
    int correctOption;
} Question;

void initMenuEnigme(Menu *menus);
void initMenuEnigmeButtons(Button *buttons);
void displayQuizUI(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void renderMenuEnigme(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventEnigme(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void loadQuestions(const char *filename, Question *questions, int *numQuestions);
void checkAnswer(int selectedOption, int correctOption, int *score, int *lives, int *level, int *menuState);

#endif