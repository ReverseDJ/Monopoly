//
// Created by mdeno on 12/14/2022.
//
#include "functions.h"

extern PlayerTurn activePlayers;
extern Tile * Board[];
extern std::unordered_map<std::string,OwnableCard*> bankCards;
extern bool curPisBankrupt;
extern int botMoneyStart;

bool checkBotBalance(Player * P, int money, Player * debtor = nullptr){
    if (P->money >= money){ //if player has enough money to pay rent
        return true;
    }
    else if(!(P->ownedCards.empty())) { //if player does not have enough money but can mortgage properties or sell houses
        std::cout << "BOT PLAYER doesn't have enough money to pay this balance.\n";
        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n";

        while ((P->money < money) && (P->ownedCards.empty() == false)) { //while player can sell houses/mortgage properties

            displayProperties(P);

            std::vector<OwnableCard *> cardsCopy = {};

            for (auto i: P->ownedCards) {        // mortgage non-monopoly properties
                cardsCopy.push_back(i.second);
            }

            int index = rand() % cardsCopy.size();

            std::string playerResponse = cardsCopy[index]->cardID;

            if (cardsCopy[index]->type == "Property") {
                if (checkMonopoly(P, cardsCopy[index]) != cardsCopy[index]->cardID[2] - 48) {
                    P->money = P->money + (P->ownedCards[playerResponse])->mort;

                    (P->ownedCards[playerResponse])->mortgage();

                    P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                    P->ownedCards.erase(playerResponse);

                    std::cout << playerResponse << " has been mortgaged.\n";
                } else {
                    PropertyCard *temp = dynamic_cast<PropertyCard *>(cardsCopy[index]);
                    if (temp->linkTile->hotelNum == 1) {
                        P->money += (temp->hotelCost) / 2;
                    } else if (temp->linkTile->houseNum > 0) {
                        P->money += (temp->houseCost) / 2;
                    }
                    else{
                        bool monopolyHasHouses = false;
                        for(int i = 0 ; i < playerResponse[2]-48; i++){
                            std::string curID = playerResponse;
                            curID[3] = char(i+48);
                            PropertyCard * curCard = dynamic_cast<PropertyCard*>(P->ownedCards[curID]);
                            if(curCard->linkTile->houseNum != 0 || curCard->linkTile->hotelNum != 0){
                                monopolyHasHouses = true;
                            }
                        }
                        if(!monopolyHasHouses){
                            P->money = P->money + (P->ownedCards[playerResponse])->mort;

                            (P->ownedCards[playerResponse])->mortgage();

                            P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                            P->ownedCards.erase(playerResponse);

                            std::cout << playerResponse << " has been mortgaged.\n";
                        }
                    }
                }
            }
            else if (cardsCopy[index]->type == "RailRoad") {
                P->money = P->money + (P->ownedCards[playerResponse])->mort;

                (P->ownedCards[playerResponse])->mortgage();

                P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                P->ownedCards.erase(playerResponse);

                std::cout << playerResponse << " has been mortgaged.\n";
            }
            else if (cardsCopy[index]->type == "Utility") {
                P->money = P->money + (P->ownedCards[playerResponse])->mort;

                (P->ownedCards[playerResponse])->mortgage();

                P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                P->ownedCards.erase(playerResponse);

                std::cout << playerResponse << " has been mortgaged.\n";
            }
            std::cout << "current balance: " << P->money << "\n";
            std::cout << "amount due: " << money << "\n";

            if (P->money >= money){ //checks if player now has enough money to pay rent. If not, continues loop
                return true;
            }

            std::cout << "You don't have enough money to pay this balance.\n";
            Bankruptcy(P, activePlayers, debtor); //if value of houses/properties cannot cover rent, player goes bankrupt.
            return false;
        }

    }
    else{                   // can't mortgage or sell anything
        Bankruptcy(P, activePlayers, debtor); //needs to reference a global PlayerTurn object, which will be in the game loop
        curPisBankrupt = true;
        return false;
    }
}


bool trackBotSpending(Player *P) {
    int botSpent = botMoneyStart - P->money;
    if ((botSpent > 0.30*botMoneyStart) || (botSpent > 600)) return false;
    else return true;
}

