#include "../include/game.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_ttf.h>

// Global ball array
static Ball balls[MAX_BALLS];

// Global variables for resource management
static SDL_Surface* enemy_frames[4] = {NULL, NULL, NULL, NULL};

// Physics constants
#define GRAVITY 0.5f
#define JUMP_FORCE -15.0f
#define MAX_FALL_SPEED 15.0f
//#define GROUND_Y 460  // Adjust to align with the visual ground in the background image

// Enemy behavior constants
#define PATROL_SPEED 2
#define CHASE_SPEED 4
#define ATTACK_SPEED 6
#define VISION_RANGE 300
#define ATTACK_RANGE 50
#define PATROL_RANGE 150
#define ENEMY_TURN_RATE 0.1f

// Ball physics constants
#define BALL_GRAVITY 0.2f
//#define BALL_INITIAL_SPEED 12.0f
#define BALL_LIFETIME 2000 // milliseconds


// Background system is now handled in bgsource.c

// Initialize game resources
int init_game_resources(void) {
    // Load shared enemy frames
    const char* enemy_frame_paths[] = {
        "./game/assets/enemy/shadowf1t.png",
        "./game/assets/enemy/shadowf2t.png",
        "./game/assets/enemy/shadowf3t.png",
        "./game/assets/enemy/shadowf4t.png"
    };

    for (int i = 0; i < 4; i++) {
        enemy_frames[i] = IMG_Load(enemy_frame_paths[i]);
        if (!enemy_frames[i]) {
            printf("Failed to load enemy frame %d: %s\n", i, SDL_GetError());
            // Cleanup previously loaded frames
            for (int j = 0; j < i; j++) {
                SDL_FreeSurface(enemy_frames[j]);
                enemy_frames[j] = NULL;
            }
            return 0;
        }
    }
    return 1;
}

// Cleanup game resources
void cleanup_game_resources(void) {
    for (int i = 0; i < 4; i++) {
        if (enemy_frames[i]) {
            SDL_FreeSurface(enemy_frames[i]);
            enemy_frames[i] = NULL;
        }
    }
}

// Enemy implementation
void init_enemy(Enemy *enemy, int health, int x) {
    enemy->health = health;
    enemy->x = x;
    enemy->y = GROUND_Y - 153;  // Default ground level position
    enemy->current_frame = 0;
    enemy->state = WAITING;
    enemy->dx = 0;
    enemy->dy = 0;
    enemy->patrol_start_x = x;
    enemy->patrol_direction = 1;
    enemy->last_attack_time = 0;
    enemy->attack_cooldown = 1000;
    enemy->frame_counter = 0;  // Initialize frame counter

    // Use shared enemy frames
    for (int i = 0; i < 4; i++) {
        enemy->frames[i] = enemy_frames[i];
    }
}

void display_enemy(Enemy *enemy, SDL_Surface* screen, const SDL_Rect* camera) {
    if (enemy->health > 0) {
        // Calculate enemy's screen position
        SDL_Rect dstrect = {
            .x = enemy->x - camera->x,
            .y = enemy->y - camera->y,
            .w = 0, .h = 0
        };
        
        // Only draw if enemy is on screen
        if (dstrect.x + enemy->frames[0]->w >= 0 && dstrect.x <= SCREEN_WIDTH &&
            dstrect.y + enemy->frames[0]->h >= 0 && dstrect.y <= SCREEN_HEIGHT) {
            
            SDL_BlitSurface(enemy->frames[enemy->current_frame], NULL, screen, &dstrect);
            
            // Draw health bar above enemy
            SDL_Rect bg = { dstrect.x, dstrect.y - 10, 50, 5 };
            SDL_Rect fg = { dstrect.x, dstrect.y - 10, (enemy->health * 50) / 100, 5 };
            SDL_FillRect(screen, &bg, SDL_MapRGB(screen->format, 255, 0, 0));
            SDL_FillRect(screen, &fg, SDL_MapRGB(screen->format, 0, 255, 0));
        }
    }
}

