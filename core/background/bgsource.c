#include "../include/game.h"
#include "bgmain.h"
#include <stdio.h>
#include <stdlib.h>

void init_background(Background *bg, const char *image_path, int level) {
    bg->image = IMG_Load(image_path);
    if (!bg->image) {
        fprintf(stderr, "Failed to load background image '%s': %s\n", image_path, IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if (level == 1) {
        bg->platform_count = 3;
        bg->platforms = malloc(bg->platform_count * sizeof(Platform));
        bg->platforms[0] = (Platform){0, 0, 100, 20, 'F'};
        bg->platforms[1] = (Platform){150, 50, 100, 20, 'M'};
        bg->platforms[2] = (Platform){300, 100, 100, 20, 'D'};
    } else if (level == 2) {
        bg->platform_count = 5;
        bg->platforms = malloc(bg->platform_count * sizeof(Platform));
        for (int i = 0; i < bg->platform_count; i++) {
            bg->platforms[i] = (Platform){i*120, 200, 100, 20, 'F'};
        }
    } else {
        bg->platform_count = 0;
        bg->platforms = NULL;
    }
    bg->camera = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bg->world_width = bg->image->w;
    bg->world_height = bg->image->h;
}

void display_background(const Background *bg, SDL_Surface *screen) {
    SDL_BlitSurface(bg->image, &bg->camera, screen, NULL);
    // Optionally: draw platforms here
}

void free_background(Background *bg) {
    if (bg->image) {
        SDL_FreeSurface(bg->image);
        bg->image = NULL;
    }
    if (bg->platforms) {
        free(bg->platforms);
        bg->platforms = NULL;
    }
}

void scroll_background(Background *bg, int dx, int dy) {
    for (int i = 0; i < bg->platform_count; i++) {
        bg->platforms[i].x += dx;
        bg->platforms[i].y += dy;
    }
}