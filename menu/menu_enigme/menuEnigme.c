#include "menuEnigme.h"
#include "../header.h"

// Global variables for quiz
static int currentQuestionIndex = -1;
static int score = 0;
static int lives = 3;
static int level = 1;
static Question questions[MAX_QUESTIONS];
static int numQuestions = 0;


// Global variables for puzzle
//static SDL_Rect puzzlePosition = {800, 500, 300, 300}; // Position of the puzzle
static SDL_Rect piecePositions[9]; // Positions of the nine pieces
static SDL_Rect correctPositions[9]; // Correct positions of the nine pieces
static SDL_Surface *pieceImages[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static int isDragging = 0; // Whether the player is dragging a piece
static int draggedPieceIndex = -1; // Index of the currently dragged piece
static SDL_Rect draggedPieceOffset = {0, 0, 0, 0}; // Offset for dragging


// Puzzle assets
static SDL_Surface *puzzleImage = NULL;

// Global variable for enigme menu
int showSubButtons = 0;
SDL_Surface *screen = NULL;
int menuState = MENU_OPTION; // Initialize to MENU_OPTION or any default state



//--------------------------------------------Enigme Menu Functions-------------------------------------------
void initMenuEnigme(Menu *menus) {
    printf("Init Menu Enigme\n");
    menus[MENU_ENIGME].n_btns = 5;
    menus[MENU_ENIGME].buttons = malloc(menus[MENU_ENIGME].n_btns * sizeof(Button));
    if (!menus[MENU_ENIGME].buttons) {
        fprintf(stderr, "Failed to allocate memory for buttons\n");
        exit(EXIT_FAILURE);
    }
    loadQuestions("./menu/menu_enigme/questions.txt", questions, &numQuestions);
    menus[MENU_ENIGME].init_buttons = initMenuEnigmeButtons;
    menus[MENU_ENIGME].render = renderMenuEnigme;
    menus[MENU_ENIGME].handleEvent = handleEventEnigme;
    menus[MENU_ENIGME].init_buttons(menus[MENU_ENIGME].buttons);

    srand(time(NULL)); // Seed the random number generator
    loadQuestions("./menu/menu_enigme/questions.txt", questions, &numQuestions);
    if (numQuestions == 0) {
        fprintf(stderr, "No questions loaded. Exiting.\n");
        exit(EXIT_FAILURE);
    }
}


// Function to initialize enigme menu buttons
void initMenuEnigmeButtons(Button *buttons) {
    buttons[0].text = "Quiz";
    buttons[0].rect = (SDL_Rect){600, 200, 300, 40};
    buttons[0].selected = 0;

    buttons[1].text = "Puzzle";
    buttons[1].rect = (SDL_Rect){1000, 200, 300, 40};
    buttons[1].selected = 0;

    for (int i = 2; i < 5; i++) {
        
        buttons[i].rect = (SDL_Rect){400, 300 + (i - 2) * 200, 300, 40};
        buttons[i].selected = 0;
    }
}
void renderMenuEnigme(SDL_Surface *background, SDL_Surface *butImage, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL); // Render the background

    if (showSubButtons == 0) {
        // Render the main menu buttons (Quiz and Puzzle)
        for (int i = 0; i < 2; i++) {
            renderButton(screen, butImage, font, textColor, buttons[i]);
        }
    } else if (showSubButtons == 1) {
        // Render the Quiz UI
        renderQuizUI(screen, butImage, font, textColor, buttons, n_btns);
    } else if (showSubButtons == 2) {
        // Render the Puzzle UI
        renderPuzzle(screen);
    }

    SDL_Flip(screen); // Update the screen
}





