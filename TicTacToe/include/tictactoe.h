#ifndef TICTACTOE_H
#define TICTACTOE_H


#include "tictactoe.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_SIZE 256
#define SIZE 3
#define WINDOW_SIZE 1000
#define OVERLAY_DURATION 1000
#define INPUT_DELAY 200 




typedef struct{
    int x;
    int y;
} coordinate;

typedef struct{
    int column;
    int row;
} grid;

typedef enum{
    EMPTY,
    PLAYER1,
    PLAYER2
} player;

typedef enum{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAMEOVER
} GameState;

typedef struct {
    SDL_Rect rect;
    const char *text;
} Button;


bool setMarker(player turn, grid localGrid);
grid cellChecker(int cellSize, coordinate cell);
bool winCheck(player mark);
bool idleCheck();

void drawBorders(SDL_Renderer *renderer, int cellSize, int windowSize);
void drawX(SDL_Renderer *renderer, int cellSize, grid square, int offset);
void drawO(SDL_Renderer *renderer, int cellSize, grid square, int offset);
void drawBoard(SDL_Renderer *renderer, int cellSize);

void gameOverlay(SDL_Renderer *renderer, int windowSize, TTF_Font *font, char* gameText);

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text,
              SDL_Color color, SDL_Rect container);

void drawButton(SDL_Renderer *renderer, Button *button, TTF_Font *font,
                SDL_Color textColor, SDL_Color buttonColor);

void drawMenu(SDL_Renderer *renderer,int windowSize,
              TTF_Font *titleFont,TTF_Font *buttonFont,
              TTF_Font *authorFont, Button *buttons,int buttonCount);

int isInside(Button *button, coordinate m);
void initMenuButtons(Button *buttons, int windowSize);

int gameLoop();

#endif