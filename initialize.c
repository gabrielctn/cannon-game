#include "headers/global.h"
#include "headers/show.h"
#include "headers/save.h"


void initializeSDL(){

	//SDL initialisation
    SDL_Init(SDL_INIT_VIDEO); 
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Error when initializing SDL : %s\n", SDL_GetError()); 
        exit(1); 
    }
    atexit(SDL_Quit);
	//True Type Font
    if(TTF_Init() == -1){
        printf("Problem when initializing TTF : %s\n", TTF_GetError());
        exit(2);
    }
	srand(time(NULL)); // for random numbers
}



void loadMedia(Image *Tiles, Message *Infos, SDL_Renderer *renderer){

    loadImages(Tiles);
	loadMessages(Infos, renderer);
}



void startGame(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Image *Tiles, int gameTypeChoice, Player activePlayer, SDL_Renderer *renderer, Message *Infos){
	
	if(gameTypeChoice == 3){ // We load the last saved game where it stopped
	        loadGame(pawnsLayout, &activePlayer); 
	        if(activePlayer == BLACK) {
	            showBoard(pawnsLayout, Tiles, renderer, Infos->tBlack);
	        } else {
	            showBoard(pawnsLayout, Tiles, renderer, Infos->tRed);
	        }
	} else { // Initialize board and place cities
	        initBoard(pawnsLayout);
	        showBoard(pawnsLayout, Tiles, renderer, Infos->tCities);
	        initCities(pawnsLayout, Tiles, renderer, Infos->tCities, gameTypeChoice);
	        showBoard(pawnsLayout, Tiles, renderer, Infos->tBlack);
	    }
}