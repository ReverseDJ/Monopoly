# ECE-309-Monopoly

Our project implements monopoly. 
All of the rules are explained on this page: https://www.ultraboardgames.com/monopoly/game-rules.php

Our code can be compiled through our make file, which creates an executable. Our code uses C++ 14 and, as far as we know, there are no Operating System or other dependencies.

We employed the following rule simplifications.

- There is no trading or bargaining between players in our implementation

- When the player gets out of jail, they stay on the jail space until their next turn.

- Players are not required to buy/hold the same number of houses on each space of a monopoly. When the player has a monopoly, houses and hotels can be bought individually for each property.

Additionally, because there are over a hundred cards with data that needed to be instantiated as our objects, we wrote a program to take the data from a TSV (tab separated value) file and instantiate all of our card and tile objects.

Bot players are created by naming a player some string containing the word BOT, such as BOT123, testBOT, etc.

There are no known errors with our game when played only by console players. 

However, our computer player has the following issues.

- Does not unmortgage properties

- Causes a segmentation fault when the computer player goes bankrupt

Additionally, when running our make file, we get an error that our computer does not support the software.


## Running the program

"make all" compiles all source file into executable Monopoly.exe
"make run" starts a Monopoly game. It does not set up the game with computer players. The user must input a number of players and their names.
To run a game with only bot players, create any number of players that have "BOT" (case-sensitive) somewhere in their name.

To quit, press Ctrl+C.
To test out any functionality of the game, select the '1' option to manually input player movements when prompted. This will allow you to move each player (including bots) to any location on the board at the start of each turn.



