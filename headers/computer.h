#ifndef DEF_COMPUTER
#define DEF_COMPUTER 

void computerPawnAction(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT]);
bool eatPawnForComputer(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Content surroundingCells[5], SDL_Rect *pawnPos);
void selectRandomPawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect *posPawnToMove);
bool moveComputerPawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Content surroundingCells[5], SDL_Rect *pawnPos, SDL_Rect *cityPos);


#endif