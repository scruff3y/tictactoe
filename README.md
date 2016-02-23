# tictactoe

Tic Tac Toe game in C. Built-in bot too!

Currently, the game can be made any size larger than 2x2 (though they will always be square). Also you have to edit the source and recompile to make the changes.

    NUMBER_OF_GAME_SQUARES
The size of one side of the game. It is recommended to keep this at 3 if robots are playing.

    numberOfPlayers
The number of players in the game. Note that you will have to add a line below stating that players character, and whether or not they are human. Again, keep this at 2 if *any* robots are playing (each bot simulates the turns of every other *player*, so keep it low).

How the bot works:
-

Recursively tests every possible game state from the current state. If a move wins the game for the bot, that move is assigned a score of 1. If it's a draw, that move is given the score of 0. If the game is not yet over, simulate the next move (by calling itself). If the move results in a loss, assign a score of -15.

At least, I *think* that's what it does.

Still to come:
-

Run-time changeable game size, number of players, high score table (number of wins).

Ability to also edit credentials of players (name for high-score table, tile to use in game).
