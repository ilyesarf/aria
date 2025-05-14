#include "enemy.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <time.h>
/**
 * @file enemymain.c
 * @brief Testing programm.
 * @author Med Hedi Belaid
 * @version 1.0
 * @date may 12, 2025
 *
 *Testing program for enemy
 *
*/
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    IMG_Init(IMG_INIT_PNG);
    srand(time(NULL));

    // Load the background image
    SDL_Surface* background = load_background("shadowanim/lastlvl.png");

    Enemy shadow;
    init_enemy(&shadow, 100, 150, 100);

    StaticElement wall = { .rect = {300, 300, 100, 100} }; // Example player
    StaticElement wall2 = { .rect = {400, 200, 100, 20} }; // Example obstacle

    int running = 1;
    SDL_Event event;
    int player_x = 300, player_y = 300;
    int level = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        SDL_BlitSurface(background, NULL, screen, NULL);

        SDL_Rect wallRect = wall.rect;
        SDL_FillRect(screen, &wallRect, SDL_MapRGB(screen->format, 250, 250, 100));
        SDL_Rect wall2Rect = wall2.rect;
        SDL_FillRect(screen, &wall2Rect, SDL_MapRGB(screen->format, 0, 250, 0));

        if (level == 1){
            move_enemy_randomly(&shadow, level); 
        }
        if (level == 2){
            move_enemy_ai(&shadow, player_x, player_y, S1, S2);
        }
        else if(level == 3){
            move_enemy_randomly2(&shadow, level);
        }

        SDL_Rect enemy_rect = {shadow.x, shadow.y, 128, 128};
        if (check_collision_enemy_es(enemy_rect, wall.rect)) {
    // Calculate the reverse direction based on the enemy's movement
    shadow.x -= (shadow.dx * 50); // Step back in the opposite X-direction
    shadow.y -= (shadow.dy * 50); // Step back in the opposite Y-direction
}

if (check_collision_player_enemy(wall2.rect, &shadow)) {
    // Calculate the reverse direction based on the enemy's movement
    shadow.x -= (shadow.dx * 50); // Step back in the opposite X-direction
    shadow.y -= (shadow.dy * 50); // Step back in the opposite Y-direction
}
        animate_enemy_move(&shadow);
        display_enemy(&shadow, screen);

        SDL_Flip(screen);
        SDL_Delay(60);
    }

    // Free resources
    for (int i = 0; i < 4; i++) SDL_FreeSurface(shadow.frames[i]);
    SDL_FreeSurface(background);
    SDL_Quit();
    return 0;
}
