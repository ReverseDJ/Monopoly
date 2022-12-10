#include <vector>
#include <fstream>
#include <string>
#include "functions.h"
#include <sstream>
#include <deque>

#define COMMUNITY_CHEST (1)
#define CHANCE (2)

extern std::deque<DeckCard*> chanceCards;
extern std::deque<DeckCard*> communityChestCards;

// read in chance, community chest from csv file 
// uses a double-ended queue (deque) to pop, push, and swap in O(1)
// add shuffle function later
void makeChance() {
    getMoneyCard * tempCard = new getMoneyCard(50, "dummy card", "dummy name", "ch00");
    chanceCards.push_back(dynamic_cast<DeckCard*>(tempCard));
}

void makeCommunityChest() {   
    getMoneyCard * tempCard = new getMoneyCard(50, "dummy card", "dummy name", "cc00");
    communityChestCards.push_back(dynamic_cast<DeckCard*>(tempCard));
}
