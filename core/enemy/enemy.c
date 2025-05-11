#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>

void init_enemy(Enemy *enemy, int health, int x, int y) {
    enemy->health = health;
    enemy->x = x;
    enemy->y = y;
    enemy->current_frame = 0;

    enemy->frames[0] = IMG_Load("shadowanim/shadowf1t.png");
    enemy->frames[1] = IMG_Load("shadowanim/shadowf2t.png");
    enemy->frames[2] = IMG_Load("shadowanim/shadowf3t.png");
    enemy->frames[3] = IMG_Load("shadowanim/shadowf4t.png");

    for (int i = 0; i < 4; i++) {
        if (!enemy->frames[i]) {
            printf("Failed to load frame %d: %s\n", i, SDL_GetError());
            exit(1);
        }
    }
}

SDL_Surface* load_background(const char* filename) {
    SDL_Surface* image = IMG_Load(filename);
    if (!image) {
        printf("Failed to load background: %s\n", SDL_GetError());
        exit(1);
    }
    return image;
}

void display_enemy(Enemy *enemy, SDL_Surface* screen) {
    SDL_Rect dstrect = { enemy->x, enemy->y, 0, 0 };
    SDL_BlitSurface(enemy->frames[enemy->current_frame], NULL, screen, &dstrect);
    draw_enemy_health_bar(screen, enemy);
}

void draw_enemy_health_bar(SDL_Surface* screen, Enemy* enemy) {
    SDL_Rect bg = { enemy->x, enemy->y - 10, 50, 5 };
    SDL_Rect fg = { enemy->x, enemy->y - 10, (enemy->health * 50) / 100, 5 };

    SDL_FillRect(screen, &bg, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_FillRect(screen, &fg, SDL_MapRGB(screen->format, 0, 255, 0));
}

void animate_enemy_move(Enemy *enemy) {
    enemy->current_frame = (enemy->current_frame + 1) % 4;
}

void move_enemy_randomly(Enemy *enemy, int level) {
    if (enemy->steps_left <= 0) {
        // Calculate a new random direction
        do {
            enemy->dx = (rand() % (level == 1 ? 3 : 5)) - (level == 1 ? 1 : 2);
            enemy->dy = (rand() % (level == 1 ? 3 : 5)) - (level == 1 ? 1 : 2);
        } while (enemy->dx == 0 && enemy->dy == 0); // Ensure non-zero movement

        // Set steps_left to a random distance for continuous movement
        enemy->steps_left = rand() % 10 + 5; // Move between 5 to 15 steps in the same direction
    }

    // Move the enemy in the current direction
    enemy->x += enemy->dx;
    enemy->y += enemy->dy;

    // Decrease the steps remaining
    enemy->steps_left--;
}

void move_enemy_ai(Enemy *enemy, int player_x, int player_y) {
    if (player_x > enemy->x) enemy->x += 2;
    else if (player_x < enemy->x) enemy->x -= 2;

    if (player_y > enemy->y) enemy->y += 2;
    else if (player_y < enemy->y) enemy->y -= 2;
}

int check_collision_player_enemy(int player_x, int player_y, Enemy *enemy) {
    SDL_Rect r = { enemy->x, enemy->y, 300, 128 };
    if (player_x >= r.x && player_x <= r.x + r.w &&
        player_y >= r.y && player_y <= r.y + r.h) {
        return 1;
    }
    return 0;
}

int check_collision_enemy_es(SDL_Rect enemy_rect, SDL_Rect es_rect) {
    if ((enemy_rect.x + enemy_rect.w < es_rect.x) || (enemy_rect.x > es_rect.x + es_rect.w) ||
        (enemy_rect.y + enemy_rect.h < es_rect.y) || (enemy_rect.y > es_rect.y + es_rect.h)) {
        return 0;
    }
    return 1;
}

void update_health(Enemy *enemy, int damage) {
    enemy->health -= damage;
    if (enemy->health < 0) enemy->health = 0;
}