void animate_enemy_move(Enemy *enemy) {
    // Only update animation every 15 frames (makes it about 4 times slower)
    enemy->frame_counter++;
    if (enemy->frame_counter >= 15) {
        enemy->current_frame = (enemy->current_frame + 1) % 4;
        enemy->frame_counter = 0;
    }
}

void move_enemy_randomly(Enemy *enemy, int level) {
    // Only move if enemy has health
    if (enemy->health <= 0) {
        return;
    }

    (void)level;
    static Uint32 last_dir_change = 0;
    const Uint32 dir_change_interval = 1000;
    
    if (enemy->dx == 0 && enemy->dy == 0) {
        enemy->dx = (rand() % 2) ? 1 : -1;
        enemy->dy = (rand() % 2) ? 1 : -1;
    }
    
    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_dir_change > dir_change_interval) {
        if (rand() % 4 == 0) enemy->dx = (rand() % 2) ? 1 : -1;
        if (rand() % 4 == 0) enemy->dy = (rand() % 2) ? 1 : -1;
        last_dir_change = current_time;
    }
    
    int move_speed = 3; // Slightly slower for better control
    enemy->x += enemy->dx * move_speed;
    enemy->y += enemy->dy * move_speed;
    
    // World boundary checks
    if (enemy->x < 0) {
        enemy->x = 0;
        enemy->dx = 1;
    }
    if (enemy->y < 0) {
        enemy->y = 0;
        enemy->dy = 1;
    }
    if (enemy->x > SCREEN_WIDTH * 3 - enemy->frames[0]->w) { // Use extended world width
        enemy->x = SCREEN_WIDTH * 3 - enemy->frames[0]->w;
        enemy->dx = -1;
    }
    if (enemy->y > SCREEN_HEIGHT - enemy->frames[0]->h) {
        enemy->y = SCREEN_HEIGHT - enemy->frames[0]->h;
        enemy->dy = -1;
    }
    
    // Remove ground level restrictions to allow free movement
}

void move_enemy_ai(Enemy *enemy, int player_x, int player_y, int vision_range, int attack_range) {
    if (enemy->health <= 0) return;

    float dx = player_x - enemy->x;
    float dy = player_y - enemy->y;
    float distance = sqrt(dx * dx + dy * dy);
    
    // Update enemy state based on distance
    switch (enemy->state) {
        case WAITING:
            if (distance <= vision_range) {
                enemy->state = FOLLOWING;
                enemy->patrol_direction = (dx > 0) ? 1 : -1;
            } else {
                // Patrol behavior
                enemy->x += enemy->patrol_direction * PATROL_SPEED;
                
                // Reverse direction at patrol limits or world boundaries
                if (fabs(enemy->x - enemy->patrol_start_x) > PATROL_RANGE || 
                    enemy->x <= 0 || 
                    enemy->x >= SCREEN_WIDTH * 3 - enemy->frames[0]->w) {
                    
                    enemy->patrol_direction *= -1;
                }
                
                // Random vertical movement during patrol
                if (rand() % 100 < 5) { // 5% chance to change vertical direction
                    enemy->dy = (rand() % 3) - 1; // -1, 0, or 1
                    enemy->y += enemy->dy * PATROL_SPEED;
                    
                    // Keep within screen height
                    if (enemy->y < 0) {
                        enemy->y = 0;
                        enemy->dy = 1;
                    } else if (enemy->y > SCREEN_HEIGHT - enemy->frames[0]->h) {
                        enemy->y = SCREEN_HEIGHT - enemy->frames[0]->h;
                        enemy->dy = -1;
                    }
                }
            }
            break;

        case FOLLOWING:
            if (distance <= attack_range) {
                enemy->state = ATTACKING;
            } else if (distance > vision_range * 1.5) { // Give more leeway before losing sight
                enemy->state = WAITING;
            } else {
                // Smooth pursuit movement
                float angle = atan2(dy, dx);
                float new_x = enemy->x + cos(angle) * CHASE_SPEED;
                float new_y = enemy->y + sin(angle) * CHASE_SPEED;
                
                // Check world boundaries
                if (new_x >= 0 && new_x <= SCREEN_WIDTH * 3 - enemy->frames[0]->w) {
                    enemy->x = new_x;
                }
                
                // Allow full vertical movement within screen bounds
                if (new_y >= 0 && new_y <= SCREEN_HEIGHT - enemy->frames[0]->h) {
                    enemy->y = new_y;
                }
            }
            break;

        case ATTACKING:
            if (distance > attack_range * 1.2) { // Give a bit more range before switching back
                enemy->state = FOLLOWING;
            } else {
                // Aggressive movement
                float angle = atan2(dy, dx);
                float new_x = enemy->x + cos(angle) * ATTACK_SPEED;
                float new_y = enemy->y + sin(angle) * ATTACK_SPEED;
                
                // Check world boundaries
                if (new_x >= 0 && new_x <= SCREEN_WIDTH * 3 - enemy->frames[0]->w) {
                    enemy->x = new_x;
                }
                
                // Allow full vertical movement within screen bounds
                if (new_y >= 0 && new_y <= SCREEN_HEIGHT - enemy->frames[0]->h) {
                    enemy->y = new_y;
                }
            }
            break;
    }
}

