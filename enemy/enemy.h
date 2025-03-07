#ifndef ENEMY_H
#define ENEMY_H

#include <SDL/SDL.h>

typedef struct {
    int health;
    int x, y; // Position of the enemy
    SDL_Surface* sprite; // Enemy sprite
} Enemy;

void init_enemy(Enemy *enemy, int health, int x, int y, SDL_Surface* sprite);
void display_enemy(Enemy *enemy, SDL_Surface* screen);
void move_enemy_randomly(Enemy *enemy, int level);
void animate_enemy_move(Enemy *enemy);
void animate_enemy_attack(Enemy *enemy);
int check_collision_player_enemy(int player_x, int player_y, Enemy *enemy);
void update_health(Enemy *enemy, int damage);
void display_health(Enemy *enemy);

#endif // ENEMY_H