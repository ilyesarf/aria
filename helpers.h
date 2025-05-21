/**
 * @file helpers.h
 * @brief Helper utilities and common structures for the game
 *
 * This file contains utility functions and common data structures
 * used throughout the game, including screen, audio, and resource
 * management functions.
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

/** @brief Width of the game screen */
#define SCREEN_WIDTH 1280
/** @brief Height of the game screen */
#define SCREEN_HEIGHT 512

// Forward declarations
struct Enemy;
struct Background;

/**
 * @struct Player
 * @brief Player character data structure
 */
typedef struct Player {
    SDL_Surface* img;             /**< Player sprite image */
    SDL_Rect pos;                 /**< Position and size */
    int health;                   /**< Current health points */
    int lives;                    /**< Remaining lives */
    int score;                    /**< Current score */
    float velocity;               /**< Movement velocity */
    float spd;                    /**< Speed multiplier */
    SDL_Rect posSprite;           /**< Position in sprite sheet */
    int state;                    /**< Current player state */
    float acc;                    /**< Acceleration */
    Uint32 last_damage_time;      /**< Time of last damage taken */
    float vy;                     /**< Vertical velocity */
    int is_jumping;               /**< Whether player is jumping */
    int facing_left;              /**< Direction player is facing */
} Player;

/**
 * @struct Level
 * @brief Game level data structure
 */
typedef struct Level {
    int n;                        /**< Level number */
    struct Background *background;/**< Level background */
    struct Enemy *enemies;        /**< Array of enemies in level */
} Level;

/**
 * @struct Save
 * @brief Game save data structure
 */
typedef struct Save {
    Player *players;              /**< Player data array (can be single player or array of 2) */
    int num_players;              /**< Number of active players (1 or 2) */
    int input_methods[2];         /**< Input method for each player: 0=keyboard, 1=controller */
    Level level;                  /**< Level data */
} Save;

/**
 * @brief Initialize the game screen
 * @return Initialized SDL surface
 */
SDL_Surface* init_screen();

/**
 * @brief Initialize audio subsystem
 */
void init_audio();

/**
 * @brief Load an image from file
 * @param filename Path to image file
 * @return Loaded SDL surface
 */
SDL_Surface* load_image(char *filename);

/**
 * @brief Load a sound effect from file
 * @param filename Path to sound file
 * @return Loaded sound chunk
 */
Mix_Chunk* load_sound(char *filename);

/**
 * @brief Load a font from file
 * @param filename Path to font file
 * @return Loaded font
 */
TTF_Font* load_font(char *filename);

/**
 * @brief Load music from file
 * @param filename Path to music file
 * @return Loaded music
 */
Mix_Music* load_music(const char* filename);

#endif // HELPERS_H 