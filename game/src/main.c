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

    // Initialize game resources
    if (!init_game_resources()) {
        printf("Failed to initialize game resources!\n");
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }
    
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
    Player players[2]; // Array to hold up to 2 players
    init_player(&players[0], "./game/assets/player/1.png", SCREEN_WIDTH / 4);
    players[0].pos.y = GROUND_Y - players[0].pos.h;
    
    // Initial camera update to ensure proper world view from the start
    updateBackgroundCamera(&background, &players[0].pos, SCREEN_WIDTH, SCREEN_HEIGHT, 100);
    
    // Initialize enemies with different behaviors spread across the world
    for (int i = 0; i < NUM_ENEMIES; i++) {
        // Spread enemies throughout the world, not just visible area
        int x = rand() % (SCREEN_WIDTH * 3 - 100);
        
        // Vary enemy heights throughout the screen
        int y = rand() % (SCREEN_HEIGHT - 200);
        
        // Make sure some enemies are near the player's starting area
        if (i == 0) {
            // First enemy within visible range of player start position
            x = players[0].pos.x + SCREEN_WIDTH/2 + rand() % 300;
            y = players[0].pos.y - 100 - rand() % 200; // Position above the player
        }
        
        init_enemy(&enemies[i], 100, x);
        enemies[i].y = y; // Set custom y position
    }

    // Initialize ball system
    init_balls();
    
    // Initialize save structure
    Save save;
    save.level.enemies = enemies;
    save.level.background = &background;
    save.players = players;
    save.num_players = 1; // Default to single player
    save.input_methods[0] = 0; // Default to keyboard for player 1
    save.input_methods[1] = 1; // Default to controller for player 2 (if used)
    save.level.n = 1; // Level 1
    
    // Check for existing save file
    if (fopen("savegame.dat", "rb")) {
       menuState = MENU_NEW_LOAD_SAVE;
       menu(screen, background.image, font, textColor, butImage, hoverSound, musique, &menuState, &save, menus);
    } else {
       // No save file, go to player selection menu
       menuState = MENU_PLAYER;
       menu(screen, background.image, font, textColor, butImage, hoverSound, musique, &menuState, &save, menus);
    }

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
        // Handle global controls
        if (input.escape && menuState == MAIN_GAME) {
            menuState = MENU_SAVE;
            printf("Entering menu save\n");
            input.escape = 0;
            SDL_Delay(200); // Add a small delay to prevent immediate state toggle
        } else if (input.escape && menuState == MENU_SAVE) {
            menuState = MAIN_GAME;

            input.escape = 0;
            SDL_Delay(200); // Add a small delay to prevent immediate state toggle
        }

        if (save.players[0].pos.x >= background.world_width){
            menuState = MENU_ENIGME;
        } 

        if (menuState == MENU_BEST_SCORE) {
            if (input.space) {
                // Reset game
                save.players[0].health = PLAYER_MAX_HEALTH;
                save.players[0].lives = 3;
                save.players[0].score = 0;
                save.players[0].pos.x = 200;
                save.players[0].pos.y = 500;
                
                // Reset enemies
                for (int i = 0; i < NUM_ENEMIES; i++) {
                    int x = rand() % (SCREEN_WIDTH - 100);
                    init_enemy(&enemies[i], 100, x);
                }
                
                menuState = MAIN_GAME;
            } else if (input.q) {
                menu(screen, background.image, font, textColor, butImage, hoverSound, musique, &menuState, &save, menus);
            }
        } else if (menuState == MENU_SAVE || menuState == MENU_ENIGME || menuState == MENU_BEST_SCORE) {
            menu(screen, background.image, font, textColor, butImage, hoverSound, musique, &menuState, &save, menus);
        }

        if (menuState == MAIN_GAME) {
            // Get active player(s) from save
            Player* player1 = &save.players[0];
            Player* player2 = (save.num_players > 1) ? &save.players[1] : NULL;
            
            // Create ball when 'H' is pressed
            if (input.h) {
                create_ball(player1);
                input.h = 0;
            }

            // Update game state for player 1
            move_player(&input, player1, background, dt);
            jump_player(&input, player1, &jump_height);
            animate_player(player1, input);
            
            // Update game state for player 2 if active
            if (player2) {
                // TODO: Add controller input handling for player 2
                // For now, use same input for testing
                move_player(&input, player2, background, dt);
                jump_player(&input, player2, &jump_height);
                animate_player(player2, input);
            }
            
            update_balls();

            // Update enemies and check collisions
            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (i % 2 == 0) {
                    move_enemy_randomly(&enemies[i], save.level.n, background.platforms, background.platform_count);
                } else {
                    move_enemy_ai(&enemies[i], player1->pos.x, player1->pos.y, 300, 100);
                }
                animate_enemy_move(&enemies[i]);

                // Check for enemy attacks against player 1
                if (check_collision_player_enemy(player1->pos, &enemies[i]) && 
                    enemy_can_attack(&enemies[i], current_time)) {
                    update_player_health(player1, ENEMY_DAMAGE, current_time);
                    enemies[i].last_attack_time = current_time;
                }
                
                // Check for enemy attacks against player 2 if active
                if (player2 && check_collision_player_enemy(player2->pos, &enemies[i]) && 
                    enemy_can_attack(&enemies[i], current_time)) {
                    update_player_health(player2, ENEMY_DAMAGE, current_time);
                    enemies[i].last_attack_time = current_time;
                }
            }

            // Check ball collisions with enemies
            check_ball_enemy_collisions(enemies, NUM_ENEMIES);
            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].health <= 0) {
                    player1->score += 100;
                    // Don't respawn enemy, just make it invisible
                    enemies[i].x = -1000;  // Move far off screen
                    enemies[i].y = -1000;
                }
            }

           for (int i = 0; i < background.platform_count; i++) {
                // Check platform collisions for player 1
                if (check_collision_with_platform(player1->pos, &background.platforms[i])) {
                    // If player is falling and hits platform from above
                    if (player1->vy > 0 && player1->pos.y + player1->pos.h > background.platforms[i].y) {
                        player1->pos.y = background.platforms[i].y - player1->pos.h;
                        player1->vy = 0;
                        player1->is_jumping = 0;
                    }
                }
                
                // Check platform collisions for player 2 if active
                if (player2 && check_collision_with_platform(player2->pos, &background.platforms[i])) {
                    // If player is falling and hits platform from above
                    if (player2->vy > 0 && player2->pos.y + player2->pos.h > background.platforms[i].y) {
                        player2->pos.y = background.platforms[i].y - player2->pos.h;
                        player2->vy = 0;
                        player2->is_jumping = 0;
                    }
                }
            } 

            // Update camera to follow player 1
            updateBackgroundCamera(&background, &player1->pos, SCREEN_WIDTH, SCREEN_HEIGHT, 100);

            // Check game over condition for player 1
            if (player1->health <= 0) {
                if (player1->lives > 0) {
                    player1->lives--;
                    player1->health = PLAYER_MAX_HEALTH;
                } else {
                    menuState = MENU_BEST_SCORE;
                }
            }
            
            // Check game over condition for player 2 if active
            if (player2 && player2->health <= 0) {
                if (player2->lives > 0) {
                    player2->lives--;
                    player2->health = PLAYER_MAX_HEALTH;
                } else if (player1->health <= 0) {
                    // Only go to game over if both players are out of lives
                    menuState = MENU_BEST_SCORE;
                }
            }

            // Update minimap with player position
            MAJMinimap(player1->pos, &minimap, 0.05f); // Reduced scale factor to make minimap smaller

            // Clear screen and render
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

            // Display background
            display_background(&background, screen);

            // Display game objects
            display_balls(screen, &background.camera);
            
            if (save.num_players == 1) {
                // Single player mode - full screen
                display_player(screen, player1, &background.camera);
            } else {
                // Split screen mode for two players
                // Define split screen areas
                SDL_Rect topHalf = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/2};
                SDL_Rect bottomHalf = {0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2};
                
                // Draw dividing line
                SDL_Rect divider = {0, SCREEN_HEIGHT/2 - 2, SCREEN_WIDTH, 4};
                SDL_FillRect(screen, &divider, SDL_MapRGB(screen->format, 255, 255, 255));
                
                // TODO: Implement proper split screen rendering with separate cameras
                // For now, just display both players on the same screen
                display_player(screen, player1, &background.camera);
                display_player(screen, player2, &background.camera);
                
                // Comment to mark variables as intentionally unused until split screen is fully implemented
                (void)topHalf;
                (void)bottomHalf;
            }
            
            for (int i = 0; i < NUM_ENEMIES; i++) {
                display_enemy(&enemies[i], screen, &background.camera);
            }

            // Display minimap
            dessinerJoueurMinimap(screen, &minimap);

            // Display HUD
            char score_text[32];
            sprintf(score_text, "Score: %d", player1->score);
            SDL_Color white = {255, 255, 255, 255};
            render_text_centered(screen, score_text, font, white, 10);

            char lives_text[32];
            sprintf(lives_text, "Lives: %d", player1->lives);
            render_text_centered(screen, lives_text, font, white, 40);

            char health_text[32];
            sprintf(health_text, "Health: %d", player1->health);
            render_text_centered(screen, health_text, font, white, 70);
            
            // Display player 2 HUD if active
            if (player2) {
                sprintf(score_text, "P2 Score: %d", player2->score);
                render_text_centered(screen, score_text, font, white, SCREEN_HEIGHT - 70);
                
                sprintf(lives_text, "P2 Lives: %d", player2->lives);
                render_text_centered(screen, lives_text, font, white, SCREEN_HEIGHT - 40);
                
                sprintf(health_text, "P2 Health: %d", player2->health);
                render_text_centered(screen, health_text, font, white, SCREEN_HEIGHT - 10);
            }
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
    
    // Free players
    for (int i = 0; i < save.num_players; i++) {
        free_player(&save.players[i]);
    }
    
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