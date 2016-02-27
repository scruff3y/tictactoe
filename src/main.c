/*
Tic Tac Toe by scruff3y, 2016.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "../include/data.h"

int main(int argc, char * argv) {
	if (argc > 1) {
		printf("\n\tTicTacToe by scruff3y, 2016.\n\n");
		printf("How to play:\n");
		printf("The rules are the same as regular tic-tac-toe, fill a row, column or diagonal\n");
		printf("with your tile, and you win!\n\n");
		printf("To make a move, enter the row and column number of the tile you'd like to play on.\n\n");
		printf("If you really want to get a party going, try playing on a size 10 board\n");
		printf("with 10 players!\n\n");
		printf("Or, for the masochists, play a game against the computer. Hint: you will lose.\n");
		printf("(This is only availiable on a board size of 2 or 3\n");
		exit(0);
	}

	// This is just used to clear stdin between inputs.
	int c;

	// Welcome.
	printf("\n\tWelcome to TicTacToe!\n\n");
	printf("What size game would you like to play?: ");
	scanf("%i", &NUMBER_OF_GAME_SQUARES);

	while ((NUMBER_OF_GAME_SQUARES < 2) || (NUMBER_OF_GAME_SQUARES > 10)) {
		while ((c = getc(stdin)) != '\n');

		printf("Sorry, the game size must be between 2 and 10 squares.\n");
		printf("Please enter another value: ");
		scanf("%i", &NUMBER_OF_GAME_SQUARES);
	}

	while ((c = getc(stdin)) != '\n');

	// Init game board.
	GameBoard game = {0};

	char numberOfPlayers = 0;
	printf("How many players?: ");
	scanf("%i", &numberOfPlayers);

	while ((numberOfPlayers < 2) || (numberOfPlayers > 26)) {
		while ((c = getc(stdin)) != '\n');

		printf("Please specify a number between 2 and 26: ");
		scanf("%i", &numberOfPlayers);
	}

	// init playerlist.
	PlayerList playerList = InitPlayerList(numberOfPlayers);

	if ((numberOfPlayers == 2) && (NUMBER_OF_GAME_SQUARES <= 3)) {
		while ((c = getc(stdin)) != '\n');
		printf("Human or computer opponent (Y/N)?: ");
		char input;
		scanf("%c", &input);

		while ((input != 'Y') && (input != 'N') && (input != 'y') && (input != 'n')) {
			while ((c = getc(stdin)) != '\n');

			printf("Please try again: ");
			scanf("%c", &input);
		}

		if ((input == 'Y') || (input == 'y')) playerList.players[0].human = FALSE;
	}

	/*
	Main game loop:

	1. Check if game is over.
	2. If it isn't, do the next turn (data on whos turn it is stored in PlayerList)
	3. Otherwise, exit.
	*/
	while (TRUE) {
		printBoard(game);
		int gamestate = IsWon(game);
		if (gamestate != TRUE) {
			gameEnd(gamestate);
			exit(0);
		}
		else {
			nextTurn(&game, &playerList);
		}
	}
	return 0;
}

