// functions.h
#include "classes.h"

//uses players location to determine tile to buy;
void buyProperty(player * P);

//called if player needs to pay rent or taxes, needs to check if player has enough money to cover rent value. If not, force to mortgage or initiate bankruptcy
bool checkBalance(Player * P, int rent);

//takes in a player and a card and returns how many cards the player has that match the type of the passed in card
int checkMonopoly(Player * P, Card * C);

