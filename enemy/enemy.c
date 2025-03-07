#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

void init_enemy(Enemy *enemy, int health, int x, int y, SDL_Surface* frames[]) {
    enemy->health = health;
    enemy->x = x;
    enemy->y = y;
    for (int i = 0; i < 4; i++) {
        enemy->frames[i] = frames[i];
    }
    enemy->current_frame = 0;
}

void display_enemy(Enemy *enemy, SDL_Surface* screen) {
    SDL_Rect dstrect;
    dstrect.x = enemy->x;
    dstrect.y = enemy->y;
    SDL_BlitSurface(enemy->frames[enemy->current_frame], NULL, screen, &dstrect);
}

void move_enemy_randomly(Enemy *enemy, int level) {
    if (level == 1) {
        enemy->x += (rand() % 3) - 1; // Move randomly in x direction
        enemy->y += (rand() % 3) - 1; // Move randomly in y direction
    } else if (level == 2) {
        enemy->x += (rand() % 5) - 2; // Move more randomly in x direction
        enemy->y += (rand() % 5) - 2; // Move more randomly in y direction
    }
}

void animate_enemy_move(Enemy *enemy) {
    enemy->current_frame = (enemy->current_frame + 1) % 4;
}

void animate_enemy_attack(Enemy *enemy) {
    // Code to animate enemy attack
}

int check_collision_player_enemy(int player_x, int player_y, Enemy *enemy) {
    // Simplified collision detection
    if (player_x == enemy->x && player_y == enemy->y) {
        return 1; // Collision detected
    }
    return 0; // No collision
}

void update_health(Enemy *enemy, int damage) {
    enemy->health -= damage;
}

void display_health(Enemy *enemy) {
    printf("Enemy health: %d\n", enemy->health);
}
