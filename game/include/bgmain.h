/**
 * @file bgmain.h
 * @brief Background handling and camera system
 *
 * This file provides functionality for managing game backgrounds,
 * camera positioning, and platform structures.
 */

#ifndef BGMAIN_H
#define BGMAIN_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/**
 * @struct Platform
 * @brief Structure representing platform elements in the game world
 */
typedef struct {
    int x, y;             /**< Position coordinates */
    int width, height;    /**< Dimensions */
    char type;            /**< 'F' = fixed, 'M' = mobile, 'D' = destructible */
} Platform;

/**
 * @struct Background
 * @brief Structure for managing the game background and camera
 */
typedef struct {
    Platform *platforms;     /**< Array of platforms */
    int platform_count;      /**< Number of platforms */
    SDL_Surface *image;      /**< Background image */
    SDL_Rect camera;         /**< Camera view rectangle */
    int world_width;         /**< Total world width */
    int world_height;        /**< Total world height */
} Background;

/**
 * @brief Initialize background
 * @param bg Pointer to background structure
 * @param image_path Path to background image file
 * @param level Current game level
 */
void init_background(Background *bg, const char *image_path, int level);

/**
 * @brief Display background on screen
 * @param bg Pointer to background structure
 * @param screen Surface to draw on
 */
void display_background(const Background *bg, SDL_Surface *screen);

/**
 * @brief Free background resources
 * @param bg Pointer to background structure
 */
void free_background(Background *bg);

/**
 * @brief Scroll background by given amounts
 * @param bg Pointer to background structure
 * @param dx Horizontal scroll amount
 * @param dy Vertical scroll amount
 */
void scroll_background(Background *bg, int dx, int dy);

/**
 * @brief Update camera position to follow player
 * @param bg Pointer to background structure
 * @param player Player position rectangle
 * @param screenWidth Width of screen
 * @param screenHeight Height of screen
 * @param margin Camera margin around player
 */
void updateBackgroundCamera(Background *bg, const SDL_Rect *player, int screenWidth, int screenHeight, int margin);

#endif