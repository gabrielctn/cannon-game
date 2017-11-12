#include "headers/global.h"


void saveGame(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player activePlayer){

	FILE* OUT = NULL;
	OUT = fopen("saveGame.txt", "w+");
	if(OUT != NULL){
		int x,y;

		/*Pawn's positions are written in a file
		We go through the board and check the content of the cells*/

		for(x=0; x<10; x++){
			for(y=0; y<10; y++){

				switch(pawnsLayout[x][y]){
					
					case EMPTY : fprintf(OUT, "0"); break;

					case BLACK_PAWN : fprintf(OUT, "1"); break;
					case BLACK_PAWN_SELECT : fprintf(OUT, "1"); break;

					case RED_PAWN : fprintf(OUT, "2"); break;
					case RED_PAWN_SELECT : fprintf(OUT, "2"); break;

					case RED_CITY : fprintf(OUT, "3"); break;

					case BLACK_CITY : fprintf(OUT, "4"); break;

					default: fprintf(OUT, "0"); break;
				}
			}
		}

		fprintf(OUT, "\n");
		//First line contains the pawns, the second says who's turn it is

		if(activePlayer == BLACK){
			fprintf(OUT, "0");
		}else{
			fprintf(OUT, "1");
		}

		fclose(OUT);
	}
}

//Loads the board and return the active player
void loadGame(Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT], Player* activePlayer){
	
	FILE* IN = NULL;
	IN = fopen("saveGame.txt", "r");
	int presentChar = 0;

	if(IN != NULL){
		int x, y;
		
		for(x=0; x<10; x++){
			for(y=0; y<10; y++){

				presentChar = fgetc(IN);

				switch(presentChar){
					case '0' : pawnsLayout[x][y] = EMPTY; break;
					case '1' : pawnsLayout[x][y] = BLACK_PAWN; break;
					case '2' : pawnsLayout[x][y] = RED_PAWN; break;
					case '3' : pawnsLayout[x][y] = RED_CITY; break;
					case '4' : pawnsLayout[x][y] = BLACK_CITY; break;
					default: pawnsLayout[x][y] = EMPTY; break;
				}
			}
		}
		presentChar = fgetc(IN);
		presentChar = fgetc(IN);

		fclose(IN);

		*activePlayer = (presentChar == 0) ? BLACK : RED;
	}
}