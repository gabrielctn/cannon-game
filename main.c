
/* For compilation instructions check the README */

#include "headers/global.h"
#include "headers/initialize.h"
#include "headers/show.h"
#include "headers/move.h"
#include "headers/cannon.h"
#include "headers/computer.h"
#include "headers/save.h"

int main(int argc, char *argv[])
{
    //==================================DECLARATIONs OF VARIABLES======================================

    bool continueGame = TRUE, correctMove = FALSE, selectedPawn = FALSE;
    int nbBlackPawns, nbRedPawns, gameTypeChoice;
    Content pawnsLayout[GAME_WIDTH][GAME_HEIGHT] = {{EMPTY}};
    Player activePlayer = BLACK; //Black player starts
    SDL_Rect posClick={ 0, 0 }, selection={ 0, 0, CASE_WIDTH, CASE_HEIGHT };
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Image Tiles; // Game's images
    Message Infos; // Messages printed below the board

    initializeSDL();

    gameTypeChoice = chooseGameType(); // We show a short guide to play, and ask the player to choose to play against the computer, 
                                       // an other player, or reload the last game
	
    if(SDL_CreateWindowAndRenderer(GAME_WIDTH * CASE_WIDTH, (GAME_HEIGHT + 1) * CASE_HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
            exit(1);
    }
    SDL_SetWindowTitle(window, "Game of Thro...the Cannon");

    loadMedia(&Tiles, &Infos, renderer);
    
    //==================================END OF VARIABLE DECLARATIONS======================================
    
    srand(time(NULL)); // for random numbers
    
    startGame(pawnsLayout, &Tiles, gameTypeChoice, activePlayer, renderer, &Infos); //Start of the game: load last game or initialize board and the cities

    //======================================================================================================================================
    //==========================================================MAIN PROGRAM================================================================
    //======================================================================================================================================
    do{

    	//CASE WHEN WE PLAY AGAINST THE COMPUTER
    	if(activePlayer == RED && gameTypeChoice == 1){
    		computerPawnAction(pawnsLayout);
    		activePlayer = BLACK;
    	}

    	SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    nbBlackPawns = countPawns(pawnsLayout, BLACK);
                    nbRedPawns = countPawns(pawnsLayout, RED);

                    if(nbBlackPawns < nbRedPawns){
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","RED WINS !! Good job young fellow", NULL);
                    } else if(nbRedPawns < nbBlackPawns){
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","BLACK WINS !! Good job young fellow", NULL);
                    } else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over","PAR: no one wins !", NULL);
                    continueGame = FALSE;
                    saveGame(pawnsLayout, activePlayer);
                break;

                case SDL_MOUSEBUTTONUP:
                    //We retrieve the position as the number of cells
                	posClick.x = (event.button.x / CASE_WIDTH);
                	posClick.y = (event.button.y / CASE_HEIGHT);
                	//If a pawn is already selected, the click is for a move
                	if(selectedPawn == TRUE){
                		correctMove = pawnAction(pawnsLayout, posClick, activePlayer, selection);
                		//If the move is correct, the active player changes
                		if(activePlayer == BLACK && correctMove){
                            activePlayer = RED;
                        } else if(activePlayer == RED && correctMove){
                            activePlayer = BLACK;
                        }
                		selectedPawn = FALSE; 
                	} //Otherwise, the click is a selection
                	else {
                		selectedPawn = selectPawns(pawnsLayout, posClick, activePlayer, &selection);
                	}
                    if(activePlayer == BLACK) {
                        showBoard(pawnsLayout, &Tiles, renderer, Infos.tBlack);
                    } else {
                        showBoard(pawnsLayout, &Tiles, renderer, Infos.tRed);
                    }
                    continueGame = !endOfGame(pawnsLayout);
                    correctMove = FALSE;
                break;
                
                case SDL_KEYUP :
                    // The key "C" is used to select a cannon
                    if (event.key.keysym.sym == SDLK_c){
                        showBoard(pawnsLayout, &Tiles, renderer, Infos.tCannon);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Cannon", 
                            "Cannon activated: \n    - Select your cannon by clicking on its extremities\n    - Choose the direction of the shot or movement by clicking again on the extremity of the cannon that is going on the right direction", NULL
                        );
                        correctMove = cannon(pawnsLayout, activePlayer, renderer, &Infos, &Tiles);
                    }
                    //If the move is correct, the active player changes
                    if(activePlayer == BLACK && correctMove){
                        activePlayer = RED; 
                        showBoard(pawnsLayout, &Tiles, renderer, Infos.tRed);
                        continueGame = !endOfGame(pawnsLayout);
                    } else if(activePlayer == RED && correctMove){
                        activePlayer = BLACK;
                        showBoard(pawnsLayout, &Tiles, renderer, Infos.tBlack);
                       	continueGame = !endOfGame(pawnsLayout);
                    } else showBoard(pawnsLayout, &Tiles, renderer, Infos.tWrongMove);
                    correctMove = FALSE;

                    //The key "L" is used to load the last game
                    if(event.key.keysym.sym == SDLK_l){
                    	loadGame(pawnsLayout, &activePlayer);
                    	selectedPawn = FALSE;
                    	showBoard(pawnsLayout, &Tiles, renderer, Infos.tWrongMove);
                    }
            	break;
            }
        }
    } while(continueGame);
  	//======================================================================================================================================
    //=======================================================END OF THE MAIN PROGRAM========================================================
    //======================================================================================================================================

    // FREE & QUIT
    SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; 
}