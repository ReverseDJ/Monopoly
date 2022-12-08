//
// Created by mdeno on 12/7/2022.
//

#include <string>
#include <math.h>
#include "classes.h"
#include "functions.h"

void PropertyCard::mortgage(){
    linkTile->isMortgaged = true;
}
void PropertyCard::unmortgage(){
    linkTile->isMortgaged = false;
}
void PropertyCard::buy(Player * P){
    linkTile->owner = P;
}

void RailRoadCard::mortgage(){
    linkTile->isMortgaged = true;
}
void RailRoadCard::unmortgage(){
    linkTile->isMortgaged = false;
}
void RailRoadCard::buy(Player * P){
    linkTile->owner = P;
}

void UtilityCard::mortgage(){
    linkTile->isMortgaged = true;
}
void UtilityCard::unmortgage(){
    linkTile->isMortgaged = false;
}
void UtilityCard::buy(Player * P){
    linkTile->owner = P;
}

void getMoneyCard::doDeckCardFunction(Player * P){

    if (moneyAmount < 0 && checkBalance(P,(-1*moneyAmount))){
        P->money = P->money + moneyAmount;
    }
    else{
        P->money = P->money + moneyAmount;
    }
}

void transferMoneyCard::doDeckCardFunction(Player * P){
    int payAmount;

    if (moneyAmount >= 0){
        for (int i = 0; i < playerList.size(); ++i){
            if (checkBalance(playerList[i],moneyAmount) == true){
                (playerList[i])->money = (playerList[i])->money - moneyAmount;
                payAmount = payAmount + moneyAmount;
            }
        }
        P->money = P->money + payAmount;
    }
    else{
        if (checkBalance(P,(-1*(playerList.size())*moneyAmount)) == true)
            for (int i = 0; i < playerList.size(); ++i){
                (playerList[i])->money = (playerList[i])->money - moneyAmount;
            }
    }
}

void getOutOfJailCard::doDeckCardFunction(Player * P){
    P->inJail = false;
    P->DeckCards.erase(name);
}

void PropertyTile::doCardFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)){
        int rent;
        std::string linkedID = linkedCard->cardID;
        int numCardsInMonopoly = int(linkedID[1]);

        if (hotelNum != 0){
            rent = linkedCard->hotelRent;
        }
        else if ((hotelNum == 0) && (houseNum != 0)){
            rent = linkedCard->houseRent[houseNum];
        }
        else{
            rent = linkedCard->baseRent;
            int monopoly = checkMonopoly(P, linkedCard);

            if (monopoly >= numCardsInMonopoly){
                rent = rent*2;
            }
        }
        checkBalance(P, rent);
        P->money = P->money - rent;
        owner->money = owner->money + rent;
    }
    else{
        if (owner == nullptr){
            buyProperty(P);
        }
    }
}

void RailRoadTile::doCardFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)) {
        int rent = linkedCard->baseRent;
        int monopoly = checkMonopoly(P, linkedCard);

        rent = rent * pow(2, (monopoly - 1));

        checkBalance(P, rent);
        P->money = P->money - rent;
        owner->money = owner->money + rent;
    } else {
        if (owner == nullptr){
            buyProperty(P);
        }
    }
}

void TaxTile::doCardFunction(Player * P) {
    if (checkBalance(P, taxValue)){
        P->money = P->money - taxValue;
    }
}

void UtilityTile::doCardFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)) {
        int rent;
        int monopoly = checkMonopoly(P, linkedCard);

        if (monopoly == 2){
            rent = linkedCard->rentTwoUtil;
        }
        else{
            rent = linkedCard->rentOneUtil;
        }
        checkBalance(P,rent);
        P->money = P->money - rent;
        owner->money = owner->money + rent;
    } else {
        if (owner == nullptr){
            buyProperty(P);
        }
    }
}

void GoToJailTile::doCardFunction(Player * P) {
    P->inJail = true;
    P->location = jailLocation;
}

void DrawCardTile::doCardFunction(Player * P){
    DeckCard * frontCard = cardDeck.front();
    cardDeck.pop();

    if (frontCard->type == "GetOutOfJail"){
        P->DeckCards[frontCard->name] = frontCard;
    }
    else{
        cardDeck.push(frontCard);
        frontCard->doDeckCardFunction(P);
    }

}


