#include "menuBestScore.h"
#include "../header.h"
#include <string.h>
#include "../game/include/game.h"  // Add game.h include for game-related constants and types

#define MAX_SCORES 3
static ScoreEntry scores[MAX_SCORES];
static int inputActive = 0;
static char inputText[50] = "";

void initMenuBestScore(Menu *menus) {
    menus[MENU_BEST_SCORE].n_btns = 3;
    menus[MENU_BEST_SCORE].buttons = malloc(menus[MENU_BEST_SCORE].n_btns * sizeof(Button));
    if (!menus[MENU_BEST_SCORE].buttons) {
        fprintf(stderr, "Failed to allocate memory for buttons\n");
        exit(EXIT_FAILURE);
    }
    menus[MENU_BEST_SCORE].init_buttons = initMenuBestScoreButtons;
    menus[MENU_BEST_SCORE].render = renderMenuBestScore;
    menus[MENU_BEST_SCORE].handleEvent = handleEventBestScore;
    menus[MENU_BEST_SCORE].init_buttons(menus[MENU_BEST_SCORE].buttons);
    loadScores(scores);
}

void initMenuBestScoreButtons(Button *buttons) {
    // Calculate button width and spacing
    int buttonWidth = 200;
    int spacing = 20;
    int totalWidth = (buttonWidth * 3) + (spacing * 2);
    int startX = (SCREEN_WIDTH - totalWidth) / 2;
    int buttonY = SCREEN_HEIGHT - 100; // Position buttons near bottom of screen

    // Position buttons horizontally
    buttons[0].rect = (SDL_Rect){startX, buttonY, buttonWidth, 40};
    buttons[0].text = "Enter Name";
    buttons[0].selected = 0;

    buttons[1].rect = (SDL_Rect){startX + buttonWidth + spacing, buttonY, buttonWidth, 40};
    buttons[1].text = "Quit";
    buttons[1].selected = 0;

    buttons[2].rect = (SDL_Rect){startX + (buttonWidth + spacing) * 2, buttonY, buttonWidth, 40};
    buttons[2].text = "Main Menu";
    buttons[2].selected = 0;
}

void renderMenuBestScore(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    // Render title
    SDL_Surface *titleSurface = TTF_RenderText_Solid(font, "High Scores", textColor);
    SDL_Rect titleRect = {SCREEN_WIDTH/2 - titleSurface->w/2, 50, titleSurface->w, titleSurface->h};
    SDL_BlitSurface(titleSurface, NULL, screen, &titleRect);
    SDL_FreeSurface(titleSurface);

    // Render top 3 scores
    for (int i = 0; i < MAX_SCORES; i++) {
        char scoreText[100];
        snprintf(scoreText, sizeof(scoreText), "%d. %s - %d", i + 1, scores[i].name, scores[i].score);
        SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
        SDL_Rect scoreRect = {SCREEN_WIDTH/2 - scoreSurface->w/2, 150 + i * 50, scoreSurface->w, scoreSurface->h};
        SDL_BlitSurface(scoreSurface, NULL, screen, &scoreRect);
        SDL_FreeSurface(scoreSurface);
    }

    // Render input prompt if input is not active
    if (!inputActive) {
        SDL_Surface *promptSurface = TTF_RenderText_Solid(font, "Press 'Enter Name' to save your score", textColor);
        SDL_Rect promptRect = {350, 300, promptSurface->w, promptSurface->h};
        SDL_BlitSurface(promptSurface, NULL, screen, &promptRect);
        SDL_FreeSurface(promptSurface);
    }

    // Render buttons
    for (int i = 0; i < n_btns; i++) {
        renderButton(screen, butImage, font, textColor, buttons[i]);
    }

    // Render input box if active
    if (inputActive) {
        renderInputBox(screen, font, inputText);
    }

    SDL_Flip(screen);
}

