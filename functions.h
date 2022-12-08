// functions.h
#include "classes.h"
#include <iostream>
#include <list>
extern PlayerTurn * activePlayers;

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

void displayProperties(Player * P){
    std::cout << "You own the following properties:\n";
        
        std::unordered_map<std::string, OwnableCard*>::iterator it = P->ownedCards.begin();
     
        while (it != P->ownedCards.end()){
            
            std::string propertyName = it->first;
            int mortgageValue = it->second->mort;

            if (it->second->type == "Property"){
                int numHouses = (dynamic_cast<PropertyCard*>(it->second))->linkTile->houseNum;
                int numHotels = (dynamic_cast<PropertyCard*>(it->second))->linkTile->hotelNum;
                std::cout << propertyName << " : " << mortgageValue << " houses: " << numHouses << " hotels: " << numHotels << "\n";
            }else{
            std::cout << propertyName << " : " << mortgageValue << "\n";
        }
        it++;
    }
}

bool checkBalance(Player * P, int money){
    if (P->money >= money){
        return true;
    }
    else if(!(P->ownedCards.empty())){
        std::cout << "You don't have enough money to pay this balance.\n";
        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n"; 
        
        while((P->money < money) && (P->ownedCards.empty() == false)){
            
            displayProperties(P);

            std::cout << "Which property would you like to mortgage or sell houses on?\n";

            std::string playerResponse;

            std::cin >> playerResponse;

            if (P->ownedCards.find(playerResponse) != P->ownedCards.end()){

                //ARE WE ACCESSING CARDS WITH NAMES OR CARD IDS!!!!!!!!
                if (P->ownedCards[playerResponse]->type == "Property"){

                    PropertyCard* playerCard = dynamic_cast<PropertyCard*>(P->ownedCards[playerResponse]);

                    PropertyTile* playerTile = playerCard->linkTile;

                    if ((playerTile->hotelNum > 0) || (playerTile->hotelNum > 0)){

                        if (playerTile->hotelNum > 0){
                            std::cout << "Would you like to sell your hotel?\n";


                            std::string sellHotel;
                            std::cin >> sellHotel;

                            if (sellHotel == "yes"){
                                playerTile->hotelNum = 0;
                                playerTile->houseNum = 4;
                                P->money = P->money + (playerCard->hotelCost)*0.5;
                                std::cout << "You have sold your hotel, worth " << (playerCard->hotelCost)*0.5 << "\n";
                            }
                        }

                        std::cout << "How many houses would you like to sell?\n";
                        int sellHouse;
                        std::cin >> sellHouse;

                        if ((sellHouse > 0) && (sellHouse < playerTile->houseNum)){
                            playerTile->houseNum = playerTile->houseNum - sellHouse;
                            P->money = P->money + (playerCard->houseCost)*sellHouse*0.5;
                            std::cout << "You have sold " << sellHouse << " houses, worth " << (playerCard->houseCost)*sellHouse*0.5 << "\n";
                        }
                    }

                }
                else{

                 P->money = P->money + (P->ownedCards[playerResponse])->mort;

                 (P->ownedCards[playerResponse])->mortgage();

                  P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                  P->ownedCards.erase("playerResponse");

                  std::cout << playerResponse << " has been mortgaged.\n";
                }

            }
        }
        
        checkBalance(P,money);
    }
    else{
        Bankruptcy(P, activePlayers); //needs to reference a global PlayerTurn object, which will be in the game loop
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

void buyProperty(player * P){
    Tile* purchaseTile = Board[P->location];
    
    cout << "Purchase property " << purchaseTile->name << "?\n"
    
    cin >> std::string playerResponse;
    
    if (playerResponse == "yes"){
    
    if (purchaseTile->type == "Property"){
        PropertyTile* purchaseProperty = dynamic_cast<PropertyTile*>(purchaseTile);
    }
    else if (purchaseTile->type == "RR"){
        RailRoadTile* purchaseProperty = dynamic_cast<RailRoadTile*>(purchaseTile);
    }
    else if (purchaseTile->type == "Utility"){
        UtilityTile* purchaseProperty = dynamic_cast<UtilityTile*>(purchaseTile);
    }
    
     OwnableCard* purchaseCard = purchaseProperty->linkedCard;
        
        if ((P->money) > (purchaseCard->saleValue)){
            purchaseProperty->owner = P;
            P->ownedCards[purchaseCard->name] = purchaseCard;
            cout << "You have successfully purchased " << purchaseCard->name << "\n";
        }
        else{
            cout << "You do not have enough money to purchase this property.\n"
        }
}
}
