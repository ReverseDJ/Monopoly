//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"

extern PlayerTurn * activePlayers;
extern Tile * Board[];
extern std::unordered_map<std::string,OwnableCard*> bankCards;
extern std::deque<DeckCard*> chanceCards;
extern std::deque<DeckCard*> communityChestCards;

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

void getOutOfJailCard::doDeckCardFunction(Player *P){ //runs if player uses get out of jail card
    P->inJail = false;
    if (cardID == "CH") {
        chanceCards.push_back(P->DeckCards[cardID]);
    }
    else if (cardID == "CH") {
        communityChestCards.push_back(P->DeckCards[cardID]);
    }
    P->DeckCards.erase(cardID);
}

void PropertyTile::doTileFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)){ //if tile is owned and not mortgaged
        int rent;
        std::string linkedID = linkedCard->cardID;
        int numCardsInMonopoly = int(linkedID[2]-48); //uses the cardID field of the linked card to determine how many properties are in the set.

        if (hotelNum != 0){
            rent = linkedCard->hotelRent; //sets rent to hotel rent if property has hotel
        }
        else if ((hotelNum == 0) && (houseNum != 0)){
            rent = linkedCard->houseRent[houseNum]; //if property has houses, sets rent to appropriate value based on number of houses. Rent values for houses are stored in array.
        }
        else{
            rent = linkedCard->baseRent;
            int numOfCards = checkMonopoly(owner, linkedCard); //returns the number of properties the owner has in the set.
            std::cout<<owner<<" owns this proprty, they own "<<numOfCards<<" of "<<numCardsInMonopoly<<" cards.\n";
            if (numOfCards == numCardsInMonopoly){ //compares to number of properties in set to determine if player has a monopoly.
                rent = rent*2; //doubles rent if player has monopoly.
            }
        }
        checkBalance(P, rent,false); //makes sure player can pay rent.
        P->money = P->money - rent;
        owner->money = owner->money + rent;
        std::cout<<"You paid $"<<rent<<" to "<<owner->name<<", you have $"<<P->money<<" left"<<std::endl;
    }
    else{
        if (owner == nullptr){ //if property is unowned, offer property to player
            buyProperty(P);
        }
    }
}

void RailRoadTile::doTileFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)) { // if owned and not mortgaged
        int rent = linkedCard->baseRent;
        int monopoly = checkMonopoly(owner, linkedCard); //determine how many rail roads owner has
        rent *= int(pow(2, (monopoly-1))); //rent is doubled for every additional rail road owner has.

        checkBalance(P, rent,false); //makes sure player can pay rent.
        P->money = P->money - rent;
        owner->money = owner->money + rent;
        std::cout<<"You paid $"<<rent<<" to "<<owner->name<<", you have $"<<P->money<<" left"<<std::endl;
    } else {
        if (owner == nullptr){ //if property is unowned, offer to player.
            buyProperty(P);
        }
    }
}

void TaxTile::doTileFunction(Player * P) { //if player can pay tax, subtract from balance
    if (checkBalance(P, taxValue,false)){
        P->money = P->money - taxValue;
    }
}

void UtilityTile::doTileFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)) { // if owned and not mortgaged
        int rent;
        int monopoly = checkMonopoly(P, linkedCard); //check if player owns both utilities

        if (monopoly == 2){
            rent = linkedCard->rentTwoUtil; //if players owns both utilities, set appropriate rent
        }
        else{
            rent = linkedCard->rentOneUtil;
        }
        checkBalance(P,rent,false);
        P->money = P->money - rent; //check that player can pay rent and charge
        owner->money = owner->money + rent;
    } else {
        if (owner == nullptr){
            buyProperty(P);
        }
    }
}

void GoToJailTile::doTileFunction(Player * P) {
    int jailLocation = 10; //jail is the 11th space on the board, or Board[10]
    P->inJail = jailLocation;
    P->location = 10;
    std::cout<<"Player "<<P->name<<" has gone to Jail"<<std::endl;
}

void CornerTile::doTileFunction(Player * P) {
    //std::cout << P->name << "is on tile: " << this->name << std::endl;
}

