#include <SDL/SDL.h>//inclure SDL.h  pour permettre au programme d'être portable sur tous les systèmes.
#include <SDL/SDL_image.h>//.................//Pour manipuler des images ayants des types autre que bmp
#include <SDL/SDL_mixer.h>//.................//Pour manipuler de l’audio.
#include <SDL/SDL_ttf.h>//.................//Pour manipuler des textes
#include "main.h"

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        exit(1);
    }
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0) {
        printf("Mix_OpenAudio error: %s\n", Mix_GetError());
        exit(1);
    }
    SDL_WM_SetCaption("Best Scores Menu", NULL);
    TTF_Init();
}
SDL_Surface* create_window(int width, int height) {
    SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Window creation failed: %s\n", SDL_GetError());
        exit(1);
    }
    return screen;
}

void cleanUp(SDL_Surface *screen) {
    SDL_FreeSurface(screen);
    SDL_Quit();
}

SDL_Surface* load_image(const char* filename){
    SDL_Surface* image = IMG_Load(filename);
    if (!image) {
        printf("Image not found: %s\n", filename);
        exit(1);
    }
    return image;
}
void displayButton(SDL_Surface *screen, SDL_Surface *button, int x, int y) {
    SDL_Rect buttonPosition;
    buttonPosition.x = x;
    buttonPosition.y = y;
    SDL_BlitSurface(button, NULL, screen, &buttonPosition);
}
int isButtonClicked(int mouseX, int mouseY, int buttonX, int buttonY, int buttonW, int buttonH) {
    return (mouseX > buttonX && mouseX < buttonX + buttonW &&
            mouseY > buttonY && mouseY < buttonY + buttonH);
}


TTF_Font* load_font(const char* filename, int size){
    TTF_Init();
    TTF_Font* font = TTF_OpenFont(filename, size);
    if (!font) {
        printf("Font not found: %s\n", filename);
        exit(1);
    }
    return font;
}

Mix_Music* load_music(const char* filename) {
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Music not found: %s\n", filename);
        exit(1);
    }
    return music;
}
void handleInput(SDL_Event event, char *inputText, int maxLength) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
            inputText[strlen(inputText) - 1] = '\0';  // Remove last character
        } 
        else if (event.key.keysym.sym == SDLK_RETURN) {
            printf("User entered: %s\n", inputText);  // Simulate action
        } 
        else {
            Uint16 unicode = event.key.keysym.unicode;  // Get Unicode character
            if (unicode >= 32 && unicode <= 126 && strlen(inputText) < maxLength - 1) { 
                int len = strlen(inputText);
                inputText[len] = (char)unicode;  // Add character
                inputText[len + 1] = '\0';  // Null-terminate
            }
        }
    }
}


void renderInputBox(SDL_Surface *screen, TTF_Font *font, const char *inputText) {
    // Define input box dimensions
    SDL_Rect box = {400, 500, 400, 50};  
    SDL_FillRect(screen, &box, SDL_MapRGB(screen->format, 255, 255, 255));  // White box

    // Render text inside the input box
    SDL_Color textColor = {0, 0, 0};  // Black color
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, inputText, textColor);
    if (textSurface) {
        SDL_Rect textPosition = {box.x + 10, box.y + 10};  // Offset text inside the box
        SDL_BlitSurface(textSurface, NULL, screen, &textPosition);
        SDL_FreeSurface(textSurface);
    }

    // Refresh screen to avoid flickering
    SDL_Flip(screen);

}

void handleEvents(int *running, SDL_Surface *screen, SDL_Surface *Button, int buttonX, int buttonY,
    int *valider, char *inputText, int buttonX2, int buttonY2, int buttonX3, int buttonY3, int *menustate) {
SDL_Event event;

while (SDL_PollEvent(&event)) {
if (event.type == SDL_QUIT) {
*running = 0;
handleInput(event, inputText, 50);
} 
else if (event.type == SDL_KEYDOWN) {
if (event.key.keysym.sym == SDLK_RETURN) *valider = 0;
else if (event.key.keysym.sym == SDLK_e) *menustate = 9;
else if (event.key.keysym.sym == SDLK_ESCAPE) *running = 0;
} 
else if (event.type == SDL_MOUSEBUTTONDOWN) {
int mouseX = event.button.x;
int mouseY = event.button.y;

// Check if "valid" button is clicked
if (isButtonClicked(mouseX, mouseY, buttonX, buttonY, Button->w, Button->h)) {
  printf("Valid button clicked!\n");
  *valider = 0; 
}

// ✅ FIXED COMPARISON: `if (*valider == 0)` instead of `if (valider=0)`
if (*valider == 0) {
  printf("We got in the loop!\n");

  // Check if "Quitter" button is clicked
  if (isButtonClicked(mouseX, mouseY, buttonX2, buttonY2, Button->w, Button->h)) {
      printf("Quitter button clicked!\n");
      *running = 0;  
  }

  // Check if "Retour" button is clicked
  if (isButtonClicked(mouseX, mouseY, buttonX3, buttonY3, Button->w, Button->h)) {
      printf("Retour button clicked!\n");
      *menustate = 0;  
  }
}
}
}
}



void renderBestScores(SDL_Surface *screen, ScoreEntry scores[],TTF_Font *font) {
    SDL_Rect pos = {500, 300, 0, 0};
    char buffer[50];
    
    for (int i = 0; i < MAX_SCORES; i++) {
        snprintf(buffer, sizeof(buffer), "%d. %s - %d ", i + 1, scores[i].name, scores[i].score);
        SDL_Color color = {255, 255, 255};
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, buffer, color);
        SDL_BlitSurface(text_surface, NULL, screen, &pos);
        SDL_FreeSurface(text_surface);
        pos.y += 40;
    }
}

void loadScores(ScoreEntry scores[]) {
    FILE *file = fopen("scores.dat", "r");
    if (file) {
        fread(scores, sizeof(ScoreEntry), MAX_SCORES, file);
        fclose(file);
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(scores[i].name, "---");
            scores[i].score = 0;
        }
    }
}
void displayText(SDL_Surface *screen, const char *message, int x, int y) {
    TTF_Font *font = TTF_OpenFont("KiwiSoda.ttf", 80);  // Load font (size 24)
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255};  // White color
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, message, textColor);
    if (!textSurface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textPosition;
    textPosition.x = x;
    textPosition.y = y;

    SDL_BlitSurface(textSurface, NULL, screen, &textPosition); // Draw text
    SDL_Flip(screen);  // Update screen

    SDL_FreeSurface(textSurface); // Free memory
    TTF_CloseFont(font); // Close font
}

void saveScores(ScoreEntry scores[]) {
    FILE *file = fopen("scores.dat", "w");
    if (file) {
        fwrite(scores, sizeof(ScoreEntry), MAX_SCORES, file);
        fclose(file);
    }
}

void addScore(ScoreEntry scores[], const char *name, int score) {
    for (int i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i].score) {
            for (int j = MAX_SCORES - 1; j > i; j--) {
                scores[j] = scores[j - 1];
            }
            strncpy(scores[i].name, name, sizeof(scores[i].name));
            scores[i].score = score;
            break;
        }
    }
}