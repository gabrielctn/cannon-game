#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define GAME_WIDTH 10
#define GAME_HEIGHT 10
#define CASE_WIDTH 50
#define CASE_HEIGHT 50

typedef enum {
	EMPTY, BLACK_PAWN, RED_PAWN, RED_CITY, BLACK_CITY, BLACK_PAWN_SELECT, RED_PAWN_SELECT, POS_ERR
} Content;

typedef enum {
	BLACK, RED
} Player;

typedef enum {
	N,S,E,W,NW,NE,SW,SE,ERR
} Direction;

typedef enum { FALSE, TRUE } bool;

typedef struct Image {
	SDL_Surface* pRed;
	SDL_Surface* pBlack;
	SDL_Surface* pRedSelect;
	SDL_Surface* pBlackSelect;
	SDL_Surface* vBlack;
	SDL_Surface* vRed;
	SDL_Surface* board;
} Image;

typedef struct Message {
	SDL_Texture *tCities;
    SDL_Texture *tBlack;
    SDL_Texture *tRed;
    SDL_Texture *tWrongMove;
    SDL_Texture *tCannon;
    SDL_Texture *tCannonExtremity1;
    SDL_Texture *tCannonExtremity2;
    SDL_Texture *tCannonDir;
    SDL_Texture *tCannonChoice;
} Message;