void DrawCardTile::doTileFunction(Player * P){
    DeckCard * frontCard = cardDeck->front(); //gets card from top of deck
    cardDeck->pop_front(); //removes card from deck
    std::cout<<frontCard->cardDesc<<std::endl;
    /*if (frontCard->type == "GetOutOfJail"){ //if card is get out of jail card
        P->DeckCards[frontCard->cardID] = frontCard; //gives get out of jail card to player
    }
    else{
        cardDeck->push_back(frontCard);
        frontCard->doDeckCardFunction(P); //runs appropriate card function
    }*/
}


#define BACK_3  (-3)
#define NEXT_RR (-5)
#define NEXT_U  (-10)

void movePlayerCard::doDeckCardFunction(Player * P){
    std::cout<<cardDesc<<std::endl;
    if (dest < 0) {
        switch (dest) {
            case BACK_3: {
                movePlayer(P, P->location + dest, true);
                break;
            }
            case NEXT_RR: {
                movePlayer(P, ((5 + ((P->location + 5) / 10) * 10) % 40), false);
                break;
            }
            case NEXT_U: {
                int i = ((P->location < 28) && (P->location > 12)) ? 28 : 12;
                movePlayer(P, i, false);
                break;
            }
            default: {
                std::cout << "Bad card destination" << std::endl;
                break;
            }
        }
    }

    else {
        movePlayer(P, dest, inst);
    }

    Board[P->location]->doTileFunction(P);
}

void getMoneyCard::doDeckCardFunction(Player * P){
    std::cout<<cardDesc<<std::endl;
    if (moneyAmount < 0 && checkBalance(P,(-1*moneyAmount),false)){ //if player needs to pay money, check their balance (returns true if they can pay) and subtract the money.
        P->money = P->money + moneyAmount; //moneyAmount will be negative if this branch is active.
    }
    else {
        P->money = P->money + moneyAmount;
    }
}

void transferMoneyCard::doDeckCardFunction(Player * P){
    int payAmount;
    //backs up current player
    Player * curPlayerBackup = activePlayers->currentPlayer;
    //updates to next player since we don't want to subtract money from the player who is paying
    activePlayers->next_player();
    //sets next player to temp;
    Player * tempPlayer = activePlayers->currentPlayer;

    while(tempPlayer != curPlayerBackup){

        // + pay amount = card drawer receives money, else card drawer pays to all other players
        // if card drawer is paying, check balance at each loop iteration

        if (payAmount > 0) {
            if (checkBalance(tempPlayer, payAmount,false)) {
                tempPlayer->money -= payAmount;
                curPlayerBackup->money += payAmount;
            }
        }
        else {
            if (checkBalance(curPlayerBackup, payAmount,false)) {
                curPlayerBackup->money += payAmount; // payAmount < 0
                tempPlayer->money -= payAmount;
            }
        }

        activePlayers->next_player();
        tempPlayer = activePlayers->currentPlayer;
    }

    /*
    if (moneyAmount >= 0){ //called if player recieves money from other players
        for (int i = 0; i < playerList.size(); ++i){  //iterates over a list of all players and checks their balance. If they can pay, subtracts the amount.
            if (checkBalance(playerList[i],moneyAmount,false) == true){
                (playerList[i])->money = (playerList[i])->money - moneyAmount;
                payAmount = payAmount + moneyAmount; //sum of amount payed by other players.
            }
        }
        P->money = P->money + payAmount;
    }
    else{ //called if player pays money to others.
        if (checkBalance(P,(-1*(playerList.size())*moneyAmount),false) == true) //checks if player can pay balance to all other players
            for (int i = 0; i < playerList.size(); ++i){
                (playerList[i])->money = (playerList[i])->money - moneyAmount; //loops over list of players and pays correct amount to all.
            }
    }
     */
}

void payPerBuildingCard::doDeckCardFunction(Player *P) {
    int totalToPay;
    PropertyCard * tempCard;

    // traverse map OwnedCards
    for (auto it = P->ownedCards.begin(); it != P->ownedCards.end(); it++) {
        if (it->second->type == "Property") {
            tempCard = dynamic_cast<PropertyCard*>(it->second);
            totalToPay += tempCard->linkTile->houseNum * this->amtPerHouse;
            totalToPay += tempCard->linkTile->hotelNum * this->amtPerHotel;
        }
    }

    if (checkBalance(P, totalToPay,false)) {
        P->money -= totalToPay;
    }
}

