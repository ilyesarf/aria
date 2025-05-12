#ifndef BGMAIN_H
#define BGMAIN_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct {
    int x, y; // Position
    int width, height; // Dimensions
    char type; // 'F' = fixed, 'M' = mobile, 'D' = destructible
} Platform;

typedef struct {
    Platform *platforms;
    int platform_count;
    SDL_Surface *image; // Background image
    SDL_Rect camera;
    int world_width;
    int world_height;
} Background;

// Background and camera functions
void init_background(Background *bg, const char *image_path, int level);
void display_background(const Background *bg, SDL_Surface *screen);
void free_background(Background *bg);
void scroll_background(Background *bg, int dx, int dy);
void updateBackgroundCamera(Background *bg, const SDL_Rect *player, int screenWidth, int screenHeight, int margin);

#endif