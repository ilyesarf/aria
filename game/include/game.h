/**
 * @file game.h
 * @brief Core game mechanics and data structures
 * 
 * This file contains definitions for all core game components including
 * player, enemies, backgrounds, game states, and related functions for
 * a 2D platformer game.
 */

#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "../helpers.h"
#include "../menu/header.h"

/** @brief Ground level (pixels from top) */
#define GROUND_Y (SCREEN_HEIGHT - 10)
/** @brief Gravity acceleration constant */
#define GRAVITY 0.5f
/** @brief Initial jump velocity */
#define JUMP_FORCE -15.0f
/** @brief Maximum falling speed */
#define MAX_FALL_SPEED 15.0f
/** @brief Maximum number of projectiles */
#define MAX_BALLS 10
/** @brief Movement speed of projectiles */
#define BALL_SPEED 8
/** @brief Width of ball projectiles */
#define BALL_WIDTH 30
/** @brief Height of ball projectiles */
#define BALL_HEIGHT 30
/** @brief Maximum player health */
#define PLAYER_MAX_HEALTH 100
/** @brief Milliseconds between damage events */
#define DAMAGE_COOLDOWN 1000
/** @brief Initial speed for projectiles */
#define BALL_INITIAL_SPEED 15.0f
/** @brief Lifetime of projectiles in milliseconds */
#define BALL_LIFETIME 2000

/**
 * @struct Platform
 * @brief Platform structure for background
 */
typedef struct {
    int x, y;            /**< Position coordinates */
    int w, h;   /**< Dimensions */
    char type;           /**< 'F' = fixed, 'M' = mobile, 'D' = destructible */
} Platform;

/**
 * @struct Background
 * @brief Background structure with platforms and camera
 */
typedef struct Background {
    Platform *platforms;    /**< Array of platforms */
    int platform_count;     /**< Number of platforms */
    SDL_Surface *image;     /**< Background image */
    SDL_Rect camera;        /**< Camera view rectangle */
    int world_width;        /**< Total world width */
    int world_height;       /**< Total world height */
} Background;

/**
 * @enum GameState
 * @brief Game state enumeration
 */
typedef enum {
    GAME_STATE_MENU,       /**< Menu screen active */
    GAME_STATE_PLAYING,    /**< Gameplay active */
    GAME_STATE_PAUSED,     /**< Game paused */
    GAME_STATE_GAME_OVER   /**< Game over screen active */
} GameState;

/**
 * @enum EnemyState
 * @brief Enemy behavior states
 */
typedef enum {
    WAITING,     /**< Idle state, minimal movement */
    FOLLOWING,   /**< Pursuing player */
    ATTACKING    /**< Performing attack */
} EnemyState;

/**
 * @struct Enemy
 * @brief Enemy character data structure
 */
typedef struct Enemy {
    int health;                 /**< Health points */
    float x, y;                 /**< Position coordinates */
    SDL_Surface* frames[4];     /**< Animation frames */
    int current_frame;          /**< Current animation frame */
    EnemyState state;           /**< Current behavior state */
    float dx, dy;               /**< Movement velocities */
    float patrol_start_x;       /**< Starting point for patrol route */
    int patrol_direction;       /**< Direction of patrol movement */
    Uint32 last_attack_time;    /**< Time of last attack */
    Uint32 attack_cooldown;     /**< Time between attacks */
    int frame_counter;          /**< Animation frame counter */
} Enemy;

/**
 * @struct Ball
 * @brief Projectile data structure
 */
typedef struct {
    SDL_Rect pos;           /**< Position and size */
    float dx, dy;           /**< Movement velocities */
    int active;             /**< Whether ball is active */
    Uint32 creation_time;   /**< Time when created */
    int radius;             /**< Ball radius */
} Ball;

/**
 * @struct Input
 * @brief Player input state
 */
typedef struct {
    // Movement controls
    int left, right, up, down;  /**< Directional movement */
    int jump;                   /**< Jump action */
    int accel;                  /**< Acceleration */
    
    // Action controls
    int h;                      /**< Shoot ball */
    int space;                  /**< Restart game */
    int escape;                 /**< Pause game */
    int q;                      /**< Quit game */
} Input;

/**
 * @struct Image
 * @brief Image with two positions for parallax effects
 */
typedef struct {
    SDL_Rect pos1;          /**< First position */
    SDL_Rect pos2;          /**< Second position */
    SDL_Surface* img;       /**< Image surface */
} Image;

/* Function declarations */

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
void display_background(Background *bg, SDL_Surface *screen);

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

/**
 * @brief Initialize enemy
 * @param enemy Pointer to enemy structure
 * @param health Initial health points
 * @param x Initial x position
 */
void init_enemy(Enemy *enemy, int health, int x);

/**
 * @brief Display enemy on screen
 * @param enemy Pointer to enemy structure
 * @param screen Surface to draw on
 * @param camera Camera view rectangle
 */
void display_enemy(Enemy *enemy, SDL_Surface* screen, const SDL_Rect* camera);

/**
 * @brief Draw health bar for enemy
 * @param screen Surface to draw on
 * @param enemy Pointer to enemy structure
 */
void draw_enemy_health_bar(SDL_Surface* screen, Enemy* enemy);

/**
 * @brief Animate enemy movement
 * @param enemy Pointer to enemy structure
 */
