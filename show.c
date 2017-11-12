#include "headers/global.h"
#include "headers/move.h"


void showGuide(SDL_Window *window, SDL_Renderer *renderer){

    SDL_Surface* guide = SDL_LoadBMP("images/guide.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, guide);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_FreeSurface(guide);
	SDL_DestroyTexture(texture);
}



// Initialises the positions of the pawns at the begining of the game
void initBoard(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT]){
	int x, y;

	for(y=1; y<=3; y++){
		for(x=1; x<=9; x++){
			if(x % 2 != 0) pawnsLayout[x][y] = RED_PAWN;
		}
	}

	for(y=6; y<=8; y++){
		for(x=0; x<=8; x++){
			if(x % 2 == 0) pawnsLayout[x][y] = BLACK_PAWN;
		}
	}
}



void loadImages(Image *Tiles){
	
	Tiles->pRed = SDL_LoadBMP("images/redPawn.bmp");
    Tiles->pBlack = SDL_LoadBMP("images/blackPawn.bmp");
    Tiles->pRedSelect = SDL_LoadBMP("images/redPawnSelect.bmp");
    Tiles->pBlackSelect = SDL_LoadBMP("images/blackPawnSelect.bmp");
    Tiles->vBlack = SDL_LoadBMP("images/blackCity.bmp");
    Tiles->vRed = SDL_LoadBMP("images/redCity.bmp");
    Tiles->board = SDL_LoadBMP("images/board.bmp");
}



