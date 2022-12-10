// functions.cpp -- function implementations

#include "functions.h"
#include <iostream>
#include <list>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>

extern PlayerTurn * activePlayers;
extern Tile * Board[];
extern std::unordered_map<std::string,OwnableCard*> bankCards;


void displayProperties(Player * P){
    std::cout << "You own the following properties:\n";
        
        std::unordered_map<std::string, OwnableCard*>::iterator it = P->ownedCards.begin(); //iterates over all cards player owns
     
        while (it != P->ownedCards.end()){
            
            std::string propertyID = it->first; //property ID
            std::string propertyName = it->second->name;
            int mortgageValue = it->second->mort; //mortgage value

            if (it->second->type == "Property"){
                int numHouses = (dynamic_cast<PropertyCard*>(it->second))->linkTile->houseNum; //gets number of houses
                int numHotels = (dynamic_cast<PropertyCard*>(it->second))->linkTile->hotelNum; //gets number of hotels
                std::cout << propertyName << " : " << mortgageValue << " houses: " << numHouses << " hotels: " << numHotels << "\n";
            }else{
            std::cout << propertyName << " ID: " << propertyID << " Mortgage Value: " << mortgageValue << "\n";
        }
        it++;
    }
}

bool checkBalance(Player * P, int money){
    if (P->money >= money){ //if player has enough money to pay rent
        return true;
    }
    else if(!(P->ownedCards.empty())){ //if player does not have enough money but can mortgage properties or sell houses
        std::cout << "You don't have enough money to pay this balance.\n";
        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n"; 
        
        while((P->money < money) && (P->ownedCards.empty() == false)){ //while player can sell houses/mortgage properties
            
            displayProperties(P);

            std::cout << "Which property would you like to mortgage or sell houses on?\n";
            std::cout << "Enter the ID: ";

            std::string playerResponse;

            std::cin >> playerResponse;

            if (P->ownedCards.find(playerResponse) != P->ownedCards.end()){ //if the player's choice is in their ownedProperties deck

                if (P->ownedCards[playerResponse]->type == "Property"){ //if property type, check if there are houses/hotels to sell

                    PropertyCard* playerCard = dynamic_cast<PropertyCard*>(P->ownedCards[playerResponse]); //casts to type PropertyCard to use linkTile member

                    PropertyTile* playerTile = playerCard->linkTile;

                    if ((playerTile->houseNum > 0) || (playerTile->hotelNum > 0)){ //if player has houses or hotels to sell

                        if (playerTile->hotelNum > 0){
                            std::cout << "Would you like to sell your hotel?\n";


                            std::string sellHotel;
                            std::cin >> sellHotel;

                            if (sellHotel == "yes"){
                                playerTile->hotelNum = 0; //removes hotel and adds four houses
                                playerTile->houseNum = 4;
                                P->money = P->money + (playerCard->hotelCost)*0.5;
                                std::cout << "You have sold your hotel, worth " << (playerCard->hotelCost)*0.5 << "\n";
                            }
                        }

                        std::cout << "How many houses would you like to sell?\n";
                        int sellHouse;
                        std::cin >> sellHouse;

                        if ((sellHouse > 0) && (sellHouse < playerTile->houseNum)){ //if player wants to sell houses
                            playerTile->houseNum = playerTile->houseNum - sellHouse;
                            P->money = P->money + (playerCard->houseCost)*sellHouse*0.5; //gives the player back half the house purchase value
                            std::cout << "You have sold " << sellHouse << " houses, worth " << (playerCard->houseCost)*sellHouse*0.5 << "\n";
                        }
                    }

                }
                else{ //if not a property type (cannot have houses and hotels), mortgages property.

                 P->money = P->money + (P->ownedCards[playerResponse])->mort;

                 (P->ownedCards[playerResponse])->mortgage();

                  P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                  P->ownedCards.erase("playerResponse");

                  std::cout << playerResponse << " has been mortgaged.\n";
                }

            }
            
         if (P->money >= money){ //checks if player now has enough money to pay rent. If not, continues loop
            return true;
          }
            
        }
        Bankruptcy(P, activePlayers); //if value of houses/properties cannot cover rent, player goes bankrupt.
         return false; 
    }
    else{
        Bankruptcy(P, activePlayers); //needs to reference a global PlayerTurn object, which will be in the game loop
        return false;
    }
  }

  void Bankruptcy(Player* P, PlayerTurn* Turn){
    Turn->RemovePlayer(P);  //removes player from playerTurn object
    std::cout << P->name << " has gone bankrupt!\n";
}