int check_collision_player_enemy(SDL_Rect player_rect, Enemy *enemy) {
    SDL_Rect enemy_rect = { 
        enemy->x, 
        enemy->y, 
        enemy->frames[0]->w,
        enemy->frames[0]->h
    };
    
    if (player_rect.x + player_rect.w < enemy_rect.x ||
        player_rect.x > enemy_rect.x + enemy_rect.w ||
        player_rect.y + player_rect.h < enemy_rect.y ||
        player_rect.y > enemy_rect.y + enemy_rect.h) {
        return 0;
    }
    return 1;
}

void update_enemy_health(Enemy *enemy, int damage) {
    enemy->health -= damage;
    if (enemy->health < 0) {
        enemy->health = 0;
    }
}

// Player implementation
void init_player(Player* player, const char* sprite_path, int x) {
    player->img = IMG_Load(sprite_path);
    if (player->img == NULL) {
        printf("Unable to load player image: %s\n", SDL_GetError());
        exit(1);
    }
    player->velocity = 5;
    player->acc = 0;
    player->health = PLAYER_MAX_HEALTH;
    player->lives = 3;
    player->score = 0;
    player->pos.x = x;
    player->pos.y = GROUND_Y - player->pos.h;  // Place right above ground level
    player->pos.w = 90;
    player->pos.h = 153;
    player->spd = 0;
    player->last_damage_time = 0;
    player->vy = 0;
    player->is_jumping = 0;
    player->facing_left = 0;

    player->posSprite.x = 0;
    player->posSprite.y = 0;
    player->posSprite.w = 90;
    player->posSprite.h = 153;
}

