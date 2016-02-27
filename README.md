# tictactoe

Tic Tac Toe game in C. Built-in bot too!

Currently, the game can be made any size larger than 2x2 (though they will always be square). If you specify a game of size 2 or 3 and 2 players, you can choose to play against a bot I wrote.

The game size and the number of players are specified when the game is started.

Execute the program from the command-line with an argument (literally anything, it just checks if anything was passed, but not *what* was passed) to see a short help/about message.

By the way, this program is intented to be run from the command-line. ASCII art ftw! (and because I don't know how to make a UI yet!)

How the bot works:
-

Iteratively checks each space on the board. If no player has played there, simulate a move there (by making the move on a local copy of the game state). If that move resulted in a win, assign it a value of 10, and, if the current call to bot() is the 'root' call, make that move on the real board. Otherwise, if the move resulted in a draw, assign it a score of zero. Otherwise, if the move resulted in a loss for the bot(), assign that move a score of -10. Otherwise (i.e, game is not over), assign this move the return value of a call to bot(), passing this recursive call a copy of the modified game-state.

Add the score for this move to the bot's score. If the score for that move was the best it's seen so far, and the current call is the 'root' call, then set the output variables to the current space on the board.

Finally, return the bot's score.
