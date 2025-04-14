#ifndef ENEMY_H
#define ENEMY_H

#include <SDL/SDL.h>
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

void init_enemy(Enemy *enemy, int health, int x, int y);
void display_enemy(Enemy *enemy, SDL_Surface* screen);
void draw_enemy_health_bar(SDL_Surface* screen, Enemy* enemy);
void animate_enemy_move(Enemy *enemy);
void move_enemy_randomly(Enemy *enemy, int level);
void move_enemy_ai(Enemy *enemy, int player_x, int player_y);
int check_collision_player_enemy(int player_x, int player_y, Enemy *enemy);
int check_collision_enemy_es(SDL_Rect enemy_rect, SDL_Rect es_rect);
void update_health(Enemy *enemy, int damage);

#endif
