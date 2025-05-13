#ifndef HELPERS_H
#define HELPERS_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 512

// Forward declarations
struct Enemy;
struct Background;

typedef struct Player {
    SDL_Surface* img;
    SDL_Rect pos;
    int health;
    int lives;
    int score;
    float velocity;
    float spd;
    SDL_Rect posSprite;
    int state;
    float acc;
    Uint32 last_damage_time;
    float vy;
    int is_jumping;
    int facing_left;
} Player;

typedef struct Level {
    int n;
    struct Background *background;
    struct Enemy *enemies;
} Level;

typedef struct Save {
    Player *players;
    Level level;
} Save;

// Function declarations
SDL_Surface* init_screen();
void init_audio();
SDL_Surface* load_image(char *filename);
Mix_Chunk* load_sound(char *filename);
TTF_Font* load_font(char *filename);
Mix_Music* load_music(const char* filename);

#endif // HELPERS_H 