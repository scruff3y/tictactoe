// data header for tictactoe

// The number of squares on the game grid.
char NUMBER_OF_GAME_SQUARES = 3;

// Boolean enum type.
typedef enum {FALSE = (char)0, TRUE = (char)1} boolean;

/*
The game board store the tile of the player who played there, or 0x00 if noone
has played there yet.
*/
typedef struct {
	char tile[10][10];
} GameBoard;

// tile: char displayed on game board.
typedef struct {
	unsigned char tile;
	boolean human;
	// Humans are asked which square to place a token in.
} Player;

/*
Contains a list of players, the number of players and the index in the player
list of who's turn it currently is.
*/
typedef struct {
	Player * players;
	unsigned char numPlayers;
	unsigned char whosTurn;
} PlayerList;

typedef struct {
	GameBoard gameboard;
	PlayerList playerlist;
} GameData;

/*
Returns TRUE if game is not over,
FALSE if game is a draw,
the character of the player who won, if the game is over.
*/
char IsWon(GameBoard);

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

// Inits PlayerList.
PlayerList InitPlayerList(char numberOfPlayers);