void loadMessages(Message *Infos, SDL_Renderer* renderer){

	SDL_Color color = { 255, 255, 255 };
	TTF_Font *font;
	
	font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 18);
    if(font == NULL){
        fprintf(stderr, "Problem while loading the font: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface *tPlaceCities = TTF_RenderText_Blended(font, "Place your cities, black pawns start !", color);
    SDL_Surface *tBlackTurn = TTF_RenderText_Blended(font, "BLACK, your turn !", color);
    SDL_Surface *tRedTurn = TTF_RenderText_Blended(font, "RED, your turn!", color);
    SDL_Surface *tWrongMove = TTF_RenderText_Blended(font, "WRONG !! Incorrect move... TRY AGAIN !", color);
    SDL_Surface *tCannon = TTF_RenderText_Blended(font, "Cannon activated ! Wait for it...!", color);
    SDL_Surface *tCannonExtremity1 = TTF_RenderText_Blended(font, "Click on an extremity of your cannon", color);
	SDL_Surface *tCannonExtremity2 = TTF_RenderText_Blended(font, "Now click on the second extremity of the cannon", color);
	SDL_Surface *tCannonDir = TTF_RenderText_Blended(font, "OK, now click the extremity that is in the right direction", color);
	SDL_Surface *tCannonChoice = TTF_RenderText_Blended(font, "Now press S to shoot, or M to move", color);

	Infos->tCities = SDL_CreateTextureFromSurface(renderer,tPlaceCities);
    Infos->tBlack = SDL_CreateTextureFromSurface(renderer,tBlackTurn);
    Infos->tRed = SDL_CreateTextureFromSurface(renderer,tRedTurn);
    Infos->tWrongMove = SDL_CreateTextureFromSurface(renderer,tWrongMove);
    Infos->tCannon = SDL_CreateTextureFromSurface(renderer,tCannon);
    Infos->tCannonExtremity1 = SDL_CreateTextureFromSurface(renderer,tCannonExtremity1);
    Infos->tCannonExtremity2 = SDL_CreateTextureFromSurface(renderer,tCannonExtremity2);
    Infos->tCannonDir = SDL_CreateTextureFromSurface(renderer,tCannonDir);
	Infos->tCannonChoice = SDL_CreateTextureFromSurface(renderer,tCannonChoice);

    SDL_FreeSurface(tPlaceCities);
    SDL_FreeSurface(tBlackTurn);
    SDL_FreeSurface(tRedTurn);
    SDL_FreeSurface(tWrongMove);
    SDL_FreeSurface(tCannon);
    SDL_FreeSurface(tCannonExtremity1);
    SDL_FreeSurface(tCannonExtremity2);
    SDL_FreeSurface(tCannonDir);
	SDL_FreeSurface(tCannonChoice);
}



void showBoard(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Image *Tiles, SDL_Renderer *renderer, SDL_Texture *textureText){

	SDL_Texture *texture;
	SDL_Rect pos = {0,0,GAME_WIDTH*CASE_WIDTH,GAME_HEIGHT*CASE_HEIGHT};
	int x, y, textW = 0, textH = 0;

	SDL_RenderClear(renderer);

	texture = SDL_CreateTextureFromSurface(renderer, Tiles->board);
	SDL_RenderCopy(renderer, texture, NULL, &pos);

    SDL_QueryTexture(textureText, NULL, NULL, &textW, &textH);
    SDL_Rect dstrect = { 20, 510, textW, textH };
	SDL_RenderCopy(renderer, textureText, NULL, &dstrect);
    SDL_RenderPresent(renderer);

	pos.w = CASE_WIDTH;
	pos.h = CASE_HEIGHT;

	for(x=0; x<10; x++){
		for(y=0; y<10; y++){

			pos.x = x*CASE_WIDTH;
			pos.y = y*CASE_HEIGHT;
			switch(pawnsLayout[x][y]){
				case EMPTY : break;

				case RED_PAWN :
					texture = SDL_CreateTextureFromSurface(renderer, Tiles->pRed);
					SDL_RenderCopy(renderer, texture, NULL, &pos);
				break;

				case BLACK_PAWN :
					texture = SDL_CreateTextureFromSurface(renderer, Tiles->pBlack);
					SDL_RenderCopy(renderer, texture, NULL, &pos);
				break;

				case RED_PAWN_SELECT :
					texture = SDL_CreateTextureFromSurface(renderer, Tiles->pRedSelect);
					SDL_RenderCopy(renderer, texture, NULL, &pos);
				break;

				case BLACK_PAWN_SELECT :
					texture = SDL_CreateTextureFromSurface(renderer, Tiles->pBlackSelect);
					SDL_RenderCopy(renderer, texture, NULL, &pos);
				break;

				case BLACK_CITY : 
					texture = SDL_CreateTextureFromSurface(renderer, Tiles->vBlack);
					SDL_RenderCopy(renderer, texture, NULL, &pos);
				break;

				case RED_CITY :
					texture = SDL_CreateTextureFromSurface(renderer, Tiles->vRed);
					SDL_RenderCopy(renderer, texture, NULL, &pos);
				break;

				default: ;
				break;
			}
		}
	}
	SDL_RenderPresent(renderer);
}


void initCities(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Image *Tiles, SDL_Renderer *renderer, SDL_Texture *tText, int gameTypeChoice){

	SDL_Event event;
	int nbCities = 0, xposComputerCity;
	SDL_Rect posClick = {0,0};

	while(nbCities != 2){
		SDL_WaitEvent(&event);
	    switch(event.type){
	        case SDL_MOUSEBUTTONUP:
	        	posClick.x = (event.button.x / CASE_WIDTH);
	        	posClick.y = (event.button.y / CASE_HEIGHT);

	        	if(gameTypeChoice == 1 && nbCities == 1){ // The computer picks a random x coordonate between 1 and 8 for his red city
	        		xposComputerCity = 1 + (rand() % 8);
	        		pawnsLayout[ xposComputerCity ][ 0 ] = RED_CITY;
	        		nbCities++;
	        		showBoard(pawnsLayout, Tiles, renderer, tText);
	        	}
	        	if((posClick.y == 9 && posClick.x != 0 && posClick.x != 9) && nbCities == 0){ // Black city first
	        		pawnsLayout[ posClick.x ][ posClick.y ] = BLACK_CITY ;
	        		nbCities++;
	        		showBoard(pawnsLayout, Tiles, renderer, tText);
	 			}
	        	if((posClick.y == 0 && posClick.x != 0 && posClick.x != 9) && nbCities == 1){ // Red city after
	        		pawnsLayout[ posClick.x ][ posClick.y ] = RED_CITY ;
	        		nbCities++;
	        		showBoard(pawnsLayout, Tiles, renderer, tText);
	    		}
	    	break;
	    }
	}
}



bool endOfGame(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT]){

	int x, y = 0;
	bool redCity = FALSE, blackCity = FALSE, blackOnCityLine = FALSE, RedOnCityLine = FALSE;
	int nbRedPawns = 0, nbBlackPawns = 0;

	//We ckeck if the cities are still present
	//and if an enemy pawn is on the ligne of the city
	for(x=0; x<10; x++){
		if(pawnsLayout[x][y] == RED_CITY){
			redCity = TRUE;
		}
		if(pawnsLayout[x][y] == BLACK_PAWN){
			blackOnCityLine++;
		}
	}
	y=9;
	for(x=0; x<10; x++){
		if(pawnsLayout[x][y] == BLACK_CITY){
			blackCity = TRUE;
		}
		if(pawnsLayout[x][y] == RED_PAWN){
			RedOnCityLine++;
		}
	}

	// If a city is missing
	if(!blackCity){ 
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","RED WINS !! Good job young fellow", NULL);
		return TRUE; // Game is over
	} else if(!redCity){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","BLACK WINS !! Good job young fellow", NULL);
		return TRUE; // Game is over
	}

	// Otherwise we count the pawns
	nbBlackPawns = countPawns(pawnsLayout, BLACK);
	nbRedPawns = countPawns(pawnsLayout, RED);

	// If all of a player's pawns are on the line of the city, he has no more possible moves. He looses
	if(nbBlackPawns == blackOnCityLine) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","RED WINS !! Good job young fellow", NULL);
		return TRUE;
	}
	if(nbRedPawns == RedOnCityLine) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","BLACK WINS !! Good job young fellow", NULL);
		return TRUE;
	}

	// If a player has no pawns left
	return (nbBlackPawns == 0 || nbRedPawns == 0);
}