#include "headers/global.h"
#include "headers/cannon.h"
#include "headers/show.h"
#include "headers/move.h"


//Main function of the cannon
bool cannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer, SDL_Renderer *renderer, Message *Infos, Image *Tiles){

	SDL_Rect posClick, extremity1, extremity2, selectDir;
	int nbSelections = 0; 
	bool goodSelection = FALSE, cannonOK = FALSE, targetOK, moveOK=FALSE;
	SDL_Rect cannonArray[3] = {{0,0}};

	//We wait the events to select two pawns
	SDL_Event event;
	showBoard(pawnsLayout, Tiles, renderer, Infos->tCannonExtremity1);
	do{
	    if (SDL_PollEvent(&event)){
	        switch(event.type){
		        case SDL_MOUSEBUTTONUP:
			        //We retrieve the position of the click as the number of cells
		        	posClick.x = (event.button.x / CASE_WIDTH);
	                posClick.y = (event.button.y / CASE_HEIGHT);

	                if(nbSelections == 0){
	             		goodSelection = selectPawns(pawnsLayout, posClick, activePlayer, &extremity1);
	             		showBoard(pawnsLayout, Tiles, renderer, Infos->tCannonExtremity2);
	             	} else{
	             		goodSelection = selectPawns(pawnsLayout, posClick, activePlayer, &extremity2);
	             	}
	             	if(goodSelection){
	             		nbSelections++;
	            		goodSelection = FALSE;
	            	}	
	        	break; 
	    	}	
	    }
	} while(nbSelections < 2);

    //Here, we've selected 2 pawns

    cannonOK = checkCannon(pawnsLayout, extremity1, extremity2, activePlayer, cannonArray);

    if(cannonOK){
    	showBoard(pawnsLayout, Tiles, renderer, Infos->tCannonDir);
    	while(!goodSelection){ //While we didn't select the 3rd pawn (the shot's direction)
        	SDL_WaitEvent(&event);
        	switch(event.type){
                case SDL_MOUSEBUTTONUP:
                	posClick.x = (event.button.x / CASE_WIDTH);
                	posClick.y = (event.button.y / CASE_HEIGHT);
                	goodSelection = selectPawns(pawnsLayout, posClick, activePlayer, &selectDir);
                break;
            }
        }
        //If the selection is really an extremity of the cannon
        if((selectDir.x == extremity2.x && selectDir.y == extremity2.y) || (selectDir.x == extremity1.x && selectDir.y == extremity1.y)){
        	Direction shotDirection = checkDirection(cannonArray, selectDir);
        	if(shotDirection != ERR){ //If it's a good direction
        							  //We choose the action "T" to shoot, and "D" to move
        		showBoard(pawnsLayout, Tiles, renderer, Infos->tCannonChoice);
        		bool actionCanon = FALSE;
        		while(!actionCanon){
        			SDL_WaitEvent(&event);
		        	switch(event.type){
		                case SDL_KEYUP:
		                	if(event.key.keysym.sym == SDLK_s){
		                		targetOK = checkTarget(pawnsLayout, activePlayer, cannonArray, selectDir, shotDirection);
		                		actionCanon=TRUE;
		                	}
		                	if(event.key.keysym.sym == SDLK_m){
		                		moveOK = moveCannon(pawnsLayout, cannonArray, selectDir, shotDirection, activePlayer);
		                		actionCanon=TRUE;
		                	}
		                break;
		            }
        		}
        	}	
        }
    }
    deselectCannon(pawnsLayout, cannonArray);
    showBoard(pawnsLayout, Tiles, renderer, Infos->tCannon);	
    if(targetOK || moveOK){ //If we shot with the cannon or it was moved
        return TRUE; //the cannon was used
    }
    return FALSE;
}

 

//Check that the selected cannon is actualy a cannon... (3 aligned pawns of the player)
bool checkCannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect extremity1, SDL_Rect extremity2, Player activePlayer, SDL_Rect cannonArray[]){

	SDL_Rect cellToTest = {0,0};
	Content caseOK, caseOKSelect;

	if(activePlayer == BLACK){
		caseOK = BLACK_PAWN;
		caseOKSelect = BLACK_PAWN_SELECT;
	} else{
		caseOK = RED_PAWN;
		caseOKSelect = RED_PAWN_SELECT;
	}

	//The cannon's extremities cannot be spaced by more than 2 cells
	if(abs(extremity1.x - extremity2.x) > 2 || abs(extremity1.y - extremity2.y) > 2){
		return FALSE;
	}

	//We check what is in the middle cell
	cellToTest.x = (extremity1.x + extremity2.x) / 2;
	cellToTest.y = (extremity1.y + extremity2.y) / 2;

	if(pawnsLayout[cellToTest.x][cellToTest.y] == caseOK){
		cannonArray[0].x = extremity1.x;
		cannonArray[0].y = extremity1.y;
		cannonArray[1].x = extremity2.x;
		cannonArray[1].y = extremity2.y;
		cannonArray[2].x = cellToTest.x;
		cannonArray[2].y = cellToTest.y;
		pawnsLayout[cellToTest.x][cellToTest.y] = caseOKSelect;
		return TRUE;
	}
	return FALSE;
}



