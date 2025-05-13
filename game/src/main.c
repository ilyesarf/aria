/**
 * @file main.c
 * @brief Main entry point for the Fantasy Forest game
 *
 * This file contains the main game loop and initialization logic for the game.
 * It handles SDL setup, game resource initialization, player/enemy creation,
 * input processing, and rendering.
 */

#include "../include/game.h"
#include "../include/minimap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/** @brief Number of enemies to create in the game world */
#define NUM_ENEMIES 3
/** @brief Damage dealt by enemy attacks */
#define ENEMY_DAMAGE 10

/**
 * @brief Main entry point for the game
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments
 * @return 0 on successful exit, -1 on error
 */
int main(int argc, char** argv) {
    (void)argc;  // Silence unused parameter warning
    (void)argv;  // Silence unused parameter warning

    // Initialize SDL and subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Create window
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    TTF_Init();
    TTF_Font *font;
    font = load_font("./assets/fonts/font.ttf");
    SDL_Color textColor = {255, 255, 255, 255}; // white text
    
    SDL_Surface *butImage = NULL;
    butImage = load_image("./assets/buttons/butbase.png");
    
    init_audio();
    Mix_Music *musique;
    Mix_Chunk *hoverSound;
    musique = load_music("./assets/music/30-hours.mp3");;
    hoverSound = load_sound("./assets/sounds/beep.wav"); 
    int volume = 50; // Initial volume (50%)

    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(volume); // Set initial volume


    Menu menus[N_MENUS];
    init_menus(menus);


        // Initialize random seed
    srand(time(NULL));

    // Initialize game resources
    if (!init_game_resources()) {
        printf("Failed to initialize game resources!\n");
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Initialize background
    Background background;
    init_background(&background, "./assets/game/fantasyforest.png", 1);  // Use fantasyforest background

    // Initialize game objects
    Player player;
    Enemy enemies[NUM_ENEMIES];
    Input input = {0}; // Initialize all input values to 0
    int jump_height = 150;
    int menuState = MAIN_GAME; // Start in main game state

    // Initialize minimap
    Minimap minimap;
    if (!initialiserMinimapAssets(&minimap)) {
        printf("Failed to initialize minimap!\n");
        return -1;
    }

    // Initialize player
    init_player(&player, "./game/assets/player/1.png", SCREEN_WIDTH / 4);
    player.pos.y = GROUND_Y - player.pos.h;
    
    // Initial camera update to ensure proper world view from the start
    updateBackgroundCamera(&background, &player.pos, SCREEN_WIDTH, SCREEN_HEIGHT, 100);
    
    // Initialize enemies with different behaviors spread across the world
    for (int i = 0; i < NUM_ENEMIES; i++) {
        // Spread enemies throughout the world, not just visible area
        int x = rand() % (SCREEN_WIDTH * 3 - 100);
        
        // Vary enemy heights throughout the screen
        int y = rand() % (SCREEN_HEIGHT - 200);
        
        // Make sure some enemies are near the player's starting area
        if (i == 0) {
            // First enemy within visible range of player start position
            x = player.pos.x + SCREEN_WIDTH/2 + rand() % 300;
            y = player.pos.y - 100 - rand() % 200; // Position above the player
        }
        
        init_enemy(&enemies[i], 100, x);
        enemies[i].y = y; // Set custom y position
    }

    // Initialize ball system
    init_balls();
    Save save;
    save.level.enemies = enemies;
    save.level.background = &background;
    //save.level.static_elements = NULL;
    save.players = &player;
    save.level.n = 1; //level 1

    menu(screen, background.image, font, textColor, butImage, hoverSound, musique, menuState, save, menus);


    // Game loop variables
    int running = 1;
    Uint32 last_time = SDL_GetTicks();
    Uint32 frame_time = 16; // Target 60 FPS

    while (running) {
        // Calculate delta time
        Uint32 current_time = SDL_GetTicks();
        Uint32 dt = current_time - last_time;
        last_time = current_time;

        // Handle input
        get_input(&input);
        printf("menuState: %d\n", menuState);
        // Handle global controls
        if (input.escape && menuState == MAIN_GAME) {
            menuState = MENU_SAVE;
            input.escape = 0;
            SDL_Delay(200); // Add a small delay to prevent immediate state toggle
        } else if (input.escape && menuState == MENU_SAVE) {
            menuState = MAIN_GAME;

            input.escape = 0;
            SDL_Delay(200); // Add a small delay to prevent immediate state toggle
        }

        if (player.pos.x >= background.world_width){
            menuState = MENU_ENIGME;
        } 

        if (menuState == MENU_BEST_SCORE) {
            if (input.space) {
                // Reset game
                player.health = PLAYER_MAX_HEALTH;
                player.lives = 3;
                player.score = 0;
                player.pos.x = 200;
                player.pos.y = 500;
                
                // Reset enemies
                for (int i = 0; i < NUM_ENEMIES; i++) {
                    int x = rand() % (SCREEN_WIDTH - 100);
                    init_enemy(&enemies[i], 100, x);
                }
                
                menuState = MAIN_GAME;
            } else if (input.q) {
                menu(screen, background.image, font, textColor, butImage, hoverSound, musique, menuState, save, menus);
            }
        } else if (menuState == MENU_SAVE || menuState == MENU_ENIGME) {
            menu(screen, background.image, font, textColor, butImage, hoverSound, musique, menuState, save, menus);
        }

        if (menuState == MAIN_GAME) {
            // Create ball when 'H' is pressed
            player = save.players[0];
            enemies[0] = save.level.enemies[0];
            enemies[1] = save.level.enemies[1];
            enemies[2] = save.level.enemies[2];

            background = *save.level.background;

            if (input.h) {
                create_ball(&player);
                input.h = 0;
            }

            // Update game state
            move_player(&input, &player, background, dt);
            jump_player(&input, &player, &jump_height);
            animate_player(&player, input);
            update_balls();

            // Update enemies and check collisions
            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (i % 2 == 0) {
                    move_enemy_randomly(&enemies[i], 1);
                } else {
                    move_enemy_ai(&enemies[i], player.pos.x, player.pos.y, 300, 100);
                }
                animate_enemy_move(&enemies[i]);

                // Check for enemy attacks
                if (check_collision_player_enemy(player.pos, &enemies[i]) && 
                    enemy_can_attack(&enemies[i], current_time)) {
                    update_player_health(&player, ENEMY_DAMAGE, current_time);
                    enemies[i].last_attack_time = current_time;
                }
            }

            // Check ball collisions with enemies
            check_ball_enemy_collisions(enemies, NUM_ENEMIES);

            // Update camera to follow player
            updateBackgroundCamera(&background, &player.pos, SCREEN_WIDTH, SCREEN_HEIGHT, 100);

            // Check game over condition
            if (player.health <= 0) {
                if (player.lives > 0) {
                    player.lives--;
                    player.health = PLAYER_MAX_HEALTH;
                } else {
                    menuState = MENU_BEST_SCORE;
                }
            }

            // Update minimap with player position
            MAJMinimap(player.pos, &minimap, 0.1f); // Scale factor of 0.1 for minimap

            // Clear screen and render
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

            // Display background
            display_background(&background, screen);

            // Display game objects
            display_balls(screen, &background.camera);
            display_player(screen, &player, &background.camera);
            
            for (int i = 0; i < NUM_ENEMIES; i++) {
                display_enemy(&enemies[i], screen, &background.camera);
            }

            // Display minimap
            dessinerJoueurMinimap(screen, &minimap);

            // Display HUD
            char score_text[32];
            sprintf(score_text, "Score: %d", player.score);
            SDL_Color white = {255, 255, 255, 255};
            render_text_centered(screen, score_text, font, white, 10);

            char lives_text[32];
            sprintf(lives_text, "Lives: %d", player.lives);
            render_text_centered(screen, lives_text, font, white, 40);

            char health_text[32];
            sprintf(health_text, "Health: %d", player.health);
            render_text_centered(screen, health_text, font, white, 70);
        }


        // Update screen
        SDL_Flip(screen);

        // Cap frame rate
        if (dt < frame_time) {
            SDL_Delay(frame_time - dt);
        }
    }

    // Cleanup
    cleanup_game_resources();
    free_player(&player);
    for (int i = 0; i < NUM_ENEMIES; i++) {
        free_enemy(&enemies[i]);
    }
    free_balls();
    free_background(&background);
    Mix_FreeChunk(hoverSound);
    Mix_FreeMusic(musique);
    TTF_CloseFont(font);
    SDL_FreeSurface(butImage);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    // Add minimap cleanup
    libererMinimapAssets(&minimap);

    return 0;
} 