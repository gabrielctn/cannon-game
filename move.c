#include "headers/global.h"
#include "headers/show.h"


int chooseGameType(){

	bool continueGame = TRUE, loadGame = FALSE;
	int computer = 1, twoPlayers = 2, opponent = 0;
    SDL_Event event;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
   
   	if(SDL_CreateWindowAndRenderer(810, 607, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetWindowTitle(window, "Rules of the Cannon Game");

    // We show the first window with the guide
	showGuide(window, renderer);

	while (continueGame){
        SDL_WaitEvent(&event);
        switch (event.type){
	        case SDL_QUIT:
	            continueGame = FALSE;
	            break;
	        case SDL_KEYUP :
	        	if (event.key.keysym.sym == SDLK_o){ // "O" : Play against the computer
		        	opponent = computer;
		        	continueGame = FALSE;
		        }
		        if (event.key.keysym.sym == SDLK_j){ // "J" : Play against another player
		        	opponent = twoPlayers;
		        	continueGame = FALSE;
		        }
		        if (event.key.keysym.sym == SDLK_l){ // "L" : Load the state of the last game's board
		        	loadGame = TRUE;
		        	continueGame = FALSE;
		        }
		    break;
        }
    }
    SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);

 	return (loadGame) ? 3 : opponent;
}



bool selectPawns(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect *selection){
// Check if where the player clicked is his own pawn
// If yes, "selection" retrieves the clicked position.
	bool selectedPawn = FALSE;

	if(activePlayer == BLACK && (pawnsLayout[posClick.x][posClick.y] == BLACK_PAWN || pawnsLayout[posClick.x][posClick.y] == BLACK_PAWN_SELECT)){
		pawnsLayout[posClick.x][posClick.y] = BLACK_PAWN_SELECT;
		selectedPawn = TRUE;
	}
	if(activePlayer == RED && (pawnsLayout[posClick.x][posClick.y] == RED_PAWN || pawnsLayout[posClick.x][posClick.y] == RED_PAWN_SELECT)){
		pawnsLayout[posClick.x][posClick.y] = RED_PAWN_SELECT;	
		selectedPawn = TRUE;
	}
	selection->x = posClick.x ;
	selection->y = posClick.y ;
	
	return selectedPawn;
}



bool checkStandStill(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select){

	if((posClick.y == select.y) && (posClick.x == select.x)){
		//If we click on a selected item, the item is deselected
		if(pawnsLayout[posClick.x][posClick.y] == BLACK_PAWN_SELECT){
			pawnsLayout[posClick.x][posClick.y] = BLACK_PAWN;
		} else{
			pawnsLayout[posClick.x][posClick.y] = RED_PAWN;
		}
		//We return 1, the item did not move, he stood still.
		return TRUE;
	}
	return FALSE;
}



bool movePawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select){

	Content pawn = (activePlayer == BLACK) ? BLACK_PAWN : RED_PAWN;
	Content enemyPawn = (activePlayer == BLACK) ? RED_PAWN : BLACK_PAWN;

	if(pawnsLayout[posClick.x][posClick.y] == EMPTY){
		//We move the pawn ahead
		if( (activePlayer == BLACK && ((posClick.y == select.y -1) && ((posClick.x >= select.x - 1) || (posClick.x <= select.x + 1)))) ||
			(activePlayer == RED && ((posClick.y == select.y +1) && ((posClick.x >= select.x -1) || (posClick.x <= select.x +1))))){
				pawnsLayout[posClick.x][posClick.y] = pawn;
				pawnsLayout[select.x][select.y] = EMPTY;
				return TRUE;
		}
		//Pawn's withdrawal
		else if(			   // An enemy pawn must be adjacent
			(activePlayer == BLACK && ((pawnsLayout[select.x - 1][select.y - 1] == enemyPawn || 
					 				 	 pawnsLayout[select.x][select.y - 1] == enemyPawn || 
					 				 	 pawnsLayout[select.x + 1][select.y - 1] == enemyPawn) 
									&& // Can withdraw two cells behind only
										((posClick.y == select.y + 2) && (posClick.x == select.x - 2 || posClick.x == select.x || posClick.x == select.x + 2)) 
									&& // The cell between the pawn and the selection must be empty
										(pawnsLayout[(posClick.x + select.x) / 2][(posClick.y + select.y) / 2] == EMPTY))) ||      
			
							    // an enemy pawn must be adjacent
			(activePlayer == RED && ((pawnsLayout[select.x - 1][select.y + 1] == enemyPawn || 
										  pawnsLayout[select.x][select.y + 1] == enemyPawn || 
										  pawnsLayout[select.x + 1][select.y + 1] == enemyPawn) 
									&& // Can withdraw two cells behind only
										((posClick.y == select.y - 2) && (posClick.x == select.x - 2 || posClick.x == select.x || posClick.x == select.x + 2)) 
									&& // The cell between the pawn and the selection must be empty
										(pawnsLayout[(posClick.x + select.x) / 2][(posClick.y + select.y) / 2] == EMPTY)))){
				pawnsLayout[posClick.x][posClick.y] = pawn;
				pawnsLayout[select.x][select.y] = EMPTY;
				return TRUE;
		}
	}
	return FALSE;
}



bool eatPawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select){

	Content pawn = (activePlayer == BLACK) ? BLACK_PAWN : RED_PAWN;
	Content enemyPawn = (activePlayer == BLACK) ? RED_PAWN : BLACK_PAWN;
	Content enemyCity = (activePlayer == BLACK) ? RED_CITY : BLACK_CITY;

	if(pawnsLayout[posClick.x][posClick.y] == enemyPawn || pawnsLayout[posClick.x][posClick.y] == enemyCity){
		if((activePlayer == RED && (((posClick.y == select.y + 1) || (posClick.y == select.y)) && 
								 ((posClick.x == select.x - 1) || (posClick.x == select.x + 1) || (posClick.x == select.x)))) ||
		   (activePlayer == BLACK && (((posClick.y == select.y -1) || (posClick.y == select.y) ) && 
								((posClick.x == select.x -1) || (posClick.x == select.x +1) || (posClick.x == select.x))))){
				pawnsLayout[posClick.x][posClick.y] = pawn;
				pawnsLayout[select.x][select.y] = EMPTY;
				return TRUE;
		}
	}
	return FALSE;
}



bool pawnAction(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect posClick, Player activePlayer, SDL_Rect select){

	bool ePawn, mPawn;
	Content pawn = (activePlayer == BLACK) ? BLACK_PAWN : RED_PAWN;

	//if the pawn stands still, movement is incorrect
	if(checkStandStill(pawnsLayout, posClick, activePlayer, select)){ 
		return FALSE;
	}

	ePawn = movePawn(pawnsLayout, posClick, activePlayer, select);

	mPawn = eatPawn(pawnsLayout, posClick, activePlayer, select);
	
	//If the movement is incorrect, we deselect the pawn
	if(!(ePawn || mPawn)) {
		pawnsLayout[select.x][select.y] = pawn;
		return FALSE;
	}
	return TRUE;
}



int countPawns(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer){

	int x, y, nbPawns = 0;
	Content pawn = (activePlayer == BLACK) ? BLACK_PAWN : RED_PAWN;
	Content pawnSelect = (activePlayer == BLACK) ? BLACK_PAWN_SELECT : RED_PAWN_SELECT;

	for(x=0; x<10; x++){
		for(y=0; y<10; y++){
			if(pawnsLayout[x][y] == pawn || pawnsLayout[x][y] == pawnSelect){
				nbPawns++;
			}
		}
	}
	return nbPawns;
}

