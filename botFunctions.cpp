//
// Created by mdeno on 12/14/2022.
//
#include "functions.h"

extern PlayerTurn activePlayers;
extern Tile * Board[];
extern std::unordered_map<std::string,OwnableCard*> bankCards;
extern bool curPisBankrupt;

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


bool trackBotSpending(Player *P, int botSpent, int botMoneyStart) {
    if ((botSpent > 0.30*botMoneyStart) || (botSpent > 600)) return true;
    else return false;
}

void botBuyProperty(Player * P) {

}