int checkMonopoly(Player * P, Card * C){
    
    std::string suitCard = C->cardID; //gets ID of current card
    int numCards = int(C->cardID[2]); //the third letter of the cardID is the total number of cards in the suit
    int cardNum = int(C->cardID[3]); //the fourth letter is the number of the card in the suit (0, 1, 2, 3, etc.)
    int monopolyCount;
    
    for (int i = 0; i < numCards; i++){ //Searches ownedProperties for other cards in the suit. We can find their cardIDs systematically.
        if (i != cardNum){
            suitCard[3] = char(cardNum);
            if(P->ownedCards.find(suitCard) != P->ownedCards.end()){
                monopolyCount = monopolyCount + 1;
            }
        }
    }
    return monopolyCount; //returns the number of other cards in the suit the player owns
}

void buyProperty(Player * P){
    Tile* purchaseTile = Board[P->location]; //gets the tils to purchase from the player's location.
    
    std::cout << "Purchase property " << purchaseTile->name << "?\n";
    
    std::string playerResponse;
    
    std::cin >> playerResponse;
    
    if (playerResponse == "yes"){
    
        if (purchaseTile->type == "Property"){
            PropertyTile* purchaseProperty = dynamic_cast<PropertyTile*>(purchaseTile); //casts to PropertyTile to use the linkedCard member

            OwnableCard* purchaseCard = purchaseProperty->linkedCard; //gets the linked card to insert to ownedProperties

            if ((P->money) > (purchaseCard->saleValue)){ //if player has enough money to buy property
                purchaseProperty->owner = P;
                P->ownedCards[purchaseCard->cardID] = purchaseCard; //adds linked card to ownedCards
                std::cout << "You have successfully purchased " << purchaseCard->name << "\n";
            }
            else{
                std::cout << "You do not have enough money to purchase this property.\n";
            }
        }
        else if (purchaseTile->type == "RR"){
            RailRoadTile* purchaseProperty = dynamic_cast<RailRoadTile*>(purchaseTile); //same code as for properties. It needs to be separate because linkedCard points to a different type

            OwnableCard* purchaseCard = purchaseProperty->linkedCard;

            if ((P->money) > (purchaseCard->saleValue)){
                purchaseProperty->owner = P;
                P->ownedCards[purchaseCard->cardID] = purchaseCard;
                std::cout << "You have successfully purchased " << purchaseCard->name << "\n";
            }
            else{
                std::cout << "You do not have enough money to purchase this property.\n";
            }
        }
        else if (purchaseTile->type == "Utility"){
            UtilityTile* purchaseProperty = dynamic_cast<UtilityTile*>(purchaseTile); //same code as for properties. It needs to be separate because linkedCard points to a different type

            OwnableCard* purchaseCard = purchaseProperty->linkedCard;

            if ((P->money) > (purchaseCard->saleValue)){
                purchaseProperty->owner = P;
                P->ownedCards[purchaseCard->cardID] = purchaseCard;
                std::cout << "You have successfully purchased " << purchaseCard->name << "\n";
            }
            else{
                std::cout << "You do not have enough money to purchase this property.\n";
            }
        }

    }
}

void movePlayer(Player *P, int dest, bool inst){ 
    int originalLocation = P->location;
    dest = dest%40;
    P->location = dest; //updates player location to destination
    
    if (!inst && (dest < originalLocation)){ //if needed, checks to see if player passed GO and adds $200
        P->money = P->money + 200;
        std::cout << P->name << " passed Go and collected $200.\n";
    }
    
}

int diceRoll(){
    srand(time(0));
    int roll=(rand()%6) +1;
    return roll;
}
