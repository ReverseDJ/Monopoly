#include <vector>
#include <fstream>
#include <string>
#include "functions.h"
#include <sstream>

#define OTHER (1)
#define PROPERTY (2)
#define RAILROAD (3)
#define TAX (4)
#define DRAWCARD (5)
#define UTILITY (6)

#define COMMUNITY_CHEST (1)
#define CHANCE (2)

// read property tiles
// MUST have global Board variable before calling this function!
// csv MUST be sorted by tile number

Tile * Board[40];
Player * curP;
std::unordered_map<std::string,OwnableCard*> bankCards;

std::deque<DeckCard*> chanceCards;
std::deque<DeckCard*> communityChestCards;

PlayerTurn * activePlayers;

int main(void) {
    // Tile * Board[40];
    makeBoard("makeboard.csv", Board);
    std::cout << dynamic_cast<PropertyTile*>(Board[1])->linkedCard->cardID << std::endl;
    std::cout << dynamic_cast<PropertyTile*>(Board[39])->linkedCard->saleValue << std::endl;
    std::cout << dynamic_cast<RailRoadTile*>(Board[5])->linkedCard->name << std::endl;
    std::cout << dynamic_cast<TaxTile*>(Board[38])->name << std::endl;
    std::cout << dynamic_cast<UtilityTile*>(Board[28])->linkedCard->mort << std::endl;

    makeDrawCards("makechance.tsv", &chanceCards);
    
    return 0;
}

