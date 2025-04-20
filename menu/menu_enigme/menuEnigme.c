#include "menuEnigme.h"
#include "../header.h"
#include <SDL/SDL_rotozoom.h> // For rotozoom effect

// Global variables for quiz
static int currentQuestionIndex = -1;
static int score = 0;
static int lives = 3;
static int level = 1;
static Question questions[MAX_QUESTIONS];
static int numQuestions = 0;

// Global variables for puzzle
static SDL_Rect puzzlePosition = {600, 300, 300, 300}; // Position of the puzzle
static SDL_Rect piecePositions[3]; // Positions of the three possible pieces
static int correctPieceIndex = 0; // Index of the correct piece
static int isDragging = 0; // Whether the player is dragging a piece
static int draggedPieceIndex = -1; // Index of the currently dragged piece
static SDL_Rect draggedPieceOffset = {0, 0, 0, 0}; // Offset for dragging
static int timer = 100; // Timer for the puzzle (in arbitrary units)

// Puzzle assets
static SDL_Surface *puzzleImage = NULL;
static SDL_Surface *pieceImages[3] = {NULL, NULL, NULL};

// Global variable for enigme menu
int showSubButtons = 0;
SDL_Surface *screen = NULL;

// Function to load quiz questions
void loadQuestions(const char *filename, Question *questions, int *numQuestions) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open questions file: %s\n", filename);
        return;
    }

    char line[256];
    int questionIndex = 0;

    while (fgets(line, sizeof(line), file) && questionIndex < MAX_QUESTIONS) {
        if (sscanf(line, "%255[^;];%255[^;];%255[^;];%255[^;];%d",
                   questions[questionIndex].question,
                   questions[questionIndex].options[0],
                   questions[questionIndex].options[1],
                   questions[questionIndex].options[2],
                   &questions[questionIndex].correctOption) == 5) {
            questionIndex++;
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", line);
        }
    }

    *numQuestions = questionIndex;
    fclose(file);

    for (int i = 0; i < *numQuestions; i++) {
        int j = rand() % *numQuestions;
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

// Function to render the quiz UI
void renderQuizUI(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    if (currentQuestionIndex == -1) {
        currentQuestionIndex = 0;
    }

    if (currentQuestionIndex >= numQuestions) {
        printf("All questions answered. Resetting...\n");
        currentQuestionIndex = 0;
    }

    Question currentQuestion = questions[currentQuestionIndex];

    renderText(screen, "Quiz", font, textColor, 600, 100);
    renderText(screen, currentQuestion.question, font, textColor, 400, 200);

    for (int i = 2; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);
    }

    renderText(screen, currentQuestion.options[0], font, textColor, 425, 300);
    renderText(screen, currentQuestion.options[1], font, textColor, 425, 500);
    renderText(screen, currentQuestion.options[2], font, textColor, 425, 700);

    SDL_Flip(screen);
}

// Function to check quiz answers
void checkAnswer(int selectedOption, int correctOption, int *score, int *lives, int *level, int *menuState) {
    if (selectedOption > -1) {
        if (selectedOption == correctOption) {
            printf("Correct answer!\n");
            (*score)++;
            if (*score % 5 == 0) {
                (*level)++;
            }
        } else {
            printf("Wrong answer!\n");
            printf("showbuttons: %d\n", showSubButtons);
            if (*lives > 0) {
                (*lives)--;
            }
            if (*lives == 0) {
                printf("Returning to Menu Option...\n");
                *score = 0;
                *lives = 3;
                *level = 1;
                showSubButtons = 0;
                *menuState = MENU_OPTION;
                
                
            }
        }
        if (*score == 10) {
            *menuState = MENU_PRINCIPAL;
        }
        currentQuestionIndex++;
        
        
    }
}