void display_player(SDL_Surface* screen, Player* player, const SDL_Rect* camera) {
    // Calculate player's screen position
    SDL_Rect screen_pos = {
        .x = player->pos.x - camera->x,
        .y = player->pos.y - camera->y,
        .w = player->pos.w,
        .h = player->pos.h
    };
    
    // Draw player
    SDL_BlitSurface(player->img, &player->posSprite, screen, &screen_pos);
    
    // Draw health bar above player
    SDL_Rect bg = { screen_pos.x, screen_pos.y - 20, player->pos.w, 10 };
    SDL_Rect fg = { screen_pos.x, screen_pos.y - 20, (player->health * player->pos.w) / PLAYER_MAX_HEALTH, 10 };
    SDL_FillRect(screen, &bg, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_FillRect(screen, &fg, SDL_MapRGB(screen->format, 0, 255, 0));
}

void update_player_health(Player* player, int damage, Uint32 current_time) {
    // Check if enough time has passed since last damage
    if (current_time - player->last_damage_time >= DAMAGE_COOLDOWN) {
        player->health -= damage;
        player->last_damage_time = current_time;
        
        // Ensure health doesn't go below 0
        if (player->health < 0) {
            player->health = 0;
        }
    }
}

void move_player(Input* input, Player* player, Uint32 dt) {
    float delta = dt / 16.667f; // Normalize for 60 FPS
    float movement_speed = player->velocity * delta;
    
    // Apply acceleration
    if (input->accel) {
        player->acc = fmin(player->acc + 0.2f * delta, 2.0f);
    } else {
        player->acc = fmax(player->acc - 0.1f * delta, 0.0f);
    }
    
    movement_speed *= (1.0f + player->acc);

    // Handle horizontal movement - allow moving across the entire world
    if (input->left && player->pos.x >= 10) {
        player->pos.x -= movement_speed;
        player->facing_left = 1;
    }
    if (input->right && player->pos.x <= SCREEN_WIDTH * 3) { // Extended boundary for wide backgrounds
        player->pos.x += movement_speed;
        player->facing_left = 0;
    }

    // Apply air resistance/friction
    if (!input->left && !input->right) {
        player->velocity = fmax(player->velocity - 0.1f * delta, 5.0f);
    } else {
        player->velocity = fmin(player->velocity + 0.2f * delta, 8.0f);
    }
}

void jump_player(Input* input, Player* player, int* jump_height) {
    (void)jump_height; // Silence unused parameter warning
    
    // Apply gravity
    player->vy += GRAVITY;
    
    // Clamp fall speed
    if (player->vy > MAX_FALL_SPEED) {
        player->vy = MAX_FALL_SPEED;
    }
    
    // Update vertical position
    player->pos.y += player->vy;
    
    // Ground collision
    if (player->pos.y >= GROUND_Y - player->pos.h) {
        player->pos.y = GROUND_Y - player->pos.h;
        player->vy = 0;
        player->is_jumping = 0;
    }
    
    // Jump input
    if (input->jump && !player->is_jumping && player->pos.y >= GROUND_Y - player->pos.h) {
        player->vy = JUMP_FORCE;
        player->is_jumping = 1;
    }
}

void animate_player(Player* player, Input input) {
    // Update sprite row based on state
    if (input.left && input.jump) {
        player->posSprite.y = 630; // Jumping left
    } else if (input.right && input.jump) {
        player->posSprite.y = 470; // Jumping right
    } else if (input.right) {
        player->posSprite.y = 312; // Running right
    } else if (input.left) {
        player->posSprite.y = 156; // Running left
    } else {
        player->posSprite.y = 0;   // Standing
    }

    // Update sprite column for animation
    if (player->posSprite.x >= 615) {
        player->posSprite.x = 0;
    } else {
        player->posSprite.x += 90;
    }
}

// Ball system implementation
void init_balls(void) {
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i].active = 0;
        balls[i].radius = BALL_WIDTH / 2;
    }
}

void create_ball(Player* player) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) {
            // Position the ball at the player's hand/front based on facing direction
            if (player->facing_left) {
                balls[i].pos.x = player->pos.x; // Left side of player
            } else {
                balls[i].pos.x = player->pos.x + player->pos.w; // Right side of player
            }
            
            balls[i].pos.y = player->pos.y + player->pos.h/3; // Approximately at hand height
            balls[i].pos.w = BALL_WIDTH * 1.5; // Make ball bigger
            balls[i].pos.h = BALL_HEIGHT * 1.5;
            balls[i].radius = (BALL_WIDTH * 1.5) / 2;
            
            // Calculate initial velocity based on player direction - make it faster
            float angle = player->facing_left ? M_PI : 0;
            balls[i].dx = cos(angle) * (BALL_INITIAL_SPEED * 1.5);
            balls[i].dy = 0.0f; // Remove vertical movement
            
            balls[i].active = 1;
            balls[i].creation_time = SDL_GetTicks();
            break;
        }
    }
}

