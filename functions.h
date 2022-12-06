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
        
        std::unordered_map<std::string, OwnableCard*>::iterator it = P->ownedCards.begin();
     
        while (it != P->ownedCards.end()){
            
        std::string propertyName = it->first;
        int mortgageValue = it->second->mort;
        std::cout << propertyName << " : " << mortgageValue << "\n";
        it++;
    }
}

bool checkBalance(Player * P, int money){
    if (P->money >= money){
        return true;
    }
    else if(P->ownedCards.empty() == false){
        std::cout << "You don't have enough money to pay this balance.\n";
        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n"; 
        
        while((P->money < money) && (P->ownedCards.empty() == false)){
            
        displayProperties(P);
        
        std::cout << "Which property would you like to mortgage?\n";
        
        std::string playerResponse;
        
        std::cin >> playerResponse;
        
        if (P->ownedCards.find(playerResponse) != P->ownedCards.end()){
             P->money = P->money + (P->ownedCards[playerResponse])->mort;
            
             (P->ownedCards[playerResponse])->mortgage();
        
              P->mortCards[playerResponse] = P->ownedCards[playerResponse];
        
              P->ownedCards.erase("playerResponse");
        
        }
        }
        
        checkBalance(P,money);
    }
    else{
        Bankruptcy(P); //needs to reference a global PlayerTurn object, which will be in the game loop
        return false;
    }
  }
    
 void Bankruptcy(Player* P, PlayerTurn* Turn){
    Turn->RemovePlayer(P);
    std::cout << P->name << " has gone bankrupt!\n";
  }

int checkMonopoly(Player * P, Card * C){
    
    std::string suitCard = C->cardID;
    int numCards = int(C->cardID[2]);
    int cardNum = int(C->cardID[3]);
    int monopolyCount;
    
    for (int i = 0; i < numCards; i++){
        if (i != cardNum){
            suitCard[3] = char(cardNum);
            if(P->ownedCards.find(suitCard) != P->ownedCards.end()){
                monopolyCount = monopolyCount + 1;
            }
        }
    }
    return monopolyCount;
}

