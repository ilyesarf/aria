#include "../include/game.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Camera follows the player, clamped to world bounds
void updateBackgroundCamera(Background *bg, const SDL_Rect *player, int screenWidth, int screenHeight, int margin) {
    // Target camera position - center on player
    int targetX = player->x + player->w / 2 - screenWidth / 2;
    int targetY = player->y + player->h / 2 - screenHeight / 2;

    // Clamp camera to world bounds
    if (targetX < 0) targetX = 0;
    if (targetY < 0) targetY = 0;
    if (targetX > bg->world_width - screenWidth)
        targetX = bg->world_width - screenWidth;
    if (targetY > bg->world_height - screenHeight)
        targetY = bg->world_height - screenHeight;

    // Update camera position
    bg->camera.x = targetX;
    bg->camera.y = targetY;
}