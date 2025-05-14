#ifndef ENEMY_H
#define ENEMY_H

#include <SDL/SDL.h>
#include <math.h>
#include <SDL/SDL_image.h>
#define S1 600  // Vision range
#define S2 100  // Attack range
#define SCREEN_WIDTH 800   // Your screen width
#define SCREEN_HEIGHT 600  // Your screen height
/**
 *@struct EnemyState
 *@brief struct for enemy state
*/
typedef enum {
    WAITING,
    FOLLOWING,
    ATTACKING
} EnemyState;

/**
 *@struct Enemy
 *@brief struct for enemy
*/


typedef struct {
    int health;
    int x, y; // Position
    SDL_Surface* frames[4]; // Animation frames
    int current_frame;
    int dx, dy;       // Current direction of movement
    EnemyState state;
} Enemy;

/**
 *@struct StaticEleme,t
 *@brief struct for a static element (obstacle,character-like figure)
*/

typedef struct {
    SDL_Rect rect; // Position et taille de l'obstacle
} StaticElement;

void init_enemy(Enemy *enemy, int health, int x, int y);
void initEnemy(Enemy* enemy, int startX, int startY);
SDL_Surface* load_background(const char* filename);
void display_enemy(Enemy *enemy, SDL_Surface* screen);
void draw_enemy_health_bar(SDL_Surface* screen, Enemy* enemy);
void animate_enemy_move(Enemy *enemy);
void move_enemy_randomly(Enemy *enemy,int level);
void move_enemy_randomly2(Enemy *enemy, int level);
void move_enemy_ai(Enemy *enemy, int player_x, int player_y, int s1, int s2);
int check_collision_player_enemy(SDL_Rect player_rect, Enemy *enemy);
int check_collision_enemy_es(SDL_Rect enemy_rect, SDL_Rect es_rect);
void update_health(Enemy *enemy, int damage);

#endif 
