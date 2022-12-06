// functions.h
#include "classes.h"

//uses players location to determine tile to buy;
void buyProperty(player * P);

//called if player needs to pay rent or taxes, needs to check if player has enough money to cover rent value. If not, force to mortgage or initiate bankruptcy
bool checkBalance(Player * P, int money);

//takes in a player and a card and returns how many cards the player has that match the type of the passed in card
int checkMonopoly(Player * P, Card * C);

void displayProperties(Player * P); //Prints out all properties owned by the player, generally useful during the game loop.

void Bankruptcy(Player* P, PlayerTurn* Turn); //Removes Player from Player Turn list and declares them bankrupt

void displayProperties(Player * P){
    std::cout << "You own the following properties:\n";
        
        std::unordered_map<std::string, Card*>::iterator it = P->ownedCards.begin();
     
        while (it != P->ownedCards.end()){
            
        std::string propertyName = it->first;
        int mortgageValue = it->second->mortValue;
        std::cout << propertyName << " : " << mortgageValue << "\n";
        it++;
    }
}

void checkBalance(Player * P, int money){
    if (P->money >= money){
        return;
    }
    else if(P->ownedCards.empty() == false){
        std::cout << "You don't have enough money to pay this balance.\n";
        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n"; 
        
        displayProperties(P);
        
        std::cout << "Which property would you like to mortgage?\n";
        
        std::string playerResponse;
        
        std::cin >> playerResponse;
        
        if ((P->ownedCards[playerResponse]->type) == "RR")){
             P->money = P->money + dynamic_cast<RailRoadCard*>(P->ownedCards[playerResponse])->mortValue;
            
             (dynamic_cast<RailRoadCard*>(P->ownedCards[playerResponse]))->linkedTile->isMortgaged = true;
        
              P->mortCards[playerResponse] = P->ownedCards[playerResponse];
        
              P->ownedCards.erase("playerResponse");
        }
        else if ((P->ownedCards[playerResponse]->type) == "Property")){
            P->money = P->money + dynamic_cast<PropertyCard*>(P->ownedCards[playerResponse])->mortValue;
            
             (dynamic_cast<PropertyCard*>(P->ownedCards[playerResponse]))->linkedTile->isMortgaged = true;
        
              P->mortCards[playerResponse] = P->ownedCards[playerResponse];
        
              P->ownedCards.erase("playerResponse");
        }
        else if ((P->ownedCards[playerResponse]->type) == "Utility")){
            P->money = P->money + dynamic_cast<UtilityCard*>(P->ownedCards[playerResponse])->mortValue;
            
            (dynamic_cast<UtilityCard*>(P->ownedCards[playerResponse]))->linkedTile->isMortgaged = true;
        
             P->mortCards[playerResponse] = P->ownedCards[playerResponse];
        
             P->ownedCards.erase("playerResponse");
        }
        
        checkBalance(P,money);
    }
    else{
        Bankruptcy(P);
    }
  }
    
    void Bankruptcy(Player* P, PlayerTurn* Turn){
        Turn->RemovePlayer(P);
        std::cout << P->name " has gone bankrupt!\n";
    }
