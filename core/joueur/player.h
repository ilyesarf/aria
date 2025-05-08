#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_W 1300
#define SCREEN_H 730
#define MAX_BALLS 10
#define BALL_SPEED 8
#define BALL_WIDTH 30
#define BALL_HEIGHT 30

typedef struct {
    SDL_Surface* img;
    SDL_Rect pos;
    int lives;
    int score;
    int velocity;
    int acceleration;
    float spd;
    SDL_Rect posSprite;
    int state;
    float acc;
} character;

typedef struct {
    SDL_Surface* img;
    SDL_Rect pos;
    int active;
    float dx, dy;
} Ball;

typedef struct {
    int left, right, jump, accel, dec;
    int q, d, a, z, f, e, space;
    int escape;
    int down;
    int interface;
    int c1, k1;
    int spark1, ash1;
    int h,j; // Added for ball attack
} in;

typedef struct {
    SDL_Rect pos1;
    SDL_Rect pos2;
    SDL_Surface* img;
} image;

// Initialization functions
void initCharacter(character* player);
void initCharacter2(character* player2);
void initCharacter3(character* player3);
void initialize_backg(image* backg);
void initialize_backg2(image* backg2);
void initialize_btn1(image btn1[]);
void initialize_btn2(image btn2[]);
void initBalls();

// Display functions
void display_btn1(SDL_Surface* screen, image btn1);
void display_btn2(SDL_Surface* screen, image btn2);
void display_backg(SDL_Surface* screen, image backg);
void display_backg2(SDL_Surface* screen, image backg2);
void display_player(SDL_Surface* screen, character* player);
void display_player2(SDL_Surface* screen, character* player2);
void display_player3(SDL_Surface* screen, character* player3);
void displayBalls(SDL_Surface* screen);

// Player control functions
void get_input(in* i, int k);
void get_keyp2(in* i);
void move_character(in* input, character* c, character* c2, Uint32 dt);
void jump_character(in* input, character* c, int* jump_height);
void jump_character2(in* input, character* c2, int* jump_height2);
void animation(character* player, in input);
void animation2(character* player, in input);

// Ball system functions
void createBall(character* player);
void updateBalls();
void createBall1(character* player2);

// Free functions
void free_player(character* player);
void free_backg(image* backg);
void free_btn(image* btn);
void freeBalls();

#endif