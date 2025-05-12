#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ENEMIES 3
#define ENEMY_DAMAGE 10

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

    // Load background
    SDL_Surface* background = load_background("assets/lastlvl.png");
    if (!background) {
        cleanup_game_resources();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Initialize game objects
    Player player;
    Enemy enemies[NUM_ENEMIES];
    Input input = {0}; // Initialize all input values to 0
    int jump_height = 150;
    GameState game_state = GAME_STATE_PLAYING;

    // Initialize player
    init_player(&player, "assets/player/1.png", 200);  // Y position is set in init_player

    // Initialize enemies with different behaviors
    for (int i = 0; i < NUM_ENEMIES; i++) {
        int x = rand() % (SCREEN_WIDTH - 100);
        int y = rand() % (SCREEN_HEIGHT - 100);
        init_enemy(&enemies[i], 100, x, y);
    }

    // Initialize ball system
    init_balls();

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
        if (input.escape && game_state != GAME_STATE_GAME_OVER) {
            game_state = (game_state == GAME_STATE_PLAYING) ? 
                        GAME_STATE_PAUSED : GAME_STATE_PLAYING;
            input.escape = 0;
        }
        
        if (game_state == GAME_STATE_GAME_OVER) {
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
                    int y = rand() % (SCREEN_HEIGHT - 100);
                    init_enemy(&enemies[i], 100, x, y);
                }
                
                game_state = GAME_STATE_PLAYING;
            } else if (input.q) {
                running = 0;
            }
        } else if (game_state == GAME_STATE_PAUSED) {
            if (input.q) {
                running = 0;
            }
        }

        if (game_state == GAME_STATE_PLAYING) {
            // Create ball when 'H' is pressed
            if (input.h) {
                create_ball(&player);
                input.h = 0;
            }

            // Update game state
            move_player(&input, &player, dt);
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
                    
                    // Check for game over
                    if (player.health <= 0) {
                        game_state = GAME_STATE_GAME_OVER;
                        player.pos.w = 0;  // Make player invisible
                        player.pos.h = 0;
                    }
                }
            }

            // Check ball-enemy collisions
            check_ball_enemy_collisions(enemies, NUM_ENEMIES);

            // Update score based on defeated enemies
            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].health <= 0) {
                    player.score += 100;
                    // Don't respawn enemy, just make it invisible
                    enemies[i].x = -1000;  // Move far off screen
                    enemies[i].y = -1000;
                }
            }
        }

        // Render game
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        display_background(screen, background);
        
        // Only display player if alive
        if (player.health > 0) {
            display_player(screen, &player);
        }
        display_balls(screen);
        
        for (int i = 0; i < NUM_ENEMIES; i++) {
            if (enemies[i].health > 0) {  // Only display living enemies
                display_enemy(&enemies[i], screen);
            }
        }

        // Display game state overlays
        if (game_state == GAME_STATE_PAUSED) {
            display_pause_menu(screen);
        } else if (game_state == GAME_STATE_GAME_OVER) {
            display_game_over(screen, player.score);
        }

        SDL_Flip(screen);

        // Cap frame rate
        Uint32 frame_duration = SDL_GetTicks() - current_time;
        if (frame_duration < frame_time) {
            SDL_Delay(frame_time - frame_duration);
        }
    }

    // Cleanup
    SDL_FreeSurface(background);
    free_player(&player);
    for (int i = 0; i < NUM_ENEMIES; i++) {
        free_enemy(&enemies[i]);
    }
    free_balls();
    cleanup_game_resources();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
} 