#ifndef DEF_INITIALIZE 
#define DEF_INITIALIZE 

void initializeSDL();
void loadMedia(Image *Tiles, Message *Infos, SDL_Renderer *renderer);
void startGame(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Image *Tiles, int gameTypeChoice, Player activePlayer, SDL_Renderer *renderer, Message *Infos);

#endif