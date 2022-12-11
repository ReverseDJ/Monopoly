// Testing Chance, Community Chest

#include "functions.h"

Tile * Board[40];
Player * curP;
std::unordered_map<std::string,OwnableCard*> bankCards;

std::deque<DeckCard*> chanceCards;
std::deque<DeckCard*> communityChestCards;
PlayerTurn * activePlayers;


int main() {

    makeBoard("makeboard.csv", Board);
    makeCards("makechance.tsv", chanceCards);
    makeCards("makechest.tsv", communityChestCards);

    return 0;
}