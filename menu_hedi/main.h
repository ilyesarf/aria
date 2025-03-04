#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_SCORES 3

typedef struct {
    char name[20];
    int score;
} ScoreEntry;

void initSDL();
void cleanUp(SDL_Surface *screen);
void handleEvents(int *running, SDL_Surface *screen, SDL_Surface *exitButton, int buttonX, int buttonY,int *valider,char *inputText,int buttonX2, int buttonY2, int buttonX3, int buttonY3,int *menustate);
void renderBestScores(SDL_Surface *screen, ScoreEntry scores[],TTF_Font *font);
void loadScores(ScoreEntry scores[]);
void saveScores(ScoreEntry scores[]);
void addScore(ScoreEntry scores[], const char *name, int score);
SDL_Surface* load_image(const char* filename);
TTF_Font* load_font(const char* filename, int size);
Mix_Music* load_music(const char* filename);
SDL_Surface* create_window(int width, int height);
void displayText(SDL_Surface *screen, const char *message, int x, int y);
void displayButton(SDL_Surface *screen, SDL_Surface *button, int x, int y);
void renderInputBox(SDL_Surface *screen, TTF_Font *font, const char *inputText);
void handleInput(SDL_Event event, char *inputText, int maxLength);


#endif // MAIN_H_INCLUDED