//Checks the shot's direction
Direction checkDirection(SDL_Rect cannonArray[], SDL_Rect selectDir){
	//We compare the middle cell of the cannon with the selection
	//Their relative positions gives us the direction

	if(cannonArray[2].x + 1 == selectDir.x){ //The cell is on the right

		if(cannonArray[2].y + 1 == selectDir.y){ //The cell is below
			return SE;
		} else if(cannonArray[2].y - 1 == selectDir.y){ //The cell is up
			return NE;
		} else{ // Otherwise the cell is just on the right
			return E;
		}

	} else if(cannonArray[2].x - 1 == selectDir.x){ //The cell is on the left

		if(cannonArray[2].y + 1 == selectDir.y){ //The cell is below
			return SW;	
		} else if(cannonArray[2].y - 1 == selectDir.y){ //The cell is up
			return NW;
		} else{ // Otherwise the cell is just on the left
			return W;
		}

	} else if((cannonArray[2].x == selectDir.x) && (cannonArray[2].y + 1 == selectDir.y)){ // If it is the cell just below
		return S;
	} else if((cannonArray[2].x == selectDir.x) && (cannonArray[2].y - 1 == selectDir.y)){ // If it is the cell just up
		return N;
	}
	// Otherwise there is an error
	return ERR;
}



// Checks the targeted cells and makes the changes accordingly
bool checkTarget(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer, SDL_Rect cannonArray[], SDL_Rect selectDir, Direction shotDirection){
	// We check the content of the 3 cells that are on the shot's way
	int checkedCells = 0;
	SDL_Rect cellToTest, targetedCells[3];

	cellToTest.x = selectDir.x;
	cellToTest.y = selectDir.y;

	do{
		switch(shotDirection){
			case N : (cellToTest.y)--; break;
			case S : (cellToTest.y)++; break;
			case E : (cellToTest.x)++; break;
			case W : (cellToTest.x)--; break;
			case NE : (cellToTest.y)--; (cellToTest.x)++; break;
			case NW : (cellToTest.y)--; (cellToTest.x)--; break;
			case SE : (cellToTest.y)++; (cellToTest.x)++; break;
			case SW : (cellToTest.y)++; (cellToTest.x)--; break;
			default : return FALSE ; break;
		}

		if(checkedCells == 0 && (pawnsLayout[cellToTest.x][cellToTest.y] != EMPTY)){
			return FALSE;
		}

		if(activePlayer == BLACK && (pawnsLayout[cellToTest.x][cellToTest.y] == BLACK_PAWN || pawnsLayout[cellToTest.x][cellToTest.y] == BLACK_CITY)){
			return FALSE;
		}
		if(activePlayer == RED && (pawnsLayout[cellToTest.x][cellToTest.y] == RED_PAWN || pawnsLayout[cellToTest.x][cellToTest.y] == RED_CITY)){
			return FALSE;
		}
		
		targetedCells[checkedCells].x = cellToTest.x;
		targetedCells[checkedCells].y = cellToTest.y;
		checkedCells++;
	} while(checkedCells != 3);

	// If all the cells are OK we empty their content
	int i;
	for (i=0; i<=2; ++i){
		pawnsLayout[targetedCells[i].x][targetedCells[i].y] = EMPTY;
	}
	return TRUE;
}



bool moveCannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect cannonArray[], SDL_Rect selectDir, Direction shotDirection, Player activePlayer){

	SDL_Rect newCase = {0,0};
	newCase.x = selectDir.x;
	newCase.y = selectDir.y;

	switch(shotDirection){
		case N : (newCase.y)--; break;
		case S : (newCase.y)++; break;
		case E : (newCase.x)++; break;
		case W : (newCase.x)--; break;
		case NE : (newCase.y)--; (newCase.x)++; break;
		case NW : (newCase.y)--; (newCase.x)--; break;
		case SE : (newCase.y)++; (newCase.x)++; break;
		case SW : (newCase.y)++; (newCase.x)--; break;
		default : return FALSE ; break;
	}
	if((newCase.y < 0 || newCase.y > 9) || (newCase.x < 0 || newCase.x > 9)){
		return FALSE;
	}

	pawnsLayout[newCase.x][newCase.y] = (activePlayer == BLACK) ? BLACK_PAWN : RED_PAWN;

	if(selectDir.x == cannonArray[0].x && selectDir.y == cannonArray[0].y){
		pawnsLayout[(cannonArray[1].x)][(cannonArray[1].y)] = EMPTY;
	} else{
		pawnsLayout[(cannonArray[0].x)][(cannonArray[0].y)] = EMPTY;
	}
	return TRUE;
}



void deselectCannon(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect cannonArray[]){

	int i;
	for ( i=0; i<3; ++i)
	{
		if(pawnsLayout[cannonArray[i].x][cannonArray[i].y] == BLACK_PAWN_SELECT){
			pawnsLayout[cannonArray[i].x][cannonArray[i].y] = BLACK_PAWN;
		}
		if(pawnsLayout[cannonArray[i].x][cannonArray[i].y] == RED_PAWN_SELECT){
			pawnsLayout[cannonArray[i].x][cannonArray[i].y] = RED_PAWN;
		}
	}
}