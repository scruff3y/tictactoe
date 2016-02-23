/*
Tic Tac Toe by scruff3y, 2016.

Game loop:
if: game is won, end.
else: next players turn.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// The number of squares on the game grid.
#define NUMBER_OF_GAME_SQUARES 3

/*
Game board notes:
No placement: -1
player 1 placement: 1
player 2 placement: 2
*/

typedef enum {FALSE = 0, TRUE = 1} boolean;

typedef struct {
	unsigned char tile[NUMBER_OF_GAME_SQUARES][NUMBER_OF_GAME_SQUARES];
} GameBoard;

typedef struct {
	unsigned char tile;
	boolean human;
	// Humans are asked which square to place a token in.
} Player;

typedef struct {
	Player * players;
	unsigned char numPlayers;
	unsigned char whosTurn;
} PlayerList;

/*
Returns TRUE if game is not over,
FALSE if game is a draw,
the character of the player who won, if the game is over.
*/
int IsWon(GameBoard);

// IsWon() helper. Returns true/false if a line is a winning line (i.e, all the same character).
int IsWinningLine(unsigned char * line);

// Does next turn.
void nextTurn(GameBoard * game, PlayerList * playerlist);

// Prints game board to stdout
void printBoard(GameBoard);

// Handles end-of-game.
void gameEnd(int gamestate);

// Takes game and produces turn.
long int bot(GameBoard game, int * row, int * col, PlayerList bot, char botTile);

// Inits player with arguments.
Player InitPlayer(unsigned char tile, boolean human);

int main(int argc, char * argv) {
	short numberOfPlayers = 2;
	GameBoard game;
	// init game board.
	for (int i = 0; i < NUMBER_OF_GAME_SQUARES; i++) {
		for (int j = 0; j < NUMBER_OF_GAME_SQUARES; j++) {
			game.tile[i][j] = 0x0;
		}
	}
	// done.

	// Temporary initialisation.
	PlayerList playerList;
	playerList.players = malloc(sizeof(Player) * numberOfPlayers);
	playerList.numPlayers = (char) numberOfPlayers;
	playerList.whosTurn = 0;

	playerList.players[0] = InitPlayer('X', TRUE);
	playerList.players[1] = InitPlayer('O', FALSE);
	// end temp init.

	while(TRUE) {
		printBoard(game);
		int gamestate = IsWon(game);
		if (gamestate != TRUE) {
			gameEnd(gamestate);
			exit(0);
		}
		else {
			/*
			do next turn.
			*/
			nextTurn(&game, &playerList);
		}
	}
	return 0;
}

/*
Returns TRUE if game is not over,
FALSE if game is a draw,
the character of the player who won, if the game is over.
*/
int IsWon(GameBoard game) {
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
			//printf("won on row or column, no draw (%i) (%i)\n", winOnCurrentRow, winOnCurrentCol);
			return game.tile[i][i];
		}
	}

	if (IsDrawn && (game.tile[NUMBER_OF_GAME_SQUARES-1][NUMBER_OF_GAME_SQUARES-1] != 0x0)) {
		//printf("won on draw\n");
		return FALSE;
	}
	else {
		if (winOnLDiag) {
			//printf("won on left diag\n");
			return game.tile[0][0];
		}
		if (winOnRDiag) {
			//printf("won on right diag\n");
			return game.tile[NUMBER_OF_GAME_SQUARES - 1][0];
		}
	}
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
				printf("An index was out of range.\n");
			}
			printf("Please try again: ");
			scanf("%i %i", &i, &j);
		}
		i = i - 1;
		j = j - 1;
		int c;
		while ((c = getc(stdin)) != '\n');
	}
	else {
		bot(*game, &i, &j, *playerlist, currentPlayer.tile);
		printf("%i %i\n", i+1, j+1);
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
	}
	else {
		printf("Congratulations Player '%c', you have won!\n", (char)gamestate);
	}
}

long int bot(GameBoard game, int * row, int * col, PlayerList playerlist, char botTile) {
	long int sum = 0;
	for (int i = 0; i < NUMBER_OF_GAME_SQUARES; i++) {
		for (int j = 0; j < NUMBER_OF_GAME_SQUARES; j++) {
			long int sumbefore = sum;
			if (game.tile[i][j] == 0x0) {
				*row = i;
				*col = j;
				game.tile[i][j] = playerlist.players[playerlist.whosTurn].tile;

				int isBotWon = IsWon(game);
				if (isBotWon == botTile) {
					return 10;
				}
				else if (isBotWon == FALSE) {
					return 0;
				}
				else if (isBotWon == TRUE) {
					PlayerList listCopy = playerlist;
					listCopy.whosTurn = (listCopy.whosTurn + 1) % listCopy.numPlayers;
					sum += bot(game, row, col, listCopy, botTile);
				}
				else {
					// game was won by someone else
					return -10;
				}
			}
			if (sum > sumbefore) {
				*row = i;
				*col = j;
			}
		}
	}
	return sum;
}

/*
note: playerlist.whosTurn is the index of the bot.

doMove{
	look for next open spot on board.
	play there.
	if: Game is won by bot, set output.
	else: doMove(newboard, &row, &col)
}
*/

Player InitPlayer(unsigned char tile, boolean human) {
	Player output;
	output.tile = tile;
	output.human = human;
	return output;
}