//----------------------------------------------------quiz functions----------------------------------------------------
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
void renderQuizUI(SDL_Surface *screen, SDL_Surface *butImage, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    if (currentQuestionIndex == -1) {
        currentQuestionIndex = 0;
    }

    if (currentQuestionIndex >= numQuestions) {
        printf("All questions answered. Resetting...\n");
        currentQuestionIndex = 0;
    }

    Question currentQuestion = questions[currentQuestionIndex];

    // Render the quiz title
    renderText(screen, "Quiz", font, textColor, 600, 100);

    // Render the current question
    renderText(screen, currentQuestion.question, font, textColor, 400, 200);

    // Render the answer options

    for (int i = 2; i < n_btns; i++) {
        buttons[i].text = currentQuestion.options[i - 2];
        renderButton(screen, butImage, font, textColor, buttons[i]);
    }
    
    // Render the score and lives
    char scoreText[50];
    char livesText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    snprintf(livesText, sizeof(livesText), "Lives: %d", lives);

    renderText(screen, scoreText, font, textColor, 50, 50);  // Display score at the top-left corner
    renderText(screen, livesText, font, textColor, 50, 100); // Display lives below the score

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






//---------------------------------------------Puzzle Functions----------------------------------------------------
// Function to initialize the puzzle
void initPuzzle() {
    // Load the full puzzle image
    printf("Initializing puzzle...\n"); // Debug print
    puzzleImage = load_image("./assets/game/aria.png");
    if (!puzzleImage) {
        fprintf(stderr, "Failed to load puzzle image!\n");
        return;
    }

    // Create surfaces for the pieces
    int pieceWidth = puzzleImage->w / 3;  // Divide the image width into 3 pieces
    int pieceHeight = puzzleImage->h / 3; // Divide the image height into 3 pieces

    for (int i = 0; i < 9; i++) {
        pieceImages[i] = SDL_CreateRGBSurface(0, pieceWidth, pieceHeight, 32, 0, 0, 0, 0);
        if (!pieceImages[i]) {
            fprintf(stderr, "Failed to create surface for piece %d\n", i);
            return;
        }
    }

    // Define the correct positions of the pieces in the puzzle image
    SDL_Rect pieceRects[9];
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int index = row * 3 + col;
            pieceRects[index] = (SDL_Rect){col * pieceWidth, row * pieceHeight, pieceWidth, pieceHeight};
            correctPositions[index] = (SDL_Rect){600 + col * pieceWidth, 300 + row * pieceHeight, pieceWidth, pieceHeight};
        }
    }

    // Extract the pieces from the puzzle image
    for (int i = 0; i < 9; i++) {
        SDL_BlitSurface(puzzleImage, &pieceRects[i], pieceImages[i], NULL);
    }

    // Shuffle the pieces
    int indices[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    for (int i = 0; i < 9; i++) {
        int j = rand() % 9;
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Set the shuffled positions for the pieces
    int startX = 200; // Starting X position for the pieces
    int startY = 300; // Starting Y position for the pieces
    for (int i = 0; i < 9; i++) {
        int index = indices[i];
        piecePositions[index] = (SDL_Rect){startX + (i % 3) * (pieceWidth + 10), startY + (i / 3) * (pieceHeight + 10), pieceWidth, pieceHeight};
    }
}

void renderPuzzle(SDL_Surface *screen) {

    // Define the box dimensions
    SDL_Rect boxRect = {600, 300, 270 , 270}; // Position and size of the box

    // Draw the white box
    SDL_FillRect(screen, &boxRect, SDL_MapRGB(screen->format, 255, 255, 255)); // White fill

    // Draw the black edges
    SDL_Rect topEdge = {boxRect.x, boxRect.y, boxRect.w, 5}; // Top edge
    SDL_Rect bottomEdge = {boxRect.x, boxRect.y + boxRect.h - 5, boxRect.w, 5}; // Bottom edge
    SDL_Rect leftEdge = {boxRect.x, boxRect.y, 5, boxRect.h}; // Left edge
    SDL_Rect rightEdge = {boxRect.x + boxRect.w - 5, boxRect.y, 5, boxRect.h}; // Right edge

    SDL_FillRect(screen, &topEdge, SDL_MapRGB(screen->format, 0, 0, 0)); // Black top edge
    SDL_FillRect(screen, &bottomEdge, SDL_MapRGB(screen->format, 0, 0, 0)); // Black bottom edge
    SDL_FillRect(screen, &leftEdge, SDL_MapRGB(screen->format, 0, 0, 0)); // Black left edge
    SDL_FillRect(screen, &rightEdge, SDL_MapRGB(screen->format, 0, 0, 0)); // Black right edge

    // Draw the 3x3 grid inside the box
    int cellWidth = boxRect.w / 3;
    int cellHeight = boxRect.h / 3;
    for (int row = 1; row < 3; row++) {
        SDL_Rect horizontalLine = {boxRect.x, boxRect.y + row * cellHeight, boxRect.w, 2};
        SDL_FillRect(screen, &horizontalLine, SDL_MapRGB(screen->format, 0, 0, 0)); // Black horizontal line
    }
    for (int col = 1; col < 3; col++) {
        SDL_Rect verticalLine = {boxRect.x + col * cellWidth, boxRect.y, 2, boxRect.h};
        SDL_FillRect(screen, &verticalLine, SDL_MapRGB(screen->format, 0, 0, 0)); // Black vertical line
    }

    // Render the pieces
    for (int i = 0; i < 9; i++) {
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

        // Check if the puzzle is completed
        int tolerance = 20; // Allowable range for approximate placement
        int piecesInPlace = 0;
    
        for (int i = 0; i < 9; i++) {
            // Check if the piece is inside the box and approximately in the correct position
            if (piecePositions[i].x >= 600 && piecePositions[i].x <= 870 && // Inside the box horizontally
                piecePositions[i].y >= 300 && piecePositions[i].y <= 570 && // Inside the box vertically
                abs(piecePositions[i].x - correctPositions[i].x) <= tolerance &&
                abs(piecePositions[i].y - correctPositions[i].y) <= tolerance) {
                piecesInPlace++;
            }
        }
    
        if (piecesInPlace == 9) { // All pieces are approximately in place
            printf("Puzzle completed!\n");

            // Render "You Won" text
            TTF_Font *font = TTF_OpenFont("./assets/fonts/font.ttf", 400); // Load a font
            if (!font) {
                fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
                return;
            }
            SDL_Color textColor = {255, 0, 0, 0}; // Red color
            renderText(screen, "You Won!", font, textColor, 250, 500); // Render text at the top
            TTF_CloseFont(font); // Close the font



            /*showSubButtons = 0; // Reset to main menu or another state
            menuState = MENU_OPTION; // Return to menu option*/
        }
}













//--------------------------------------handle events----------------------------------------------------
// Function to handle enigme menu events
void handleEventEnigme(int *menuState, Save save, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            *menuState = QUIT_GAME;
        }

        if (event.type == SDL_MOUSEMOTION) {
            // Highlight buttons on hover
            for (int i = 0; i < n_btns; i++) {
                if (showSubButtons == 0 && i < 2) { // Only highlight main buttons when in main menu
                    if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                        event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                        buttons[i].selected = 1;
                    } else {
                        buttons[i].selected = 0;
                    }
                } else {
                    buttons[i].selected = 0; // Disable hover for other buttons
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n_btns; i++) {
                    if (buttons[i].selected) {
                        if (showSubButtons == 0) { // Only handle main buttons when in main menu
                            if (i == 0) {
                                printf("Quiz selected\n");
                                showSubButtons = 1;
                            } else if (i == 1) {
                                printf("Puzzle selected\n");
                                showSubButtons = 2;
                                initPuzzle();
                            }
                        } else if (showSubButtons == 1) { // Handle Quiz-specific buttons
                            printf("Answer selected: Option %d\n", i - 2);
                            checkAnswer(i - 2, questions[currentQuestionIndex].correctOption, &score, &lives, &level, menuState);
                        }
                    }
                }
            }
        }

        // Handle puzzle-specific events
        if (showSubButtons == 2) { // Puzzle mode
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 9; i++) {
                        if (event.button.x >= piecePositions[i].x &&
                            event.button.x <= piecePositions[i].x + piecePositions[i].w &&
                            event.button.y >= piecePositions[i].y &&
                            event.button.y <= piecePositions[i].y + piecePositions[i].h) {
                            isDragging = 1;
                            draggedPieceIndex = i;
                            draggedPieceOffset.x = event.button.x - piecePositions[i].x;
                            draggedPieceOffset.y = event.button.y - piecePositions[i].y;
                            break;
                        }
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT && isDragging) {
                    isDragging = 0;

                    // Snap the dragged piece to the nearest grid position if inside the box
                    if (draggedPieceIndex != -1) {
                        int cellWidth = 270 / 3; // Width of each grid cell (90)
                        int cellHeight = 270 / 3; // Height of each grid cell (90)

                        // Calculate the center of the piece
                        int pieceCenterX = piecePositions[draggedPieceIndex].x + piecePositions[draggedPieceIndex].w / 2;
                        int pieceCenterY = piecePositions[draggedPieceIndex].y + piecePositions[draggedPieceIndex].h / 2;

                        // Check if the center of the piece is inside the box
                        if (pieceCenterX >= 600 && pieceCenterX <= 870 &&
                            pieceCenterY >= 300 && pieceCenterY <= 570) {
                            // Calculate the grid cell (0-2 for both X and Y)
                            int gridX = (pieceCenterX - 600) / cellWidth;
                            int gridY = (pieceCenterY - 300) / cellHeight;

                            // Snap the piece to the grid
                            piecePositions[draggedPieceIndex].x = 600 + gridX * cellWidth;
                            piecePositions[draggedPieceIndex].y = 300 + gridY * cellHeight;
                        }
                    }

                    draggedPieceIndex = -1;
                }
            }

            if (event.type == SDL_MOUSEMOTION && isDragging) {
                piecePositions[draggedPieceIndex].x = event.motion.x - draggedPieceOffset.x;
                piecePositions[draggedPieceIndex].y = event.motion.y - draggedPieceOffset.y;
            }
        }
    }
}
// Function to clean up the enigme menu
void cleanupMenuEnigme(Menu *menu) {
    free(menu->buttons);
    menu->buttons = NULL;
}