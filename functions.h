// functions.h

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "classes.h"


//uses players location to determine tile to buy;
void buyProperty(Player * P);
void botBuyProperty(Player * P);

//called if player needs to pay rent or taxes, needs to check if player has enough money to cover rent value. If not, force to mortgage or initiate bankruptcy
bool checkBalance(Player * P, int money,bool optional, Player * debtor);
bool checkBotBalance(Player * P, int money, Player * debtor);
bool trackBotSpending(Player *P);

//takes in a player and a card and returns how many cards the player has that match the type of the passed in card
int checkMonopoly(Player * P, Card * C);

void displayProperties(Player * P); //Prints out all properties owned by the player, generally useful during the game loop.
void displayMortgaged(Player * P); // prints mortgaged properties

void displayMortgaged(Player * P);

void Bankruptcy(Player* P, PlayerTurn Turn, Player * debtor); //Removes Player from Player Turn list and declares them bankrupt

//Take a generic player and moves them to a generic space
//dest can be an out-of-bounds location, uses % to make it a valid spot in the board array
//if inst = true then the player "teleports", doesn't pass go, doesn't collect 200
void movePlayer(Player *P, int dest, bool inst);

void makeBoard(std::string filename, Tile ** Board);
void makeDrawCards(std::string filename, std::deque<DeckCard*> * deck);
void printDeck(std::deque<DeckCard*> * deck);
void mixCards(std::deque<DeckCard*> &q); // shuffle chance/chest cards

//used to make a single dice roll
int diceRoll();

//called at the end of player turn so player can buy houses
void buyHouses(Player * P);
void botBuyHouses(Player * P);

void printStartTurn(Player * P);

//handles player turn while player is in jail. Returns true if player gets out of jail. False otherwise.
bool inJail(Player * P);
bool checkGOOJ(Player * P); // Jail helper function



// MACROS
#define BOT(P) ((P->name).find("BOT") != std::string::npos)
// detroit become human
#define BH_CHECKBALANCE(P, money, opt, debtor) { \
    if (BOT(P)) checkBotBalance(P, money, debtor);\
    else checkBalance(P, money, opt, debtor);\
}

#define BH_BUYPROPERTY(P) { \
    if (BOT(P)) botBuyProperty(P); \
    else buyProperty(P);\
}

#endif 
