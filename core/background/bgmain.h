#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct {
    int x, y; // Position
    int width, height; // Dimensions
    char type; // 'F' = fixe, 'M' = mobile, 'D' = destructible
} Platform;

typedef struct {
    Platform *platforms;
    int platform_count;
    SDL_Surface *image; // Background image
} Background;

void init_background(Background *bg, const char *image_path, int level);
void display_background(const Background *bg, SDL_Surface *screen);
void free_background(Background *bg);
void scroll_background(Background *bg, int dx, int dy);

#endif