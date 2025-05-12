#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MAX_BALLS 10
#define BALL_SPEED 8
#define BALL_WIDTH 30
#define BALL_HEIGHT 30
#define PLAYER_MAX_HEALTH 100
#define DAMAGE_COOLDOWN 1000 // 1 second cooldown between damage

// Game states
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
} GameState;

// Enemy states
typedef enum {
    WAITING,
    FOLLOWING,
    ATTACKING
} EnemyState;

// Enemy structure
typedef struct {
    int health;
    float x, y;
    SDL_Surface* frames[4];
    int current_frame;
    EnemyState state;
    float dx, dy;
    float patrol_start_x;
    int patrol_direction;
    Uint32 last_attack_time;
    Uint32 attack_cooldown;
} Enemy;

// Player structure
typedef struct {
    SDL_Surface* img;
    SDL_Rect pos;
    int health;
    int lives;
    int score;
    float velocity;
    float spd;
    SDL_Rect posSprite;
    int state;
    float acc;
    Uint32 last_damage_time;  // Time of last damage taken
    float vy;              // Vertical velocity
    int is_jumping;        // Jump state
    int facing_left;       // Direction facing
} Player;

// Ball structure
typedef struct {
    SDL_Rect pos;
    float dx, dy;
    int active;
    Uint32 creation_time;
    int radius;
} Ball;

// Input structure
typedef struct {
    // Movement controls
    int left, right, up, down;
    int jump;
    int accel;
    
    // Action controls
    int h;      // Shoot ball
    int space;  // Restart game
    int escape; // Pause game
    int q;      // Quit game
} Input;

// Image structure
typedef struct {
    SDL_Rect pos1;
    SDL_Rect pos2;
    SDL_Surface* img;
} Image;

// Background functions
SDL_Surface* load_background(const char* filename);
void display_background(SDL_Surface* screen, SDL_Surface* background);

// Enemy functions
void init_enemy(Enemy *enemy, int health, int x, int y);
void display_enemy(Enemy *enemy, SDL_Surface* screen);
void draw_enemy_health_bar(SDL_Surface* screen, Enemy* enemy);
void animate_enemy_move(Enemy *enemy);
void move_enemy_randomly(Enemy *enemy, int level);
void move_enemy_ai(Enemy *enemy, int player_x, int player_y, int vision_range, int attack_range);
int check_collision_player_enemy(SDL_Rect player_rect, Enemy *enemy);
void update_enemy_health(Enemy *enemy, int damage);

// Player functions
void init_player(Player* player, const char* sprite_path, int x);
void display_player(SDL_Surface* screen, Player* player);
void move_player(Input* input, Player* player, Uint32 dt);
void jump_player(Input* input, Player* player, int* jump_height);
void animate_player(Player* player, Input input);
void draw_player_health_bar(SDL_Surface* screen, Player* player);
void update_player_health(Player* player, int damage, Uint32 current_time);

// Ball system functions
void init_balls(void);
void create_ball(Player* player);
void update_balls(void);
void display_balls(SDL_Surface* screen);
void free_balls(void);

// Input functions
void get_input(Input* input);

// Cleanup functions
void free_player(Player* player);
void free_enemy(Enemy* enemy);

// Resource management functions
int init_game_resources(void);
void cleanup_game_resources(void);

// Function declarations for enemy attack system
int enemy_can_attack(Enemy* enemy, Uint32 current_time);

// Function declarations for collision detection
int check_collision_ball_enemy(SDL_Rect ball_rect, Enemy *enemy);
void check_ball_enemy_collisions(Enemy enemies[], int num_enemies);

// Menu rendering functions
void render_text_centered(SDL_Surface* screen, const char* text, TTF_Font* font, SDL_Color color, int y);
void display_pause_menu(SDL_Surface* screen);
void display_game_over(SDL_Surface* screen, int score);

#endif // GAME_H 