void handleEventBestScore(int *menuState, Save save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *menuState = QUIT_GAME;
            return;
        }
        
        if (event.type == SDL_MOUSEMOTION) {
            for (int i = 0; i < n_btns; i++) {
                if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                    event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                    buttons[i].selected = 1;
                } else {
                    buttons[i].selected = 0;
                }
            }
        }

        if (inputActive) {
            // Handle keyboard input when input box is active
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (strlen(inputText) > 0) {
                        addScore(scores, inputText, save.players->score);
                        saveScores(scores);
                        inputActive = 0;
                        memset(inputText, 0, sizeof(inputText));
                    }
                } else {
                    // Handle regular character input
                    char c = event.key.keysym.sym;
                    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ') {
                        if (strlen(inputText) < sizeof(inputText) - 1) {
                            inputText[strlen(inputText)] = c;
                            inputText[strlen(inputText) + 1] = '\0';
                        }
                    }
                }
            }
        } else {
            // Handle mouse clicks for buttons when input is not active
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n_btns; i++) {
                    if (buttons[i].selected) {
                        if (i == 0) { // Enter Name button
                            inputActive = 1;
                            memset(inputText, 0, sizeof(inputText));
                        } else if (i == 1) { // Quit button
                            *menuState = QUIT_GAME;
                        } else if (i == 2) { // Main Menu button
                            // Reset game state before returning to main menu
                            save.players->health = 100; // Use hardcoded value instead of PLAYER_MAX_HEALTH
                            save.players->lives = 3;
                            save.players->score = 0;
                            save.players->pos.x = 200;
                            save.players->pos.y = 500;
                            
                            *menuState = MENU_PRINCIPAL;
                        }
                    }
                }
            }
        }
    }
}

void renderInputBox(SDL_Surface *screen, TTF_Font *font, const char *inputText) {
    // Draw a larger, more visible input box
    SDL_Rect box = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 25, 400, 50};
    
    // Draw white background
    SDL_FillRect(screen, &box, SDL_MapRGB(screen->format, 255, 255, 255));
    
    // Draw black border
    SDL_Rect border = {box.x - 2, box.y - 2, box.w + 4, box.h + 4};
    SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 0, 0, 0));
    
    // Draw white background again (inside the border)
    SDL_FillRect(screen, &box, SDL_MapRGB(screen->format, 255, 255, 255));

    // Draw text
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, inputText, textColor);
    if (textSurface) {
        SDL_Rect textRect = {box.x + 10, box.y + (box.h - textSurface->h) / 2, textSurface->w, textSurface->h};
        SDL_BlitSurface(textSurface, NULL, screen, &textRect);
        SDL_FreeSurface(textSurface);
    }

    // Draw cursor if input is active
    if (inputActive) {
        SDL_Rect cursor = {box.x + 10 + (textSurface ? textSurface->w : 0), box.y + 10, 2, box.h - 20};
        SDL_FillRect(screen, &cursor, SDL_MapRGB(screen->format, 0, 0, 0));
    }
}

void saveScores(ScoreEntry scores[]) {
    FILE *file = fopen("scores.dat", "wb");
    if (file) {
        fwrite(scores, sizeof(ScoreEntry), MAX_SCORES, file);
        fclose(file);
    }
}

void addScore(ScoreEntry scores[], const char *name, int score) {
    // First check if the name already exists
    int existingIndex = -1;
    for (int i = 0; i < MAX_SCORES; i++) {
        if (strcmp(scores[i].name, name) == 0) {
            existingIndex = i;
            break;
        }
    }

    if (existingIndex != -1) {
        // If name exists and new score is higher, update it
        if (score > scores[existingIndex].score) {
            scores[existingIndex].score = score;
        }
    } else {
        // Find the correct position to insert the new score
        int insertPos = -1;
        for (int i = 0; i < MAX_SCORES; i++) {
            if (score > scores[i].score) {
                insertPos = i;
                break;
            }
        }

        if (insertPos != -1) {
            // Shift scores down
            for (int i = MAX_SCORES - 1; i > insertPos; i--) {
                scores[i] = scores[i - 1];
            }
            // Insert new score
            strncpy(scores[insertPos].name, name, sizeof(scores[insertPos].name) - 1);
            scores[insertPos].name[sizeof(scores[insertPos].name) - 1] = '\0';
            scores[insertPos].score = score;
        }
    }

    // Sort scores in descending order
    for (int i = 0; i < MAX_SCORES - 1; i++) {
        for (int j = 0; j < MAX_SCORES - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                // Swap scores
                ScoreEntry temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

void loadScores(ScoreEntry scores[]) {
    FILE *file = fopen("scores.dat", "rb");
    if (file) {
        fread(scores, sizeof(ScoreEntry), MAX_SCORES, file);
        fclose(file);
    } else {
        // Initialize with default values if file doesn't exist
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(scores[i].name, "---");
            scores[i].score = 0;
        }
    }
}

void cleanupMenuBestScore(Menu *menu) {
    free(menu->buttons);
    menu->buttons = NULL;
}