void update_balls(void) {
    Uint32 current_time = SDL_GetTicks();
    
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            // Update position without gravity
            balls[i].pos.x += balls[i].dx;
            balls[i].pos.y += balls[i].dy;
            
            // Check lifetime
            if (current_time - balls[i].creation_time > BALL_LIFETIME) {
                balls[i].active = 0;
                continue;
            }
            
            // Check world bounds instead of screen bounds
            // Balls should only deactivate if they leave the world, not just the screen
            if (balls[i].pos.x < 0 || balls[i].pos.x > SCREEN_WIDTH * 3 ||
                balls[i].pos.y < 0 || balls[i].pos.y > SCREEN_HEIGHT) {
                balls[i].active = 0;
            }
        }
    }
}

void display_balls(SDL_Surface* screen, const SDL_Rect* camera) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            // Calculate ball's screen position
            SDL_Rect ball_screen_pos = {
                .x = balls[i].pos.x - camera->x,
                .y = balls[i].pos.y - camera->y,
                .w = balls[i].pos.w,
                .h = balls[i].pos.h
            };
            
            // Only draw if ball is on screen
            if (ball_screen_pos.x + balls[i].pos.w >= 0 && ball_screen_pos.x <= SCREEN_WIDTH &&
                ball_screen_pos.y + balls[i].pos.h >= 0 && ball_screen_pos.y <= SCREEN_HEIGHT) {
                
                int radius = balls[i].radius;
                int centerX = ball_screen_pos.x + radius;
                int centerY = ball_screen_pos.y + radius;
                
                // Use a brighter color for better visibility - bright red
                Uint32 ballColor = SDL_MapRGB(screen->format, 255, 0, 0);
                
                // Draw a filled circle
                for (int dy = -radius; dy <= radius; dy++) {
                    int dx = sqrt(radius * radius - dy * dy);
                    SDL_Rect line = {
                        centerX - dx,
                        centerY + dy,
                        dx * 2,
                        1
                    };
                    SDL_FillRect(screen, &line, ballColor);
                }
                
                // Add a white center for a glowing effect
                int innerRadius = radius / 3;
                Uint32 innerColor = SDL_MapRGB(screen->format, 255, 255, 255);
                for (int dy = -innerRadius; dy <= innerRadius; dy++) {
                    int dx = sqrt(innerRadius * innerRadius - dy * dy);
                    SDL_Rect line = {
                        centerX - dx,
                        centerY + dy,
                        dx * 2,
                        1
                    };
                    SDL_FillRect(screen, &line, innerColor);
                }
            }
        }
    }
}

void free_balls(void) {
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i].active = 0;
    }
}

// Input implementation
void get_input(Input* input) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                input->q = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: input->escape = 1; break;
                    case SDLK_LEFT: input->left = 1; break;
                    case SDLK_RIGHT: input->right = 1; break;
                    case SDLK_UP: input->up = 1; input->jump = 1; break;
                    case SDLK_DOWN: input->down = 1; break;
                    case SDLK_RSHIFT: input->accel = 1; break;
                    case SDLK_h: input->h = 1; break;
                    case SDLK_SPACE: input->space = 1; break;
                    case SDLK_q: input->q = 1; break;
                    default: break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: input->escape = 0; break;
                    case SDLK_LEFT: input->left = 0; break;
                    case SDLK_RIGHT: input->right = 0; break;
                    case SDLK_UP: input->up = 0; input->jump = 0; break;
                    case SDLK_DOWN: input->down = 0; break;
                    case SDLK_RSHIFT: input->accel = 0; break;
                    case SDLK_h: input->h = 0; break;
                    case SDLK_SPACE: input->space = 0; break;
                    case SDLK_q: input->q = 0; break;
                    default: break;
                }
                break;
        }
    }
}

// Cleanup functions
void free_player(Player* player) {
    if (player->img) {
        SDL_FreeSurface(player->img);
        player->img = NULL;
    }
}

void free_enemy(Enemy* enemy) {
    // No need to free frames as they're shared resources
    for (int i = 0; i < 4; i++) {
        enemy->frames[i] = NULL;
    }
}

