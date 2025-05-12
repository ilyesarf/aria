#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
 
#include "minimap.h"
#include "background.h"
#include "personnage.h"

#define SCREEN_WIDTH        1280  
#define SCREEN_HEIGHT       960  
#define PLAYER_MOVE_SPEED   6    
#define MINIMAP_SCREEN_X_OFFSET 20
#define MINIMAP_SCREEN_Y_OFFSET 20
#define MINIMAP_WIDTH 400
#define MINIMAP_HEIGHT 100
#define MINIMAP_PLAYER_ICON_PATH "joueur_minimap.png"

#define MINIMAP_BG_L1_PATH "minimap_bg_scaled_L1.png"
#define MAIN_BG_L1_PATH "background_L1.png"

Minimap minimap_data;
Background background_obj;
Personnage personnage;
SDL_Surface* player_sprite_main = NULL;

int chargerNiveau(int levelNum, Background *bg, Minimap *m, Personnage *j,
                  const char* minimapBgPathL1,
                  const char* mainBgPathL1);

void mettreAJourCamera(Background *bg, const Personnage *joueur, int screenWidth);
void afficherBackground(SDL_Surface *screen, const Background *bg);
void afficherJoueur(SDL_Surface *screen, const Personnage *perso);
void MAJMinimapXY(SDL_Rect posJoueurAbs, Minimap *m, float scaleX, float scaleY);

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Minimap Test", NULL);

    player_sprite_main = IMG_Load("player_sprite.png");
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);

    memset(&minimap_data, 0, sizeof(Minimap));
    initialiserMinimapAssets(&minimap_data, MINIMAP_PLAYER_ICON_PATH);

    chargerNiveau(1, &background_obj, &minimap_data, &personnage,
                  MINIMAP_BG_L1_PATH,
                  MAIN_BG_L1_PATH);

    float scaleX = (float)MINIMAP_WIDTH / background_obj.world_width;
    float scaleY = (float)MINIMAP_HEIGHT / background_obj.world_height;

    // Update player size to match image
    personnage.position.w = player_sprite_main->w;
    personnage.position.h = player_sprite_main->h;

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }

        Uint8 *keystate = SDL_GetKeyState(NULL);
        personnage.vx = personnage.vy = 0;
        if (keystate[SDLK_LEFT]) personnage.vx = -PLAYER_MOVE_SPEED;
        if (keystate[SDLK_RIGHT]) personnage.vx = PLAYER_MOVE_SPEED;
        if (keystate[SDLK_UP]) personnage.vy = -PLAYER_MOVE_SPEED;
        if (keystate[SDLK_DOWN]) personnage.vy = PLAYER_MOVE_SPEED;

        personnage.position.x += personnage.vx;
        personnage.position.y += personnage.vy;

        // Clamp player to world bounds
        if (personnage.position.x < 0) personnage.position.x = 0;
        if (personnage.position.y < 0) personnage.position.y = 0;
        if (personnage.position.x + personnage.position.w > 6500)
            personnage.position.x = 6500- personnage.position.w;
        if (personnage.position.y + personnage.position.h > background_obj.world_height)
            personnage.position.y = background_obj.world_height - personnage.position.h;

        updateBackgroundCamera(&background_obj, &personnage, SCREEN_WIDTH, SCREEN_HEIGHT, 200);
        MAJMinimapXY(personnage.position, &minimap_data, scaleX, scaleY);

        afficherBackground(screen, &background_obj);

        SDL_Rect pos = {MINIMAP_SCREEN_X_OFFSET, MINIMAP_SCREEN_Y_OFFSET, 0, 0};
        SDL_BlitSurface(minimap_data.fond, NULL, screen, &pos);

        SDL_Rect p = minimap_data.joueurPosition;
        p.x += MINIMAP_SCREEN_X_OFFSET;
        p.y += MINIMAP_SCREEN_Y_OFFSET;

        if (p.x < MINIMAP_SCREEN_X_OFFSET) p.x = MINIMAP_SCREEN_X_OFFSET;
        if (p.y < MINIMAP_SCREEN_Y_OFFSET) p.y = MINIMAP_SCREEN_Y_OFFSET;
        if (p.x + p.w > MINIMAP_SCREEN_X_OFFSET + MINIMAP_WIDTH)
            p.x = MINIMAP_SCREEN_X_OFFSET + MINIMAP_WIDTH - p.w;
        if (p.y + p.h > MINIMAP_SCREEN_Y_OFFSET + MINIMAP_HEIGHT)
            p.y = MINIMAP_SCREEN_Y_OFFSET + MINIMAP_HEIGHT - p.h;

        SDL_BlitSurface(minimap_data.joueurImage, NULL, screen, &p);

        afficherJoueur(screen, &personnage);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(player_sprite_main);
    if (minimap_data.joueurImage) SDL_FreeSurface(minimap_data.joueurImage);
    if (minimap_data.fond) SDL_FreeSurface(minimap_data.fond);
    if (font) TTF_CloseFont(font);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

int chargerNiveau(int levelNum, Background *bg, Minimap *m, Personnage *j,
                  const char* minimapBgPathL1,
                  const char* mainBgPathL1) {
    bg->image = IMG_Load(mainBgPathL1);
    if (!bg->image) return 0;

    m->fond = IMG_Load(minimapBgPathL1);
    if (!m->fond) return 0;

    bg->camera = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bg->world_width = 8000;
    bg->world_height = 1080;
    j->position = (SDL_Rect){100, 100, 64, 64};
    return 1;
}

// Moves the camera with the player, but stops scrolling when the player reaches the edge of the background image
void updateBackgroundCamera(Background *bg, const Personnage *player, int screenWidth, int screenHeight, int margin) {
    // Center camera on player with margin
    int targetX = player->position.x + player->position.w / 2 - bg->camera.w / 2;
    int targetY = player->position.y + player->position.h / 2 - bg->camera.h / 2;

    // Clamp camera to world bounds
    if (targetX < 0) targetX = 0;
    if (targetY < 0) targetY = 0;
    if (targetX > bg->world_width - bg->camera.w)
        targetX = bg->world_width - bg->camera.w;
    if (targetY > bg->world_height - bg->camera.h)
        targetY = bg->world_height - bg->camera.h;

    bg->camera.x = targetX;
    bg->camera.y = targetY;
}

void afficherBackground(SDL_Surface *screen, const Background *bg) {
    SDL_BlitSurface(bg->image, &bg->camera, screen, NULL);
}

void afficherJoueur(SDL_Surface *screen, const Personnage *perso) {
    SDL_Rect screen_pos;
    screen_pos.x = perso->position.x - background_obj.camera.x;
    screen_pos.y = perso->position.y - background_obj.camera.y;
    SDL_BlitSurface(player_sprite_main, NULL, screen, &screen_pos);
}

void MAJMinimapXY(SDL_Rect posJoueurAbs, Minimap *m, float scaleX, float scaleY) {
    float centerX = posJoueurAbs.x + posJoueurAbs.w / 2.0f;
    float centerY = posJoueurAbs.y + posJoueurAbs.h / 2.0f;

    m->joueurPosition.x = (int)(centerX * scaleX) - m->joueurImage->w / 2;
    m->joueurPosition.y = (int)(centerY * scaleY) - m->joueurImage->h / 2;
    m->joueurPosition.w = m->joueurImage->w;
    m->joueurPosition.h = m->joueurImage->h;
}
