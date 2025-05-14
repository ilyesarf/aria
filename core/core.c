#include "header.h"
#include "joueur/player.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define PLAYER_MOVE_SPEED 6


Level* initLevel(int n, const char* bgPath, int enemyCount, int staticElementCount) {
    Level* level = malloc(sizeof(Level));
    if (!level) {
        printf("Failed to allocate memory for level\n");
        return NULL;
    }
    level->n = n;
    // Initialize background
    init_background(&level->background, bgPath, n);
    Background *bg = &level->background;
    // Initialize Enemies
    if (n == 1) {//level 1

        bg->platform_count = 3;
        bg->platforms = malloc(bg->platform_count * sizeof(Platform));
        bg->platforms[0] = (Platform){0, 0, 100, 20, 'F'};
        bg->platforms[1] = (Platform){150, 50, 100, 20, 'M'};
        bg->platforms[2] = (Platform){300, 100, 100, 20, 'D'};
    } else if (n == 2){ //level 2
        bg->platform_count = 5;
        bg->platforms = malloc(bg->platform_count * sizeof(Platform));
        bg->platforms[0] = (Platform){0, 0, 100, 20, 'F'};
        bg->platforms[1] = (Platform){150, 50, 100, 20, 'M'};
        bg->platforms[2] = (Platform){300, 100, 100, 20, 'D'};
        bg->platforms[3] = (Platform){450, 150, 100, 20, 'F'};
        bg->platforms[4] = (Platform){600, 200, 100, 20, 'M'};

    }
    // Initialize Static Element

    return level;
}



void cleanupLevel(Level* level) {
    if (level) {
        if (level->background.image) {
            SDL_FreeSurface(level->background.image);
        }
        if (level->enemies) {
            for (int i = 0; i < level->n; i++) {
                for (int j = 0; j < 4; j++) {
                    SDL_FreeSurface(level->enemies[i].frames[j]);
                }
            }
            free(level->enemies);
        }
        if (level->static_elements) {
            free(level->static_elements);
        }
        free(level);
    }
}

void core(SDL_Surface *screen) {
    // Initialize level (example: level 1, 3 enemies, 5 static elements)
    Level* level = initLevel(1, "./assets/game/background.png", 3, 5);
    if (!level) {
        printf("Failed to initialize level\n");
        return;
    }
    // Initialize joueur player system
    character player;
    initCharacter(&player);
    int jump_height = 150;
    in input = {0};
    int running = 1;
    Uint32 dt = 5, t_prev = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }
        get_input(&input, 0); // single player mode
        if (input.escape) running = 0;
        // Animate and move player
        animation(&player, input);
        jump_character(&input, &player, &jump_height);
        move_character(&input, &player, NULL, dt);
        // Camera follows player
        level->background.camera.x = player.pos.x - SCREEN_WIDTH / 2;
        if (level->background.camera.x < 0) level->background.camera.x = 0;
        if (level->background.camera.x > level->background.world_width - SCREEN_WIDTH)
            level->background.camera.x = level->background.world_width - SCREEN_WIDTH;
        // Render background
        display_background(&level->background, screen);
        // Render static elements
        if (level->static_elements) {
            for (int i = 0; i < 5; i++) {
                SDL_Rect r = level->static_elements[i].rect;
                r.x -= level->background.camera.x;
                r.y -= level->background.camera.y;
                SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 200, 200, 200));
            }
        }
        // Render enemies
        if (level->enemies) {
            for (int i = 0; i < level->n; i++) {
                SDL_Rect enemyRect = {level->enemies[i].x - level->background.camera.x,
                                      level->enemies[i].y - level->background.camera.y,
                                      64, 64};
                SDL_BlitSurface(level->enemies[i].frames[0], NULL, screen, &enemyRect);
            }
        }
        // Render player relative to camera
        character player_screen = player;
        player_screen.pos.x -= level->background.camera.x;
        player_screen.pos.y -= level->background.camera.y;
        display_player(screen, &player_screen);
        SDL_Flip(screen);
        SDL_Delay(16);
        dt = SDL_GetTicks() - t_prev;
        t_prev = SDL_GetTicks();
    }
    SDL_FreeSurface(player.img);
    cleanupLevel(level);
}