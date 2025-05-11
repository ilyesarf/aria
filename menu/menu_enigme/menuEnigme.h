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


// MenuEnigme Functions
void initMenuEnigme(Menu *menus);
void initMenuEnigmeButtons(Button *buttons);
void renderMenuEnigme(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleEventEnigme(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void cleanupMenuEnigme(Menu *menu) ;

// QuizEnigme Functions
void loadQuestions(const char *filename, Question *questions, int *numQuestions);
void renderQuizUI(SDL_Surface *screen, SDL_Surface *butImage, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
void handleQuizEvent(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
void checkAnswer(int selectedOption, int correctOption, int *score, int *lives, int *level, int *menuState);
void displayQuizUI(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);

// PuzzleEnigme Functions
void renderPuzzle(SDL_Surface *screen);
void handlePuzzleEvent(SDL_Event event, int *menuState, SDL_Surface *screen);
void updatePuzzleTimer(int *menuState, SDL_Surface *screen);
void displayRotozoomMessage(SDL_Surface *message, SDL_Surface *screen);
//void shufflePieces() ;



#endif