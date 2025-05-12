#include "header.h"
#include "background/bgmain.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define PLAYER_MOVE_SPEED 6

Enemy* initEnemies(int count) {
    Enemy* enemies = malloc(count * sizeof(Enemy));
    if (!enemies) {
        printf("Failed to allocate memory for enemies\n");
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        enemies[i].health = 100;
        enemies[i].x = rand() % 800;
        enemies[i].y = rand() % 600;
        enemies[i].current_frame = 0;
        
        // Load enemy animation frames
        for (int j = 0; j < 4; j++) {
            char filename[100];
            sprintf(filename, "core/enemy/shadowanim/shadowf%dt.png", j + 1);
            enemies[i].frames[j] = IMG_Load(filename);
            if (!enemies[i].frames[j]) {
                printf("Failed to load enemy frame %s: %s\n", filename, IMG_GetError());
            }
        }
    }
    return enemies;
}

Level* initLevel(const char* bgPath, int enemyCount) {
    Level* level = malloc(sizeof(Level));
    if (!level) {
        printf("Failed to allocate memory for level\n");
        return NULL;
    }

    // Initialize background
    level->background = IMG_Load(bgPath);
    if (!level->background) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        free(level);
        return NULL;
    }

    level->enemies = initEnemies(enemyCount);
    level->n = enemyCount;

    if (!level->enemies) {
        SDL_FreeSurface(level->background);
        free(level);
        return NULL;
    }

    return level;
}

Player* initPlayer(const char* spritePath) {
    Player* player = malloc(sizeof(Player));
    if (!player) {
        printf("Failed to allocate memory for player\n");
        return NULL;
    }

    player->img = IMG_Load(spritePath);
    if (!player->img) {
        printf("Failed to load player sprite: %s\n", IMG_GetError());
        free(player);
        return NULL;
    }

    player->pos = (SDL_Rect){100, 100, player->img->w, player->img->h};
    player->lives = 3;
    player->score = 0;
    player->velocity = 0;
    player->acceleration = 0;
    player->spd = PLAYER_MOVE_SPEED;
    player->state = 0;
    
    return player;
}

void updatePlayer(Player* player, Input* input, Background* bg) {
    if (input->right) {
        player->pos.x += player->spd;
    }
    if (input->left) {
        player->pos.x -= player->spd;
    }
    if (input->jump) {
        player->pos.y -= player->spd;
    }
    if (input->down) {
        player->pos.y += player->spd;
    }

    // Clamp player position to world bounds
    if (player->pos.x < 0) player->pos.x = 0;
    if (player->pos.y < 0) player->pos.y = 0;
    if (player->pos.x + player->pos.w > bg->world_width) 
        player->pos.x = bg->world_width - player->pos.w;
    if (player->pos.y + player->pos.h > bg->world_height) 
        player->pos.y = bg->world_height - player->pos.h;
}

void handleInput(Input* input) {
    Uint8* keystate = SDL_GetKeyState(NULL);
    input->right = keystate[SDLK_RIGHT];
    input->left = keystate[SDLK_LEFT];
    input->jump = keystate[SDLK_UP];
    input->down = keystate[SDLK_DOWN];
    input->escape = keystate[SDLK_ESCAPE];
}

void cleanupLevel(Level* level) {
    if (level) {
        if (level->background) {
            SDL_FreeSurface(level->background);
        }
        if (level->enemies) {
            for (int i = 0; i < level->n; i++) {
                for (int j = 0; j < 4; j++) {
                    if (level->enemies[i].frames[j]) {
                        SDL_FreeSurface(level->enemies[i].frames[j]);
                    }
                }
            }
            free(level->enemies);
        }
        free(level);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Failed to create screen: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize background
    Background bg;
    init_background(&bg, "core/background/lastlvl.png", 1);

    // Initialize level with enemies
    Level* level = initLevel("core/background/lastlvl.png", 5);
    if (!level) {
        printf("Failed to initialize level\n");
        return 1;
    }

    // Initialize player
    Player* player = initPlayer("core/joueur/1.png");
    if (!player) {
        printf("Failed to initialize player\n");
        cleanupLevel(level);
        return 1;
    }

    Input input = {0}; // Initialize input structure
    int running = 1;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Handle input
        handleInput(&input);
        if (input.escape) running = 0;

        // Update player and camera
        updatePlayer(player, &input, &bg);
        updateBackgroundCamera(&bg, &player->pos, SCREEN_WIDTH, SCREEN_HEIGHT, 100);

        // Clear screen
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Render background with camera
        display_background(&bg, screen);

        // Render player relative to camera
        SDL_Rect playerScreenPos = player->pos;
        playerScreenPos.x -= bg.camera.x;
        playerScreenPos.y -= bg.camera.y;
        SDL_BlitSurface(player->img, NULL, screen, &playerScreenPos);

        // Render enemies relative to camera
        for (int i = 0; i < level->n; i++) {
            SDL_Rect enemyPos = {
                level->enemies[i].x - bg.camera.x,
                level->enemies[i].y - bg.camera.y,
                level->enemies[i].frames[0]->w,
                level->enemies[i].frames[0]->h
            };
            SDL_BlitSurface(level->enemies[i].frames[level->enemies[i].current_frame], 
                           NULL, screen, &enemyPos);
            
            // Animate enemy
            level->enemies[i].current_frame = (level->enemies[i].current_frame + 1) % 4;
        }

        SDL_Flip(screen);
        SDL_Delay(16); // Cap at ~60 FPS
    }

    // Cleanup
    free_background(&bg);
    SDL_FreeSurface(player->img);
    free(player);
    cleanupLevel(level);
    SDL_Quit();

    return 0;
}