#ifndef DEF_MOVE
#define DEF_MOVE 



bool selectPawns(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect *selection);
bool pawnAction(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select);
bool checkStandStill(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select);
bool movePawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select);
bool eatPawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select);
int countPawns(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer);
int chooseGameType();



#endif