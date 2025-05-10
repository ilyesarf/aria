#include "bgmain.h"

void init_background(Background *bg, const char *image_path, int level) {
    bg->image = IMG_Load(image_path);
    if (!bg->image) {
        fprintf(stderr, "Failed to load background image '%s': %s\n", image_path, IMG_GetError());
        exit(EXIT_FAILURE);
    }
    bg->platforms = malloc(bg->platform_count * sizeof(Platform));
    if (!bg->platforms) {
        fprintf(stderr, "Failed to allocate memory for platforms\n");
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
        if (!bg->platforms) {
            fprintf(stderr, "Failed to allocate memory for platforms\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < bg->platform_count; i++) {
            bg->platforms[i] = (Platform){0, 0, 100, 20, 'F'}; // Example initialization
        }
    }
}

void display_background(const Background *bg, SDL_Surface *screen) {
    SDL_Rect dest = {0, 0, 0, 0};
    SDL_BlitSurface(bg->image, NULL, screen, &dest);

    for (int i = 0; i < bg->platform_count; i++) {
        printf("Platform %d: Type=%c, Position=(%d, %d)\n",
               i, bg->platforms[i].type, bg->platforms[i].x, bg->platforms[i].y);
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
}

void scroll_background(Background *bg, int dx, int dy) {
    for (int i = 0; i < bg->platform_count; i++) {
        bg->platforms[i].x += dx;
        bg->platforms[i].y += dy;
    }
}