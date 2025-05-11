#include "enemy.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <time.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    IMG_Init(IMG_INIT_PNG);
    srand(time(NULL));

    // Load the background image
    SDL_Surface* background = load_background("core/enemy/lastlvl.png");

    Enemy shadow;
    init_enemy(&shadow, 100, 100, 100);

    StaticElement wall = { .rect = {300, 300, 100, 100} }; // Example obstacle

    int running = 1;
    SDL_Event event;
    int player_x = 400, player_y = 300;
    int level = 2;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        // Render the background
        render_background(background, screen);

        SDL_Rect wallRect = wall.rect;
        SDL_FillRect(screen, &wallRect, SDL_MapRGB(screen->format, 0, 250, 100));

        if (level == 1)
            move_enemy_randomly(&shadow, level);
        else
            move_enemy_ai(&shadow, player_x, player_y);

        SDL_Rect enemy_rect = {shadow.x, shadow.y, 128, 128};
        if (check_collision_enemy_es(enemy_rect, wall.rect)) {
            shadow.x -= 2; shadow.y -= 2; // Step back if collision
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