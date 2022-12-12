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
    makeDrawCards("makechance.tsv", &chanceCards);
    makeDrawCards("makechest.tsv", &communityChestCards);

    std::cout << "CHANCE CARDS" << std::endl;
    printDeck(&chanceCards);
    std::cout << std::endl << "COMMUNITY CHEST CARDS" << std::endl;
    printDeck(&communityChestCards);

    return 0;
}