void botBuyProperty(Player * P) {
    Tile* purchaseTile = Board[P->location]; //gets the tils to purchase from the player's location.

    if (trackBotSpending(P)) {
        // get type, check if enough money
        if (purchaseTile->type == "Property"){
            PropertyTile* purchaseProperty = dynamic_cast<PropertyTile*>(purchaseTile); //casts to PropertyTile to use the linkedCard member

            OwnableCard* purchaseCard = purchaseProperty->linkedCard; //gets the linked card to insert to ownedProperties

            if (P->money > purchaseCard->saleValue){ //if player has enough money to buy property
                purchaseProperty->owner = P;
                P->ownedCards[purchaseCard->cardID] = purchaseCard; //adds linked card to ownedCards
                std::cout << "Bot has successfully purchased " << purchaseCard->name << "\n";
            }
            else{
                std::cout << "Bot does not have enough money to purchase this property.\n";
            }
        }
        else if (purchaseTile->type == "RR"){
            RailRoadTile* purchaseProperty = dynamic_cast<RailRoadTile*>(purchaseTile); //same code as for properties. It needs to be separate because linkedCard points to a different type

            OwnableCard* purchaseCard = purchaseProperty->linkedCard;

            if (P->money > purchaseCard->saleValue){
                purchaseProperty->owner = P;
                P->ownedCards[purchaseCard->cardID] = purchaseCard;
                std::cout << "Bot has successfully purchased " << purchaseCard->name << "\n";
            }
            else{
                std::cout << "Bot does not have enough money to purchase this property.\n";
            }
        }
        else if (purchaseTile->type == "Utility"){
            UtilityTile* purchaseProperty = dynamic_cast<UtilityTile*>(purchaseTile); //same code as for properties. It needs to be separate because linkedCard points to a different type

            OwnableCard* purchaseCard = purchaseProperty->linkedCard;

            if (P->money > purchaseCard->saleValue){
                purchaseProperty->owner = P;
                P->ownedCards[purchaseCard->cardID] = purchaseCard;
                std::cout << "Bot has successfully purchased " << purchaseCard->name << "\n";
            }
            else{
                std::cout << "Bot does not have enough money to purchase this property.\n";
            }
        }

    }

}



void botBuyHouses(Player * P) { //called at the end of player turn
    
    std::unordered_map<std::string, PropertyCard*> houseCards; //a new map of cards that the player can buy houses on
    
    std::cout << "Bot can buy houses/hotels on the following properties:\n";

    std::unordered_map<std::string, OwnableCard *>::iterator it = P->ownedCards.begin(); //iterates over all cards player owns

    std::cout << std::setw(25) << std::left << "Card Name";
    std::cout << std::setw(10) << std::left << "Card ID";
    std::cout << std::setw(10) << std::left << "Houses";
    std::cout << std::setw(10) << std::left << "Hotels" << std::endl;
    
    
    while (it != P->ownedCards.end()) { //iterates over all owned cards

        std::string propertyID = it->first; //property ID
        std::string propertyName = it->second->name;
        int mortgageValue = it->second->mort; //mortgage value
        

        if (it->second->type == "Property") { //only cares about property cards
            int numHouses = (dynamic_cast<PropertyCard *>(it->second))->linkTile->houseNum; //gets number of houses
            int numHotels = (dynamic_cast<PropertyCard *>(it->second))->linkTile->hotelNum; //gets number of hotels
            
            if ((numHotels == 0) && (checkMonopoly(P,it->second) == (int((it->second)->cardID[2]-48)))){
                std::cout << std::setw(25) << std::left << propertyName;
                std::cout << std::setw(10) << std::left << propertyID;
                std::cout << std::setw(10) << std::left << numHouses;
                std::cout << std::setw(10) << std::left << numHotels << std::endl;
                
                houseCards[it->first] = dynamic_cast<PropertyCard *>(it->second);
            }
        } 
        it++;
    }

    if(houseCards.empty()){
        std::cout<<"There are no properties that Bot can buy houses on"<<std::endl;
        return;
    }

        auto ite = houseCards.begin();

        while (trackBotSpending(P) && ite != houseCards.end()) {
            PropertyCard *playerCard = ite->second;
            PropertyTile *playerTile = playerCard->linkTile;

            if (playerTile->houseNum == 4) {
                if (P->money > playerCard->hotelCost) {
                    playerTile->hotelNum = 1;
                    playerTile->houseNum = 0;
                    P->money = P->money - (playerCard->hotelCost);
                    std::cout << "Bot bought a hotel on " << playerCard->name << std::endl;
                }
            }
            else {
                if (P->money > playerCard->hotelCost) {
                    playerTile->houseNum = playerTile->houseNum + 1;
                    //deducts the house purchase value from player balance
                    P->money = P->money - playerCard->houseCost;
                    std::cout << "Bot bought a house on " << playerCard->name << std::endl;
                }
            }
            ite++;
        }

}