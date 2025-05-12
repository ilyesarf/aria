#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/**
 * @file enemy.c
 * @brief Testing programm.
 * @author Med Hedi Belaid
 * @version 1.0
 * @date may 12, 2025
 *
 *Testing program for enemy
 *
*/
/**
 *@brief to initialize the enemy
 *@param enemy position x,y ; health value; enemy pointer on the enemy struct
 *@return nothing
*/
void init_enemy(Enemy *enemy, int health, int x, int y) {
    enemy->health = health;
    enemy->x = x;
    enemy->y = y;
    enemy->current_frame = 0;
    enemy->state = WAITING;

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
/**
 *@brief to initialize the background
 *@param filename the path of the image file
 *@return the image initialized
*/

SDL_Surface* load_background(const char* filename) {
    SDL_Surface* image = IMG_Load(filename);
    if (!image) {
        printf("Failed to load background: %s\n", SDL_GetError());
        exit(1);
    }
    return image;
}
/**
 *@brief to show the enemy 
 *@param pointer on the enemy struct
 *@param screen the surface of the screen
 *@return nothing
*/


void display_enemy(Enemy *enemy, SDL_Surface* screen) {
    SDL_Rect dstrect = { enemy->x, enemy->y, 0, 0 };
    SDL_BlitSurface(enemy->frames[enemy->current_frame], NULL, screen, &dstrect);
    draw_enemy_health_bar(screen, enemy);
}
/**
 *@brief to draw the enemy health bar
 *@param the screen;pointer on enemy
 *@return nothing
*/


void draw_enemy_health_bar(SDL_Surface* screen, Enemy* enemy) {
    SDL_Rect bg = { enemy->x, enemy->y - 10, 50, 5 };
    SDL_Rect fg = { enemy->x, enemy->y - 10, (enemy->health * 50) / 100, 5 };

    SDL_FillRect(screen, &bg, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_FillRect(screen, &fg, SDL_MapRGB(screen->format, 0, 255, 0));
}
/**
 *@brief to animate the enemy
 *@param pointer on the enemy struct 
 *@return nothing
*/


void animate_enemy_move(Enemy *enemy) {
    enemy->current_frame = (enemy->current_frame + 1) % 4;
}
/**
 *@brief to make the enemy move randomly
 *@param pointer on enemy ; the number of the level
 *@return nothing
*/


void move_enemy_randomly(Enemy *enemy, int level) {
    // Persistent direction tracking
    static int last_dir_change = 0;
    const int dir_change_interval = 1000; // Time in milliseconds until direction change
    
    // Initialize directions if needed
    if (enemy->dx == 0 && enemy->dy == 0) {
        enemy->dx = (rand() % 2) ? 1 : -1;
        enemy->dy = (rand() % 2) ? 1 : -1;
    }
    
    // Change direction periodically
    if (SDL_GetTicks() - last_dir_change > dir_change_interval) {
        // 25% chance to change x direction
        if (rand() % 4 == 0) enemy->dx = (rand() % 2) ? 1 : -1;
        
        // 25% chance to change y direction 
        if (rand() % 4 == 0) enemy->dy = (rand() % 2) ? 1 : -1;
        
        last_dir_change = SDL_GetTicks();
    }
    
    // Apply movement with slow speed
    int move_speed = 5; // Set a small speed for slow movement
    enemy->x += enemy->dx * move_speed;
    enemy->y += enemy->dy * move_speed;
    
    // Screen boundary checks with bounce
    if (enemy->x < 0) {
        enemy->x = 0;
        enemy->dx = 1; // Force right movement
    }
    if (enemy->y < 0) {
        enemy->y = 0;
        enemy->dy = 1; // Force down movement
    }
    if (enemy->x > SCREEN_WIDTH - enemy->frames[0]->w) {
        enemy->x = SCREEN_WIDTH - enemy->frames[0]->w;
        enemy->dx = -1; // Force left movement
    }
    if (enemy->y > SCREEN_HEIGHT - enemy->frames[0]->h) {
        enemy->y = SCREEN_HEIGHT - enemy->frames[0]->h;
        enemy->dy = -1; // Force up movement
    }
}

/**
 *@brief to move the enemy based on player's position (track him)
 *@param pointer on enemy; players position x and y
 *@param s1 enemy's vision range
 *@param s2 enemy's attack range
 *@return nothing
*/


void move_enemy_ai(Enemy *enemy, int player_x, int player_y, int s1, int s2) {
    // Calculate the distance between the enemy and the player
    int deltaX = player_x - enemy->x;
    int deltaY = player_y - enemy->y;
    int distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    // Update the enemy's state based on the distance
    switch (enemy->state) {
        case WAITING:
            if (distance <= s1 && distance > s2) {
                enemy->state = FOLLOWING; // Transition to FOLLOWING state
            }
            break;

        case FOLLOWING:
            if (distance <= s2) {
                enemy->state = ATTACKING; // Transition to ATTACKING state
            } else if (distance > s1) {
                enemy->state = WAITING; // Transition back to WAITING state
            }
            break;

        case ATTACKING:
            if (distance > s2) {
                enemy->state = FOLLOWING; // Transition back to FOLLOWING state
            }
            break;
    }

    // Perform actions based on the current state
    switch (enemy->state) {
        case WAITING:
            // Enemy stays in place
            break;

        case FOLLOWING:
            // Move toward the player
            if (player_x > enemy->x) enemy->x += 2;
            else if (player_x < enemy->x) enemy->x -= 2;

            if (player_y > enemy->y) enemy->y += 2;
            else if (player_y < enemy->y) enemy->y -= 2;
            break;

        case ATTACKING:
            // Move faster toward the player for attacking
            if (player_x > enemy->x) enemy->x += 4;
            else if (player_x < enemy->x) enemy->x -= 4;

            if (player_y > enemy->y) enemy->y += 4;
            else if (player_y < enemy->y) enemy->y -= 4;
            break;
    }
}
/**
 *@brief to move the enemy randomly but faster
 *@param pointer on enemy struct ; number of the level
 *@return nothing
*/


void move_enemy_randomly2(Enemy *enemy, int level) {
    // Persistent direction tracking
    static int last_dir_change = 0;
    const int dir_change_interval = 500; // frames until direction change
    
    // Initialize directions if needed
    if (enemy->dx == 0 && enemy->dy == 0) {
        enemy->dx = (rand() % 2) ? 1 : -1;
        enemy->dy = (rand() % 2) ? 1 : -1;
    }
    
    // Change direction periodically
    if (SDL_GetTicks() - last_dir_change > dir_change_interval) {
        // 25% chance to change x direction
        if (rand() % 4 == 0) enemy->dx = (rand() % 2) ? 1 : -1;
        
        // 25% chance to change y direction 
        if (rand() % 4 == 0) enemy->dy = (rand() % 2) ? 1 : -1;
        
        last_dir_change = SDL_GetTicks();
    }
    
    // Apply movement 
    int move_speed = 35;
    enemy->x += enemy->dx * move_speed;
    enemy->y += enemy->dy * move_speed;
    
    // Screen boundary checks with bounce
    if (enemy->x < 0) {
        enemy->x = 0;
        enemy->dx = 1; // Force right movement
    }
    if (enemy->y < 0) {
        enemy->y = 0;
        enemy->dy = 1; // Force down movement
    }
    if (enemy->x > SCREEN_WIDTH - enemy->frames[0]->w) {
        enemy->x = SCREEN_WIDTH - enemy->frames[0]->w;
        enemy->dx = -1; // Force left movement
    }
    if (enemy->y > SCREEN_HEIGHT - enemy->frames[0]->h) {
        enemy->y = SCREEN_HEIGHT - enemy->frames[0]->h;
        enemy->dy = -1; // Force up movement
    }
}
/**
 *@brief to check if there is a collision between the enemy and the player
 *@param player's hitbox ; pointer on enemy struct
 *@return 1 if there is collision 0 if no
*/


int check_collision_player_enemy(SDL_Rect player_rect, Enemy *enemy) {
    SDL_Rect enemy_rect = { 
        enemy->x, 
        enemy->y, 
        enemy->frames[0]->w,  // Use width of first frame
        enemy->frames[0]->h   // Use height of first frame
    };
    
    // Check for rectangle-rectangle collision
    if (player_rect.x + player_rect.w < enemy_rect.x ||  // Player is left of enemy
        player_rect.x > enemy_rect.x + enemy_rect.w ||   // Player is right of enemy
        player_rect.y + player_rect.h < enemy_rect.y ||  // Player is above enemy
        player_rect.y > enemy_rect.y + enemy_rect.h) {   // Player is below enemy
        return 0; // No collision
    }
    return 1; // Collision detected
}
/**
 *@brief to check collision between enemy and a platform
 *@param enemy's hitbox; platform's hitbox
 *@return 1 if there is collision 0 if no
*/


int check_collision_enemy_es(SDL_Rect enemy_rect, SDL_Rect es_rect) {
    if ((enemy_rect.x + enemy_rect.w < es_rect.x) || (enemy_rect.x > es_rect.x + es_rect.w) ||
        (enemy_rect.y + enemy_rect.h < es_rect.y) || (enemy_rect.y > es_rect.y + es_rect.h)) {
        return 0;
    }
    return 1;
}
/**
 *@brief to update the enemy's health 
 *@param pointer on enemy struct; value of damage taken
 *@return nothing
*/


void update_health(Enemy *enemy, int damage) {
    enemy->health -= damage;
    if (enemy->health < 0) enemy->health = 0;
}