/*
Returns TRUE if game is not over,
FALSE if game is a draw,
the character of the player who won, if the game is over.

Uses a nested for-loop to search the game board to see if the game has been won.
The first loop (i) checks the diagonals.
The second loop (j) checks row i and column i. 

Each row/column is iteratively checked to see if any two adjacent values are
different. If they are, then the respective boolean:
(winOn[CurrentRow | CurrentCol | LDiag | RDiag]) is set to FALSE.
If, after traversing the row and column, the respective boolean is TRUE,
then returns the value of the player who has won.
*/
char IsWon(GameBoard game) {
	boolean IsDrawn = TRUE;

	boolean winOnRDiag = TRUE;
	boolean winOnLDiag = TRUE;

	boolean winOnCurrentRow = TRUE;
	boolean winOnCurrentCol = TRUE;

	for (int i = 0; i < NUMBER_OF_GAME_SQUARES; i++) {
		int nexti = (i+1) % NUMBER_OF_GAME_SQUARES;
		if(game.tile[i][i] != game.tile[nexti][nexti]) winOnLDiag = FALSE;
		if(game.tile[NUMBER_OF_GAME_SQUARES - 1 - i][i] != game.tile[NUMBER_OF_GAME_SQUARES - 1 - nexti][nexti]) winOnRDiag = FALSE;

		if(game.tile[i][i] == 0x0) winOnLDiag = FALSE;
		if(game.tile[NUMBER_OF_GAME_SQUARES - 1 - i][i] == 0x0) winOnRDiag = FALSE;

		winOnCurrentRow = TRUE;
		winOnCurrentCol = TRUE;		

		for (int j = 0; j < NUMBER_OF_GAME_SQUARES-1; j++) {
			int nextj = (j+1) % NUMBER_OF_GAME_SQUARES;

			if(game.tile[i][j] != game.tile[i][nextj]) winOnCurrentRow = FALSE;
			if(game.tile[j][i] != game.tile[nextj][i]) winOnCurrentCol = FALSE;
			if(game.tile[i][j] == 0x0) {
				winOnCurrentRow = FALSE;
				IsDrawn = FALSE;
			}
			if(game.tile[j][i] == 0x0) {
				winOnCurrentCol = FALSE;
				IsDrawn = FALSE;
			}
		}
		if (winOnCurrentRow || winOnCurrentCol) {
			return game.tile[i][i];
		}
	}

	if (!winOnLDiag && IsDrawn && (game.tile[NUMBER_OF_GAME_SQUARES-1][NUMBER_OF_GAME_SQUARES-1] != 0x0)) {
		// Game was drawn.
		return FALSE;
	}
	else {
		if (winOnLDiag) {
			return game.tile[0][0];
		}
		if (winOnRDiag) {
			return game.tile[NUMBER_OF_GAME_SQUARES - 1][0];
		}
	}
	// Game has not ended.
	return TRUE;
}

// Does next turn.
void nextTurn(GameBoard * game, PlayerList * playerlist) {
	/*
	If the current player is human, ask where they want to play.
	otherwise, pass bot gamestate and do what it passes back.
	*/
	int i = NUMBER_OF_GAME_SQUARES+1;
	int j = NUMBER_OF_GAME_SQUARES+1;

	Player currentPlayer = playerlist->players[(int) playerlist->whosTurn];

	printf("Player %c: where do you play?: ", currentPlayer.tile);

	if (currentPlayer.human) {
		scanf("%i %i", &i, &j);
		while ((i < 1) ||
			(i > NUMBER_OF_GAME_SQUARES) ||
			(j < 1) ||
			(j > NUMBER_OF_GAME_SQUARES) ||
			(game->tile[i-1][j-1] != 0x0)) {
			if (!((i < 1) || (i > NUMBER_OF_GAME_SQUARES) || (j < 1) || (j > NUMBER_OF_GAME_SQUARES))) {
				printf("Player %c has played there already.\n", game->tile[i-1][j-1]);
			}
			else {
				printf("An index was out of range or invalid.\n");
			}
			printf("Please try again: ");
			// Clear stdin...
			int c;
			while ((c = getc(stdin)) != '\n');
			scanf("%i %i", &i, &j);
		}
		i = i - 1;
		j = j - 1;

		// Clear stdin...
		int c;
		while ((c = getc(stdin)) != '\n');
	}
	else {
		bot(*game, &i, &j, *playerlist, currentPlayer.tile);
		printf("%i %i\n", i+1, j+1);
		// ^ Bot prints line to screen as if it were playing like a person.
	}
	game->tile[i][j] = currentPlayer.tile;
	playerlist->whosTurn += 1;
	playerlist->whosTurn %= playerlist->numPlayers;
}

