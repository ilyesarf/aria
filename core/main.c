#include "header.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define PLAYER_MOVE_SPEED 6

Background initBackground(const char* imagePath, int worldWidth, int worldHeight) {
    Background bg;
    bg.image = IMG_Load(imagePath);
    if (!bg.image) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        bg.world_width = 0;
        bg.world_height = 0;
        bg.camera = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        return bg;
    }
    bg.world_width = worldWidth;
    bg.world_height = worldHeight;
    bg.camera = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    return bg;
}

Enemy* initEnemies(int count) {
    Enemy* enemies = malloc(count * sizeof(Enemy));
    if (!enemies) {
        printf("Failed to allocate memory for enemies\n");
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        enemies[i].health = 100;
        enemies[i].x = rand() % 800; // Random initial position
        enemies[i].y = rand() % 600;
        enemies[i].current_frame = 0;
        
        // Load enemy animation frames
        for (int j = 0; j < 4; j++) {
            char filename[100];
            sprintf(filename, "./core/enemy/shadowanim/shadowf%dt.png", j + 1); // shadowf1t.png, shadowf2t.png, etc.
            enemies[i].frames[j] = IMG_Load(filename);
            if (!enemies[i].frames[j]) {
                printf("Failed to load enemy frame: %s\n", IMG_GetError());
            }
        }
    }
    return enemies;
}

StaticElement* initStaticElements(int count) {
    StaticElement* elements = malloc(count * sizeof(StaticElement));
    if (!elements) {
        printf("Failed to allocate memory for static elements\n");
        return NULL;
    }

    // Initialize static elements with some default positions
    for (int i = 0; i < count; i++) {
        elements[i].rect = (SDL_Rect){
            rand() % 800,  // x position
            rand() % 600,  // y position
            32,           // width
            32            // height
        };
    }
    return elements;
}

Level* initLevel(const char* bgPath, int enemyCount, int staticElementCount) {
    Level* level = malloc(sizeof(Level));
    if (!level) {
        printf("Failed to allocate memory for level\n");
        return NULL;
    }

    level->background = initBackground(bgPath, 2000, 1000); // Not a pointer!
    if (!level->background.image) {
        free(level);
        return NULL;
    }
    level->enemies = initEnemies(enemyCount);
    level->static_elements = initStaticElements(staticElementCount);
    level->n = enemyCount;

    if (!level->enemies || !level->static_elements) {
        if (level->background.image) SDL_FreeSurface(level->background.image);
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

void updatePlayer(Player* player, Input* input) {
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
    if (player->pos.x + player->pos.w > 2000) player->pos.x = 2000 - player->pos.w;
    if (player->pos.y + player->pos.h > 1000) player->pos.y = 1000 - player->pos.h;
}

void handleInput(Input* input) {
    Uint8* keystate = SDL_GetKeyState(NULL);
    input->right = keystate[SDLK_RIGHT];
    input->left = keystate[SDLK_LEFT];
    input->jump = keystate[SDLK_UP];
    input->down = keystate[SDLK_DOWN];
    input->escape = keystate[SDLK_ESCAPE];
}

void renderBackground(SDL_Surface* screen, Background* bg, int playerX) {
    bg->camera.x = playerX - (SCREEN_WIDTH / 2);
    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.x > bg->world_width - SCREEN_WIDTH)
        bg->camera.x = bg->world_width - SCREEN_WIDTH;
    SDL_BlitSurface(bg->image, &bg->camera, screen, NULL);
}

void cleanupLevel(Level* level) {
    if (level) {
        if (level->background.image) {
            SDL_FreeSurface(level->background.image);
        }
        if (level->enemies) {
            for (int i = 0; i < level->n; i++) {
                for (int j = 0; j < 4; j++) {
                    SDL_FreeSurface(level->enemies[i].frames[j]);
                }
            }
            free(level->enemies);
        }
        if (level->static_elements) {
            free(level->static_elements);
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

    // Initialize level with background
    Level* level = initLevel("./assets/game/background.png", 5, 10);
    if (!level) {
        printf("Failed to initialize level\n");
        return 1;
    }

    // Initialize player
    Player* player = initPlayer("./assets/player/player.png");
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

        // Update player
        updatePlayer(player, &input);

        // Render background with camera following player
        renderBackground(screen, &level->background, player->pos.x);

        // Render player relative to camera position
        SDL_Rect playerScreenPos = player->pos;
        playerScreenPos.x -= level->background.camera.x;
        playerScreenPos.y -= level->background.camera.y;
        SDL_BlitSurface(player->img, NULL, screen, &playerScreenPos);

        SDL_Flip(screen);
        SDL_Delay(16); // Cap at ~60 FPS
    }

    // Cleanup
    SDL_FreeSurface(player->img);
    free(player);
    cleanupLevel(level);
    SDL_Quit();

    return 0;
}