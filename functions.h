// functions.h
#include "classes.h"

//uses players location to determine tile to buy;
void buyProperty(player * P);

//called if player needs to pay rent or taxes, needs to check if player has enough money to cover rent value. If not, force to mortgage or initiate bankruptcy
bool checkBalance(Player * P, int money);

//takes in a player and a card and returns how many cards the player has that match the type of the passed in card
int checkMonopoly(Player * P, Card * C);

void displayProperties(Player * P); //Prints out all properties owned by the player, generally useful during the game loop.



void checkBalance(Player * P, int money){
    if (P->money >= money){
        return;
    }
    else if(P->ownedCards.empty() == false){
        cout << "You don't have enough money to pay this balance.\n";
        cout << "current balance: " << P->money << "\n";
        cout << "amount due: " << money << "\n"; 
        
        displayProperties(P);
        
        cout << "Which property would you like to mortgage?\n";
        
        cin >> std::string playerResponse;
        
        P->money = P->money + P->ownedCards[playerResponse].mortValue;
        
        (P->ownedCards[playerResponse]).linkedTile->isMortgaged = true;
        
        P->mortCards[playerResponse] = P->ownedCards[playerResponse];
        
        P->ownedCards.erase("playerResponse");
        
        checkBalance(P);
    }
    else{
        Bankruptcy(P);
    } 
 }

void displayProperties(Player * P){
    cout << "You own the following properties:\n";
        
        std::unordered_map<std::string, Card*>::iterator it = P->ownedCards.begin();
     
        while (it != P->ownedCards.end()){
            
        std::string propertyName = it->first;
        int mortgageValue = it->second;
        std::cout << propertyName << " : " << mortgageValue << "\n";
        it++;
    }
}