// Prints game board to stdout
void printBoard(GameBoard game) {
	printf("\n  ");
	for (int i = 1; i <= NUMBER_OF_GAME_SQUARES; i++) {
		printf("%i ", i);
	}
	printf("\n");
	for (int i = 1; i <= NUMBER_OF_GAME_SQUARES; i++) {
		printf("%i ", i);
		for (int j = 0; j < NUMBER_OF_GAME_SQUARES; j++) {
			if (game.tile[i-1][j]) {
				printf("%c ", game.tile[i-1][j]);
			}
			else {
				printf("- ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

// Handles end-of-game.
void gameEnd(int gamestate) {
	if (gamestate == FALSE) {
		printf("It's a draw!\n");
	}
	else if (gamestate == TRUE) {
		printf("[DEBUG] Something has gone terribly wrong.\n");
		// gameEnd() is only called if gameState != true...
	}
	else {
		printf("Congratulations Player '%c', you have won!\n", (char)gamestate);
	}
}

/*
Tic Tac Toe bot.

Recursively checks every subsequent game board state after the current one to 
determine best move. I limited the game size to three because I couldn't get it
to output a move in a reasonable amount of time at any higher level.
*/
long int bot(GameBoard game, int * row, int * col, PlayerList playerlist, char botTile) {
	// The address of this variable is passed to the recursive calls to bot().
	// This allows the root call to differentiate itself from all other calls.
	static int fakeRow;
	boolean IsRoot = (&fakeRow != row);

	// If the current move results in a loss next turn, this is set to TRUE.
	// The bot will *not* play there, regardless of score.
	boolean NOPE = FALSE;

	int myScore = 0;
	// Highscore defaults to a super-low value, this is because the "best" move
	// to make might still ultimately lead to a loss.
	long int highscore = -500000;

	// copy of playerlist passed to next bot level.
	PlayerList listCopy = playerlist;
	listCopy.whosTurn = (listCopy.whosTurn + 1) % listCopy.numPlayers;

	for (char i = 0; i < NUMBER_OF_GAME_SQUARES; i++) {
		for (char j = 0; j < NUMBER_OF_GAME_SQUARES; j++) {
			int scoreForThisMove = 0;

			if (game.tile[i][j] == 0x0) {

				game.tile[i][j] = playerlist.players[playerlist.whosTurn].tile;
				char isBotWon = IsWon(game);

				if (isBotWon == botTile) {
					if (IsRoot) {
						// Playing here results in a win. Don't bother with anywhere else.
						*row = i;
						*col = j;
						return 0;
					}
					scoreForThisMove = 10;
				}
				else if (isBotWon == FALSE) {
					// Game drawn.
					scoreForThisMove = 0;
				}
				else if (isBotWon == TRUE) {
					scoreForThisMove = bot(game, &fakeRow, &fakeRow, listCopy, botTile);
					if ((scoreForThisMove == -10) && (IsRoot)) {
						// Playing here results in a loss next turn. Do *not* play here.
						NOPE = TRUE;
					}
				}
				else {
					// Game lost.
					scoreForThisMove = -10;
				}

				myScore = myScore + scoreForThisMove;

				if ((scoreForThisMove > highscore) && (!NOPE) && IsRoot) {
					highscore = scoreForThisMove;
					*row = i;
					*col = j;
				}

				// Reset game tile back before checking next move.
				game.tile[i][j] = 0x0;
			}
		}
	}
	return myScore;
}

// Initialises a Player.
Player InitPlayer(unsigned char tile, boolean human) {
	Player output;
	output.tile = tile;
	output.human = human;
	return output;
}

/*
Initialises the PlayerList.

The first two players are given the tiles 'X' and 'O' like traditional naughts
and crosses, and all subsequent players are given the letters A through W
(skipping O of course).
*/
PlayerList InitPlayerList(char numberOfPlayers) {
	PlayerList playerList;
	playerList.players = malloc(sizeof(Player) * numberOfPlayers);
	playerList.numPlayers = numberOfPlayers;
	playerList.whosTurn = 0;

	// inits first two players to X and O
	playerList.players[0] = InitPlayer('X', TRUE);
	playerList.players[1] = InitPlayer('O', TRUE);

	char currentLetter = 0;

	for(char i = 0; i < numberOfPlayers; i++) {
		char offset = 2;
		if (currentLetter == 'O') {
			offset--;
			i++;
		}
		currentLetter = i + 0x41;
		playerList.players[i+offset] = InitPlayer(currentLetter, TRUE);
	}
	return playerList;
}