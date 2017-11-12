#ifndef DEF_CANNON
#define DEF_CANNON 


bool cannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer, SDL_Renderer *renderer, Message *Infos, Image *Tiles);
bool checkCannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect extremite1, SDL_Rect extremite2, Player activePlayer, SDL_Rect cannonArray[]);
Direction checkDirection(SDL_Rect cannonArray[], SDL_Rect selectDir);
bool checkTarget(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer, SDL_Rect cannonArray[],SDL_Rect selectDir, Direction shotDirection);
void deselectCannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect cannonArray[]);
bool moveCannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect cannonArray[], SDL_Rect selectDir, Direction shotDirection, Player activePlayer);



#endif