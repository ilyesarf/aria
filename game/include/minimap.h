/**
 * @file minimap.h
 * @brief Minimap functionality for the game
 *
 * This file contains the structures and functions for implementing
 * a minimap display in the game, showing player position relative
 * to the game world.
 */

#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL/SDL.h>

/** @brief Width of player representation on minimap */
#define MINIMAP_PLAYER_WIDTH 10
/** @brief Height of player representation on minimap */
#define MINIMAP_PLAYER_HEIGHT 10
/** @brief Color for player representation on minimap (green) */
#define MINIMAP_PLAYER_COLOR 0x00FF00

/**
 * @struct Minimap
 * @brief Structure for minimap display and functionality
 */
typedef struct {
    SDL_Surface *fond;           /**< Background image for minimap */
    SDL_Rect position;           /**< Position of minimap on screen */
    SDL_Rect joueurPosition;     /**< Position of player marker on minimap */
} Minimap;

/**
 * @brief Initialize minimap assets
 * @param m Pointer to minimap structure
 * @return 1 on success, 0 on failure
 */
int initialiserMinimapAssets(Minimap *m);

/**
 * @brief Update minimap with current player position
 * @param posJoueurAbs Absolute player position in game world
 * @param m Pointer to minimap structure
 * @param redimensionnement Scale factor for converting world coordinates to minimap
 */
void MAJMinimap(SDL_Rect posJoueurAbs, Minimap *m, float redimensionnement);

/**
 * @brief Free minimap resources
 * @param m Pointer to minimap structure
 */
void libererMinimapAssets(Minimap *m);

/**
 * @brief Draw player marker on minimap
 * @param screen Surface to draw on
 * @param m Pointer to minimap structure
 */
void dessinerJoueurMinimap(SDL_Surface *screen, Minimap *m);

#endif
