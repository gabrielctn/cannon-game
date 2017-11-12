#ifndef DEF_SHOW
#define DEF_SHOW 



void initBoard(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT]);
void showBoard(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Image *Tiles, SDL_Renderer *renderer, SDL_Texture *textureText);
void initCities(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Image *Tiles, SDL_Renderer *renderer, SDL_Texture *textureText, int gameTypeChoice);
void loadImages(Image *Tiles);
void loadMessages(Message *Infos, SDL_Renderer* renderer);
bool endOfGame(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT]);
void showGuide(SDL_Window *window, SDL_Renderer *renderer);




#endif