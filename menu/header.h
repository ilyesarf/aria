/**
 * @file header.h
 * @brief Menu system for the game
 *
 * This file contains the structures and functions for implementing
 * the various game menus, including state management and
 * button interactions.
 */

#ifndef HEADER_H
#define HEADER_H


#include "../helpers.h"

/** @brief Number of buttons per menu */
#define N_BTNS 4
/** @brief Total number of menus in the game */
#define N_MENUS 11

/** @brief Menu state for exiting the game */
#define QUIT_GAME -1
/** @brief Menu state for main gameplay */
#define MAIN_GAME 0
/** @brief Menu state for main menu */
#define MENU_PRINCIPAL 1
/** @brief Menu state for options menu */
#define MENU_OPTION 2
/** @brief Menu state for save menu */
#define MENU_SAVE 3
/** @brief Menu state for loading/creating save */
#define MENU_NEW_LOAD_SAVE 4
/** @brief Menu state for player menu */
#define MENU_PLAYER 5
/** @brief Menu state for player selection */
#define MENU_CHOOSE_PLAYER 6
/** @brief Menu state for highscores */
#define MENU_BEST_SCORE 7
/** @brief Menu state for enigma minigame */
#define MENU_ENIGME 8
/** @brief Menu state for quiz minigame */
#define MENU_QUIZ 9
/** @brief Menu state for puzzle minigame */
#define MENU_PUZZLE 10

/**
 * @struct Button
 * @brief Button structure for menu interactions
 */
typedef struct {
    SDL_Rect rect;       /**< Position and size of button */
    const char *text;    /**< Text displayed on button */
    int selected;        /**< Whether button is currently selected */
} Button;

/**
 * @struct Menu
 * @brief Menu structure with buttons and callback functions
 */
typedef struct Menu {
    int n_btns;                /**< Number of buttons in this menu */
    Button *buttons;           /**< Array of buttons */
    
    /**
     * @brief Initialize buttons for this menu
     * @param buttons Array of buttons to initialize
     */
    void (*init_buttons)(Button *buttons);
    
    /**
     * @brief Render the menu
     * @param background Background surface
     * @param butImage Button image
     * @param screen Surface to draw on
     * @param font Font to use for text
     * @param textColor Color for text
     * @param buttons Array of buttons
     * @param n_btns Number of buttons
     */
    void (*render)(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns);
    
    /**
     * @brief Handle input events for this menu
     * @param menuState Pointer to current menu state
     * @param save Game save data
     * @param event SDL event to handle
     * @param buttons Array of buttons
     * @param n_btns Number of buttons
     * @param hoverSound Sound to play on button hover
     */
    void (*handleEvent)(int *menuState, Save save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound);
} Menu;

/**
 * @brief Render text on screen
 * @param screen Surface to draw on
 * @param text Text to render
 * @param font Font to use
 * @param textColor Color for text
 * @param x X position
 * @param y Y position
 */
void renderText(SDL_Surface *screen, const char *text, TTF_Font *font, SDL_Color textColor, int x, int y);

/**
 * @brief Render a button on screen
 * @param screen Surface to draw on
 * @param butImage Button image
 * @param font Font to use for text
 * @param textColor Color for text
 * @param button Button to render
 */
void renderButton(SDL_Surface *screen, SDL_Surface *butImage, TTF_Font *font, SDL_Color textColor, Button button);

/**
 * @brief Initialize all menus
 * @param menus Array of menus
 */
void init_menus(Menu *menus);

/**
 * @brief Main menu handling function
 * @param screen Surface to draw on
 * @param background Background surface
 * @param font Font to use for text
 * @param textColor Color for text
 * @param butImage Button image
 * @param hoverSound Sound to play on button hover
 * @param musique Background music
 * @param menuState Current menu state
 * @param save Game save data
 * @param menus Array of menus
 */
void menu(SDL_Surface *screen, SDL_Surface *background, TTF_Font *font, SDL_Color textColor, SDL_Surface *butImage, Mix_Chunk *hoverSound, Mix_Music *musique, int menuState, Save save, Menu *menus);

/**
 * @brief Clean up menu resources
 * @param hoverSound Sound to free
 * @param musique Music to free
 * @param background Background surface to free
 * @param font Font to free
 * @param menus Menus to free
 */
void cleanup(Mix_Chunk *hoverSound, Mix_Music *musique, SDL_Surface *background, TTF_Font *font, Menu *menus);

#endif