// Add new function to check ball-enemy collision
int check_collision_ball_enemy(SDL_Rect ball_rect, Enemy *enemy) {
    SDL_Rect enemy_rect = { 
        enemy->x, 
        enemy->y, 
        enemy->frames[0]->w,
        enemy->frames[0]->h
    };
    
    if (ball_rect.x + ball_rect.w < enemy_rect.x ||
        ball_rect.x > enemy_rect.x + enemy_rect.w ||
        ball_rect.y + ball_rect.h < enemy_rect.y ||
        ball_rect.y > enemy_rect.y + enemy_rect.h) {
        return 0;
    }
    return 1;
}

// Add new function to check and handle ball collisions with enemies
void check_ball_enemy_collisions(Enemy enemies[], int num_enemies) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            for (int j = 0; j < num_enemies; j++) {
                if (enemies[j].health > 0 && check_collision_ball_enemy(balls[i].pos, &enemies[j])) {
                    // Ball hits enemy - increased damage from 20 to 50 for more effective attacks
                    update_enemy_health(&enemies[j], 50);
                    
                    // Visual feedback - push the enemy back slightly in the direction of the ball
                    if (balls[i].dx > 0) {
                        enemies[j].x += 20; // Push right
                    } else {
                        enemies[j].x -= 20; // Push left
                    }
                    
                    balls[i].active = 0; // Deactivate the ball
                    break;
                }
            }
        }
    }
}

// Add new function for enemy attacks
int enemy_can_attack(Enemy* enemy, Uint32 current_time) {
    return (current_time - enemy->last_attack_time >= enemy->attack_cooldown);
}

void render_text_centered(SDL_Surface* screen, const char* text, TTF_Font* font, SDL_Color color, int y) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color);
    if (text_surface) {
        SDL_Rect dest = {
            (SCREEN_WIDTH - text_surface->w) / 2,
            y,
            text_surface->w,
            text_surface->h
        };
        SDL_BlitSurface(text_surface, NULL, screen, &dest);
        SDL_FreeSurface(text_surface);
    }
}

void display_pause_menu(SDL_Surface* screen) {
    // Create semi-transparent overlay
    SDL_Surface* overlay = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, 128));
    SDL_SetAlpha(overlay, SDL_SRCALPHA, 128);
    SDL_BlitSurface(overlay, NULL, screen, NULL);
    SDL_FreeSurface(overlay);

    // Initialize TTF if not already initialized
    if (!TTF_WasInit() && TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("assets/fonts/font.ttf", 48);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    
    // Render pause menu text
    render_text_centered(screen, "GAME PAUSED", font, white, SCREEN_HEIGHT/2 - 100);
    render_text_centered(screen, "Press ESC to resume", font, white, SCREEN_HEIGHT/2);
    render_text_centered(screen, "Press Q to quit", font, white, SCREEN_HEIGHT/2 + 100);

    TTF_CloseFont(font);
}

void display_game_over(SDL_Surface* screen, int score) {
    // Create dark overlay
    SDL_Surface* overlay = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, 192));
    SDL_SetAlpha(overlay, SDL_SRCALPHA, 192);
    SDL_BlitSurface(overlay, NULL, screen, NULL);
    SDL_FreeSurface(overlay);

    // Initialize TTF if not already initialized
    if (!TTF_WasInit() && TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return;
    }

    // Load fonts with different sizes
    TTF_Font* large_font = TTF_OpenFont("assets/fonts/font.ttf", 96);  // Increased size
    TTF_Font* small_font = TTF_OpenFont("assets/fonts/font.ttf", 48);  // Increased size
    
    if (!large_font || !small_font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    
    // Render game over text with large font and red color
    render_text_centered(screen, "GAME OVER", large_font, red, SCREEN_HEIGHT/2 - 150);
    
    // Render score with small font
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Final Score: %d", score);
    render_text_centered(screen, score_text, small_font, white, SCREEN_HEIGHT/2);
    
    // Only show quit option
    render_text_centered(screen, "Press Q to quit", small_font, white, SCREEN_HEIGHT/2 + 100);

    TTF_CloseFont(large_font);
    TTF_CloseFont(small_font);
} 