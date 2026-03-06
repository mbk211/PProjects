#include "tictactoe.h"

player board[SIZE][SIZE] = {0};


bool setMarker(player turn, grid localGrid)
{
    int row = localGrid.row;
    int col = localGrid.column;

    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE){
        return false;
    }

    if (board[row][col] == EMPTY){
        board[row][col] = turn;
        return true;
    }

    return false;
}

grid cellChecker(int cellSize, coordinate cell)
{
    grid out;

    out.column = cell.x / cellSize;
    out.row    = cell.y / cellSize;

    return out;
}

bool winCheck(player mark){
    unsigned short count1 = 0, count2 = 0;
    for (int i = 0; i < SIZE; i++){
        // diagonal checker
        if (board[i][i] == mark){
            count1 += 1;
            if (count1 == SIZE){
                return true;
            }
        }
        // diagonal checker
        if (board[i][SIZE - 1 - i] == mark){
            count2 += 1;
            if (count2 == SIZE){
                return true;
            }
        }
        // Vertical Chcker
        if ((board[0][i] == mark) && (board[1][i] == mark) && (board[2][i] == mark)){
            return true;
        }
        // Horizontal Chcker
        if ((board[i][0] == mark) && (board[i][1] == mark) && (board[i][2] == mark)){
            return true;
        }
    }
    return false;
}

bool idleCheck(){
    unsigned short count = 0;
    for (int j = 0; j < SIZE; j++){
        for (int i = 0; i < SIZE; i++){
            if (board[j][i] != EMPTY){
                count++;
                if (count == SIZE*SIZE){
                    return true;
                }
            }
        }
    }
    return false;
}

void drawBorders(SDL_Renderer *renderer, int cellSize, int windowSize){


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Vertical lines
    SDL_RenderDrawLine(renderer, cellSize, 0, cellSize, windowSize);
    SDL_RenderDrawLine(renderer, cellSize * 2, 0, cellSize * 2, windowSize);
    // Horizontal lines
    SDL_RenderDrawLine(renderer, 0, cellSize, windowSize, cellSize);
    SDL_RenderDrawLine(renderer, 0, cellSize * 2, windowSize, cellSize * 2);
}


void drawX(SDL_Renderer *renderer, int cellSize, grid square, int offset)
{
    int startX = square.column * cellSize;
    int startY = square.row * cellSize;

    int endX = startX + cellSize;
    int endY = startY + cellSize;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, startX + offset, startY + offset, endX - offset,   endY - offset);
    SDL_RenderDrawLine(renderer, endX - offset,   startY + offset, startX + offset, endY - offset);
}

