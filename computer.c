#include "headers/global.h"
#include "headers/move.h"



void selectRandomPawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], SDL_Rect *posPawnToMove){

	int nbPawns = 0, numPawn = 0, comptePion = 0, x = 0, y = 0;

	nbPawns = countPawns(pawnsLayout, RED);
	numPawn = (rand() % nbPawns) + 1;

	//We go through the board
	for(x=0; x<10; x++){
		for(y=0; y<10; y++){

			if(pawnsLayout[x][y] == RED_PAWN) {
				comptePion++; //we count the number of pawns encountered
			}
			if(comptePion == numPawn && pawnsLayout[x][y] == RED_PAWN){ //if it is the number of the pawn randomly picked 
																		//this is the one we move
				posPawnToMove->x = x;
				posPawnToMove->y = y;
			}
		}
	}
} 




bool eatPawnForComputer(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Content surroundingCells[5], SDL_Rect *pawnPos){

	int i, moveToDo = 5;

	// We look at the computer's pawn's surroundings and store the content
	if(pawnPos->x > 0){ //left cell
		surroundingCells[0] = pawnsLayout[pawnPos->x - 1][pawnPos->y]; 
	} 
	if(pawnPos->x > 0 && pawnPos->y < 9) { //left front cell
		surroundingCells[1] = pawnsLayout[pawnPos->x - 1][pawnPos->y + 1]; 
	} 
	if(pawnPos->y < 9) { //front cell
		surroundingCells[2] = pawnsLayout[pawnPos->x][pawnPos->y + 1]; 
	} 
	if(pawnPos->x < 9 && pawnPos->y < 9) { //front right cell
		surroundingCells[3] = pawnsLayout[pawnPos->x + 1][pawnPos->y + 1]; 
	} 
	if(pawnPos->x < 9){ //right cell
		surroundingCells[4] = pawnsLayout[pawnPos->x + 1][pawnPos->y]; 
	} 

	for(i=0; i<5; i++){
		if(surroundingCells[i] == BLACK_CITY){
			moveToDo = i; 
			break;
		}
		if(surroundingCells[i] == BLACK_PAWN && moveToDo == 5) {
			moveToDo = i;
		}
	}
	// If an enemy is surrounding the computer's pawn, we kill it
	switch(moveToDo){
			case 0 : 
				pawnsLayout[pawnPos->x - 1][pawnPos->y] = RED_PAWN; 
				pawnsLayout[pawnPos->x][pawnPos->y] = EMPTY;
				return TRUE;
				break;
			case 1 : 
				pawnsLayout[pawnPos->x - 1][pawnPos->y + 1] = RED_PAWN;
				pawnsLayout[pawnPos->x][pawnPos->y] = EMPTY;
				return TRUE; 
				break;
			case 2 : 
				pawnsLayout[pawnPos->x][pawnPos->y + 1] = RED_PAWN; 
				pawnsLayout[pawnPos->x][pawnPos->y] = EMPTY;
				return TRUE; 
				break;
			case 3 : 
				pawnsLayout[pawnPos->x + 1][pawnPos->y + 1] = RED_PAWN; 
				pawnsLayout[pawnPos->x][pawnPos->y] = EMPTY;
				return TRUE; 
				break;
			case 4 : 
				pawnsLayout[pawnPos->x + 1][pawnPos->y] = RED_PAWN; 
				pawnsLayout[pawnPos->x][pawnPos->y] = EMPTY;
				return TRUE; 
				break;
	}

	return FALSE;
}



bool moveComputerPawn(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Content surroundingCells[6], SDL_Rect *pawnPos, SDL_Rect *cityPos){

	// We move towards the enemy city if possible
	if(cityPos->x < pawnPos->x && surroundingCells[1] == EMPTY){
		pawnsLayout[pawnPos->x - 1][pawnPos->y + 1] = RED_PAWN;
	} else if(cityPos->x > pawnPos->x && surroundingCells[3] == EMPTY){
		pawnsLayout[pawnPos->x + 1][pawnPos->y + 1] = RED_PAWN;
	} else if(cityPos->x == pawnPos->x && surroundingCells[2]==EMPTY){
		pawnsLayout[pawnPos->x][pawnPos->y + 1] = RED_PAWN;
	} else return FALSE;

	pawnsLayout[pawnPos->x][pawnPos->y] = EMPTY;

	return TRUE;
}



void computerPawnAction(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT]){

	int x, y;
	bool couldEat = FALSE, correctMove = FALSE;
	Content surroundingCells[5];
	SDL_Rect posPawnToMove, enemyCity;

	for(y=9; y>=0; y--){
		for(x=0; x<10; x++){
			if(pawnsLayout[x][y] == RED_PAWN){
				posPawnToMove.x = x;
				posPawnToMove.y = y;
				if(eatPawnForComputer(pawnsLayout, surroundingCells, &posPawnToMove)){
					return;
				}
			}
			if(pawnsLayout[x][y] == BLACK_CITY){
				enemyCity.x = x;
				enemyCity.y = y;
			}
		}
	}
	// Here, no pawn could kill, so we move a random one
	do{
		selectRandomPawn(pawnsLayout, &posPawnToMove);
		correctMove = moveComputerPawn(pawnsLayout, surroundingCells, &posPawnToMove, &enemyCity);
	} while(!correctMove);
}