#include <SDL/SDL_image.h>

typedef struct {
    int health;
    int x, y; // Position
    SDL_Surface* frames[4]; // Animation frames
    int current_frame;
} Enemy;

typedef struct {
    SDL_Rect rect; // Position et taille de l'obstacle
} StaticElement;

typedef struct{
    int n;
    SDL_Surface *background;
    Enemy *enemies;
    StaticElement *static_elements;
} Level;

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
} Player;

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
} Input;

typedef struct {
    SDL_Rect pos1;
    SDL_Rect pos2;
    SDL_Surface* img;
} Image;

typedef struct {
    Player *players;
    Level level;
} Save;

void renderSave(Save save);