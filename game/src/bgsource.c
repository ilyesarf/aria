#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>

static SDL_Surface* platform_image = NULL;

void init_background(Background *bg, const char *image_path, int level) {
    bg->image = IMG_Load(image_path);
    if (!bg->image) {
        fprintf(stderr, "Failed to load background image '%s': %s\n", image_path, IMG_GetError());
        exit(EXIT_FAILURE);
    }
    
    // Load platform image
    if (!platform_image) {
        platform_image = IMG_Load("./game/assets/platforms/castleplatform.png");
        if (!platform_image) {
            fprintf(stderr, "Failed to load platform image: %s\n", IMG_GetError());
            exit(EXIT_FAILURE);
        }
    }
    
    if (level == 1) {
        bg->platform_count = 8; // More platforms for better gameplay
        bg->platforms = malloc(bg->platform_count * sizeof(Platform));
        
        // Create platforms at different positions across the world
        bg->platforms[0] = (Platform){200, 300, platform_image->w, platform_image->h, 'F'};
        bg->platforms[1] = (Platform){600, 250, platform_image->w, platform_image->h, 'F'};
        bg->platforms[2] = (Platform){1000, 350, platform_image->w, platform_image->h, 'F'};
        bg->platforms[3] = (Platform){1400, 200, platform_image->w, platform_image->h, 'F'};
        bg->platforms[4] = (Platform){1800, 300, platform_image->w, platform_image->h, 'F'};
        bg->platforms[5] = (Platform){2200, 250, platform_image->w, platform_image->h, 'F'};
        bg->platforms[6] = (Platform){2600, 400, platform_image->w, platform_image->h, 'F'};
        bg->platforms[7] = (Platform){3000, 350, platform_image->w, platform_image->h, 'F'};
        
    } else if (level == 2) {
        bg->platform_count = 5;
        bg->platforms = malloc(bg->platform_count * sizeof(Platform));
        for (int i = 0; i < bg->platform_count; i++) {
            bg->platforms[i] = (Platform){i*400, 200 + i*50, platform_image->w, platform_image->h, 'F'};
        }
    } else {
        bg->platform_count = 0;
        bg->platforms = NULL;
    }
    bg->camera = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bg->world_width = bg->image->w;
    bg->world_height = bg->image->h;
}

void display_background(Background *bg, SDL_Surface *screen) {
    // Draw the background image
    SDL_BlitSurface(bg->image, &bg->camera, screen, NULL);
    
    // Draw platforms
    for (int i = 0; i < bg->platform_count; i++) {
        // Only draw platforms that are within the visible area
        if (bg->platforms[i].x + bg->platforms[i].width >= bg->camera.x && 
            bg->platforms[i].x <= bg->camera.x + bg->camera.w &&
            bg->platforms[i].y + bg->platforms[i].height >= bg->camera.y && 
            bg->platforms[i].y <= bg->camera.y + bg->camera.h) {
            
            // Calculate platform's screen position
            SDL_Rect dst = {
                bg->platforms[i].x - bg->camera.x,
                bg->platforms[i].y - bg->camera.y,
                0, 0
            };
            
            // Draw the platform
            SDL_BlitSurface(platform_image, NULL, screen, &dst);
        }
    }
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
    if (platform_image) {
        SDL_FreeSurface(platform_image);
        platform_image = NULL;
    }
}

void scroll_background(Background *bg, int dx, int dy) {
    for (int i = 0; i < bg->platform_count; i++) {
        bg->platforms[i].x += dx;
        bg->platforms[i].y += dy;
    }
}