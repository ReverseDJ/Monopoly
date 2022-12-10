// functions.h

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "classes.h"

//uses players location to determine tile to buy;
void buyProperty(Player * P);

//called if player needs to pay rent or taxes, needs to check if player has enough money to cover rent value. If not, force to mortgage or initiate bankruptcy
bool checkBalance(Player * P, int money);

//takes in a player and a card and returns how many cards the player has that match the type of the passed in card
int checkMonopoly(Player * P, Card * C);

void displayProperties(Player * P); //Prints out all properties owned by the player, generally useful during the game loop.

void Bankruptcy(Player* P, PlayerTurn* Turn); //Removes Player from Player Turn list and declares them bankrupt

//Take a generic player and moves them to a generic space
//dest will be a valid spot in the board array
//inst determines if the code will be run to check if they pass go
void movePlayer(Player *P, int dest, bool inst);

void makeBoard(std::string filename, Tile ** Board);
void makeChance();
void makeCommunityChest();

//used to make a single dice roll
int diceRoll();
#endif 