void animate_enemy_move(Enemy *enemy);

/**
 * @brief Move enemy randomly
 * @param enemy Pointer to enemy structure
 * @param level Current game level (affects behavior)
 */
void move_enemy_randomly(Enemy *enemy, int level, Platform platforms[], int num_platforms);

/**
 * @brief Move enemy using AI to follow player
 * @param enemy Pointer to enemy structure
 * @param player_x Player x position
 * @param player_y Player y position
 * @param vision_range Distance enemy can see player
 * @param attack_range Distance enemy can attack player
 */
void move_enemy_ai(Enemy *enemy, int player_x, int player_y, int vision_range, int attack_range);

/**
 * @brief Check for collision between player and enemy
 * @param player_rect Player position rectangle
 * @param enemy Pointer to enemy structure
 * @return 1 if collision, 0 otherwise
 */
int check_collision_player_enemy(SDL_Rect player_rect, Enemy *enemy);

/**
 * @brief Update enemy health after taking damage
 * @param enemy Pointer to enemy structure
 * @param damage Amount of damage to apply
 */
void update_enemy_health(Enemy *enemy, int damage);

/**
 * @brief Initialize player
 * @param player Pointer to player structure
 * @param sprite_path Path to player sprite image
 * @param x Initial x position
 */
void init_player(Player* player, const char* sprite_path, int x);

/**
 * @brief Display player on screen
 * @param screen Surface to draw on
 * @param player Pointer to player structure
 * @param camera Camera view rectangle
 */
void display_player(SDL_Surface* screen, Player* player, const SDL_Rect* camera);

/**
 * @brief Move player based on input
 * @param input Pointer to input structure
 * @param player Pointer to player structure
 * @param background Background structure for collision checking
 * @param dt Delta time in milliseconds
 */
void move_player(Input* input, Player* player, Background background, Uint32 dt);

/**
 * @brief Handle player jumping
 * @param input Pointer to input structure
 * @param player Pointer to player structure
 * @param jump_height Pointer to jump height value
 */
void jump_player(Input* input, Player* player, int* jump_height);

/**
 * @brief Animate player based on input
 * @param player Pointer to player structure
 * @param input Input structure for movement state
 */
void animate_player(Player* player, Input input);

/**
 * @brief Draw health bar for player
 * @param screen Surface to draw on
 * @param player Pointer to player structure
 */
void draw_player_health_bar(SDL_Surface* screen, Player* player);

/**
 * @brief Update player health after taking damage
 * @param player Pointer to player structure
 * @param damage Amount of damage to apply
 * @param current_time Current game time for damage cooldown
 */
void update_player_health(Player* player, int damage, Uint32 current_time);

/**
 * @brief Initialize ball projectile system
 */
void init_balls(void);

/**
 * @brief Create a new ball projectile fired by player
 * @param player Pointer to player structure
 */
void create_ball(Player* player);

/**
 * @brief Update all active balls
 */
void update_balls(void);

/**
 * @brief Display all active balls on screen
 * @param screen Surface to draw on
 * @param camera Camera view rectangle
 */
void display_balls(SDL_Surface* screen, const SDL_Rect* camera);

/**
 * @brief Free ball system resources
 */
void free_balls(void);

/**
 * @brief Get player input state
 * @param input Pointer to input structure to update
 */
void get_input(Input* input);

/**
 * @brief Free player resources
 * @param player Pointer to player structure
 */
void free_player(Player* player);

/**
 * @brief Free enemy resources
 * @param enemy Pointer to enemy structure
 */
void free_enemy(Enemy* enemy);

/**
 * @brief Initialize all game resources
 * @return 1 on success, 0 on failure
 */
int init_game_resources(void);

/**
 * @brief Cleanup all game resources
 */
void cleanup_game_resources(void);

/**
 * @brief Check if enemy can attack based on cooldown
 * @param enemy Pointer to enemy structure
 * @param current_time Current game time
 * @return 1 if can attack, 0 otherwise
 */
int enemy_can_attack(Enemy* enemy, Uint32 current_time);

/**
 * @brief Check for collision between ball and enemy
 * @param ball_rect Ball position rectangle
 * @param enemy Pointer to enemy structure
 * @return 1 if collision, 0 otherwise
 */
int check_collision_ball_enemy(SDL_Rect ball_rect, Enemy *enemy);

/**
 * @brief Check for collisions between all balls and enemies
 * @param enemies Array of enemies
 * @param num_enemies Number of enemies in array
 */
void check_ball_enemy_collisions(Enemy enemies[], int num_enemies);

int check_collision_with_platform(SDL_Rect object_rect, Platform* platform);

    /**
 * @brief Render text centered horizontally
 * @param screen Surface to draw on
 * @param text Text to render
 * @param font Font to use
 * @param color Text color
 * @param y Vertical position
 */
void render_text_centered(SDL_Surface* screen, const char* text, TTF_Font* font, SDL_Color color, int y);

/**
 * @brief Display pause menu
 * @param screen Surface to draw on
 */
void display_pause_menu(SDL_Surface* screen);

/**
 * @brief Display game over screen
 * @param screen Surface to draw on
 * @param score Final score to display
 */
void display_game_over(SDL_Surface* screen, int score);

#endif // GAME_H 