void drawO(SDL_Renderer *renderer, int cellSize, grid square, int offset)
{
    int startX = square.column * cellSize;
    int startY = square.row * cellSize;

    int centerX = startX + cellSize / 2;
    int centerY = startY + cellSize / 2;
    int radius  = (cellSize / 2) - offset;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int segments = 100;
    for (int i = 0; i < segments; i++)
    {
        float theta1 = (2.0f * M_PI * i) / segments;
        float theta2 = (2.0f * M_PI * (i + 1)) / segments;

        int x1 = centerX + radius * cos(theta1);
        int y1 = centerY + radius * sin(theta1);

        int x2 = centerX + radius * cos(theta2);
        int y2 = centerY + radius * sin(theta2);

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void drawBoard(SDL_Renderer *renderer, int cellSize)
{
    for (int row = 0; row < SIZE; row++){
        for (int col = 0; col < SIZE; col++){

            grid localgrid;
            localgrid.row = row;
            localgrid.column = col;

            if (board[row][col] == PLAYER1){
                drawX(renderer, cellSize, localgrid, 50);
            }
            else if (board[row][col] == PLAYER2){
                drawO(renderer, cellSize, localgrid, 50);
            }
        }
    }
}

void gameOverlay(SDL_Renderer *renderer, int windowSize, TTF_Font *font, char* gameText){
    int rectWidth  = 600;
    int rectHeight = 500;

    SDL_Rect rect;

    rect.w = rectWidth;
    rect.h = rectHeight;

    rect.x = (windowSize - rectWidth) / 2;
    rect.y = (windowSize - rectHeight) / 2;

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);  

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *textSurface =
        TTF_RenderText_Blended(font, gameText, white);

    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    textRect.x = rect.x + (rect.w - textRect.w) / 2;
    textRect.y = rect.y + (rect.h - textRect.h) / 2;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color,SDL_Rect container){
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;

    textRect.x = container.x + (container.w - textRect.w) / 2;
    textRect.y = container.y + (container.h - textRect.h) / 2;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}


void drawButton(SDL_Renderer *renderer, Button *button, TTF_Font *font, SDL_Color textColor, SDL_Color buttonColor){
    SDL_Rect drawRect;

    drawRect.w = button->rect.w;
    drawRect.h = button->rect.h;
    drawRect.x = button->rect.x - button->rect.w / 2;
    drawRect.y = button->rect.y - button->rect.h / 2;

    SDL_SetRenderDrawColor(renderer,
                           buttonColor.r,
                           buttonColor.g,
                           buttonColor.b,
                           255);

    SDL_RenderFillRect(renderer, &drawRect);
    drawText(renderer, font, button->text, textColor, drawRect);
}


void drawMenu(SDL_Renderer *renderer,int windowSize,TTF_Font *titleFont,TTF_Font *buttonFont,TTF_Font *authorFont, Button *buttons,int buttonCount){
    SDL_Color white = {255,255,255,255};
    SDL_Color gray  = {60,60,60,255};

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Rect titleArea = {0, 80, windowSize, 100};
    drawText(renderer, titleFont, "TIC TAC TOE GAME", white, titleArea);

    for (int i = 0; i < buttonCount; i++)
    {
        drawButton(renderer,
                   &buttons[i],
                   buttonFont,
                   white,
                   gray);
    }

    SDL_Rect authorArea = {(windowSize/2)- 50, windowSize - windowSize/8, 100, 100};
    drawText(renderer, authorFont, "@MBK", white, authorArea);
}

int isInside(Button *button, coordinate m)
{
    int left   = button->rect.x - button->rect.w / 2;
    int right  = button->rect.x + button->rect.w / 2;
    int top    = button->rect.y - button->rect.h / 2;
    int bottom = button->rect.y + button->rect.h / 2;

    return (m.x >= left && m.x <= right &&
            m.y >= top  && m.y <= bottom);
}

void initMenuButtons(Button *buttons, int windowSize)
{
    int buttonWidth  = 300;
    int buttonHeight = 70;
    int spacing      = 30;
    int startY       = 220;

    for (int i = 0; i < 2; i++)
    {
        buttons[i].rect.w = buttonWidth;
        buttons[i].rect.h = buttonHeight;

        buttons[i].rect.x = windowSize / 2;
        buttons[i].rect.y = startY + i * (buttonHeight + spacing) + buttonHeight / 2;
    }

    buttons[0].text = "Play Game";
    buttons[1].text = "Quit";
}

int gameLoop(){

    int runProgram = 1;
    coordinate inputCell;
    grid inputGrid;
    player turn = PLAYER1;
    char overlayText[MAX_SIZE];
    GameState gameState = STATE_MENU;
    Uint32 overlayStart = 0;
    int showOverlay = 0;
    Uint32 inputLockStart = 0;
    int inputLocked = 0;


    
    int windowSize = WINDOW_SIZE; ///remove all window sizes
    int cellSize = windowSize/3;


    Button menuButtons[2];
    initMenuButtons(menuButtons, windowSize);



    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("TTF Init Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Tic Tac Toe Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowSize,windowSize,0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
   
    if (!window){
        printf("Window Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    if (!renderer){
        printf("Rellnderer Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);
    TTF_Font *titleFont  = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 48);
    TTF_Font *buttonFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 28);
    TTF_Font *authorFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 11);

    if (!font || !titleFont || !buttonFont || !authorFont) {
        printf("Font load error: %s\n", TTF_GetError());
        return 1;
    }


    while (runProgram) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        while (SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_QUIT:
                    runProgram=0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE){runProgram=0;}
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    inputCell.x = event.button.x;
                    inputCell.y = event.button.y;
                    switch (gameState){
                        case STATE_MENU:
                            if (isInside(&menuButtons[0],inputCell)){
                                gameState = STATE_PLAYING;

                                overlayStart = SDL_GetTicks();
                                showOverlay = 1;
                                strcpy(overlayText, "PLAYER 1 TURN");
                            }
                            else if (isInside(&menuButtons[1], inputCell)){
                                runProgram = 0;
                            }
                            break;
                        case STATE_PLAYING:
                            inputGrid = cellChecker(cellSize, inputCell);
                            if (showOverlay || inputLocked){break;}
                            if (setMarker(turn, inputGrid)){
                                    inputLocked = 1;
                                    inputLockStart = SDL_GetTicks();
                                if (winCheck(turn)){
                                    if (turn == PLAYER1)
                                        strcpy(overlayText, ">>>PLAYER 1 WON!!!<<<");
                                    else
                                        strcpy(overlayText, ">>>PLAYER 2 WON!!!<<<");
                                    
                                    showOverlay = 0;
                                    inputLocked = 0;
                                    gameState = STATE_GAMEOVER;
                                }
                                else if (idleCheck()){
                                    strcpy(overlayText, "NO ONE WON!");
                                    showOverlay = 0;
                                    inputLocked = 0;
                                    gameState = STATE_GAMEOVER;
                                }
                                else {
                                    inputLocked = 1;
                                    inputLockStart = SDL_GetTicks();
                                }
                            }
                            break;
                        case STATE_GAMEOVER:
                            if (inputCell.x || inputCell.y){
                                runProgram=0;
                            }
                            break;
                    }
                    break;
            }
        }
        switch (gameState){
            case STATE_MENU:
                drawMenu(renderer, windowSize, titleFont, buttonFont, authorFont, menuButtons, 3);
                break;
            case STATE_PLAYING:
            case STATE_GAMEOVER:
                drawBorders(renderer, cellSize, windowSize);
                drawBoard(renderer, cellSize);
                break;
        }
        
        
        if (gameState == STATE_GAMEOVER) {
            gameOverlay(renderer, windowSize, font, overlayText);
        }
        if (showOverlay && gameState == STATE_PLAYING){
            if (SDL_GetTicks() - overlayStart < OVERLAY_DURATION){
                gameOverlay(renderer, windowSize, font, overlayText);
            }
            else{
                showOverlay = 0;
            }
        }
        if (inputLocked && gameState == STATE_PLAYING){
            if (SDL_GetTicks() - inputLockStart > INPUT_DELAY){

                inputLocked = 0;
                turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;

                overlayStart = SDL_GetTicks();
                showOverlay = 1;

                if (turn == PLAYER1)
                    strcpy(overlayText, "PLAYER 1 TURN");
                else
                    strcpy(overlayText, "PLAYER 2 TURN");
            }
        }
                
        
        SDL_RenderPresent(renderer);
        
    }
    
    return 0;
}