// Function to initialize the puzzle
void initPuzzle() {
    // Load the full puzzle image
    puzzleImage = load_image("./assets/game/aria.png");

    // Create surfaces for the pieces
    pieceImages[0] = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    pieceImages[1] = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    pieceImages[2] = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);

    // Define the positions of the pieces in the puzzle image
    SDL_Rect pieceRects[3] = {
        {100, 100, 100, 100}, // Top-left piece
        {200, 200, 100, 100}, // Middle piece
        {300, 300, 100, 100}  // Bottom-right piece
    };

    // Extract the pieces from the puzzle image
    for (int i = 0; i < 3; i++) {
        SDL_BlitSurface(puzzleImage, &pieceRects[i], pieceImages[i], NULL);
        SDL_FillRect(puzzleImage, &pieceRects[i], SDL_MapRGB(puzzleImage->format, 0, 0, 0)); // Make the piece area black
    }

    // Apply transparency to the pieces
    for (int i = 0; i < 3; i++) {
        SDL_SetColorKey(pieceImages[i], SDL_SRCCOLORKEY, SDL_MapRGB(pieceImages[i]->format, 0, 0, 0));
    }

    // Randomly determine the correct piece
    correctPieceIndex = rand() % 3;

    // Set positions for the pieces on the screen
    piecePositions[0] = (SDL_Rect){200, 700, 100, 100};
    piecePositions[1] = (SDL_Rect){400, 700, 100, 100};
    piecePositions[2] = (SDL_Rect){600, 700, 100, 100};
}

// Function to render the puzzle
void renderPuzzle(SDL_Surface *screen) {
    printf("Rendering puzzle...\n"); // Debug print

    if (!puzzleImage) {
        printf("Puzzle image not loaded!\n");
        return;
    }

    // Render the puzzle image
    SDL_BlitSurface(puzzleImage, NULL, screen, &puzzlePosition);

    // Render the pieces
    for (int i = 0; i < 3; i++) {
        if (i != draggedPieceIndex) {
            SDL_BlitSurface(pieceImages[i], NULL, screen, &piecePositions[i]);
        }
    }

    // Render the dragged piece
    if (isDragging && draggedPieceIndex != -1) {
        SDL_Rect draggedPosition = {
            piecePositions[draggedPieceIndex].x + draggedPieceOffset.x,
            piecePositions[draggedPieceIndex].y + draggedPieceOffset.y,
            0, 0
        };
        SDL_BlitSurface(pieceImages[draggedPieceIndex], NULL, screen, &draggedPosition);
    }

    // Render the timer
    SDL_Rect timerBar = {50, 50, timer * 3, 20};
    SDL_FillRect(screen, &timerBar, SDL_MapRGB(screen->format, 255, 0, 0));
}

// Function to handle enigme menu events
void handleEventEnigme(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            *menuState = QUIT_GAME;
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
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n_btns; i++) {
                    if (buttons[i].selected) {
                        if (i == 0) {
                            printf("Quiz selected\n");
                            showSubButtons = 1;
                        } else if (i == 1) {
                            printf("Puzzle selecteddddd\n");
                            showSubButtons = 2;
                            initPuzzle();
                        }
                    }
                }
            }
        }
    }
}

// Function to render the enigme menu
void renderMenuEnigme(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);
    if (showSubButtons == 0) {
        for (int i = 0; i < 2; i++) {
            renderButton(screen, font, textColor, buttons[i]);
        }
    } else if (showSubButtons == 1) {
        renderQuizUI(screen, font, textColor, buttons, n_btns);
    } else if (showSubButtons == 2) {
        renderPuzzle(screen);
    }

    SDL_Flip(screen);
}

// Function to clean up the enigme menu
void cleanupMenuEnigme(Menu *menu) {
    for (int i = 0; i < menu->n_btns; i++) {
        if (menu->buttons[i].normalImage) {
            SDL_FreeSurface(menu->buttons[i].normalImage);
            menu->buttons[i].normalImage = NULL;
        }
        if (menu->buttons[i].hoverImage) {
            SDL_FreeSurface(menu->buttons[i].hoverImage);
            menu->buttons[i].hoverImage = NULL;
        }
    }
    free(menu->buttons);
    menu->buttons = NULL;
}