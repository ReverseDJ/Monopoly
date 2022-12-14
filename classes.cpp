//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"

extern PlayerTurn activePlayers;
extern Tile * Board[];
extern std::unordered_map<std::string,OwnableCard*> bankCards;
extern std::deque<DeckCard*> chanceCards;
extern std::deque<DeckCard*> communityChestCards;

void PropertyCard::mortgage(){
    linkTile->isMortgaged = true;
}
void PropertyCard::unmortgage(){

    if(checkBalance(linkTile->owner, mort, true, nullptr)){
        linkTile->owner->money -= mort;
        linkTile->isMortgaged = false;
        linkTile->owner->ownedCards.insert(std::make_pair(cardID, linkTile->linkedCard));
        linkTile->owner->mortCards.erase(cardID);
    }
    else{
        std::cout<<"Unmortgage Canceled"<<std::endl;
    }

}
void PropertyCard::buy(Player * P){
    linkTile->owner = P;
}

void RailRoadCard::mortgage(){
    linkTile->isMortgaged = true;
}
void RailRoadCard::unmortgage(){

    if(checkBalance(linkTile->owner, mort, true, nullptr)){
        linkTile->owner->money -= mort;
        linkTile->isMortgaged = false;
        linkTile->owner->ownedCards[cardID] = linkTile->owner->mortCards[cardID];
        linkTile->owner->mortCards.erase(cardID);
    }
    else{
        std::cout<<"Unmortgage Canceled"<<std::endl;
    }
}
void RailRoadCard::buy(Player * P){
    linkTile->owner = P;
}

void UtilityCard::mortgage(){
    linkTile->isMortgaged = true;

}
void UtilityCard::unmortgage(){
    if(checkBalance(linkTile->owner, mort, true, nullptr)){
        linkTile->owner->money -= mort;
        linkTile->isMortgaged = false;
        linkTile->owner->ownedCards[cardID] = linkTile->owner->mortCards[cardID];
        linkTile->owner->mortCards.erase(cardID);
    }
    else{
        std::cout<<"Unmortgage Canceled"<<std::endl;
    }
}
void UtilityCard::buy(Player * P){
    linkTile->owner = P;
}

void getOutOfJailCard::doDeckCardFunction(Player *P){ //runs if player uses get out of jail card
    P->inJail = false;
    P->turnsInJail = 0;
    if (cardID == "CH") {
        chanceCards.push_back(P->DeckCards[cardID]);
    }
    else if (cardID == "CC") {
        communityChestCards.push_back(P->DeckCards[cardID]);
    }
    P->DeckCards.erase(cardID);
}

void PropertyTile::doTileFunction(Player * P) {
    if(P==owner){
        std::cout<<"You own this property"<<std::endl;
    }
    else if (owner != nullptr && (!isMortgaged)){ //if tile is owned and not mortgaged
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
            if (numOfCards == numCardsInMonopoly){ //compares to number of properties in set to determine if player has a monopoly.
                rent = rent*2; //doubles rent if player has monopoly.
            }
        }

        // Check Balance different for bot and human
        if (BOT(P)) {
            if (checkBotBalance(P, rent, owner)) {
                P->money = P->money - rent;
                owner->money = owner->money + rent;
                std::cout<<"BOT paid $"<<rent<<" to "<<owner->name<<", BOT has $"<<P->money<<" left"<<std::endl;
            }
        }
        else if(checkBalance(P, rent,false,owner)){
            P->money = P->money - rent;
            owner->money = owner->money + rent;
            std::cout<<"You paid $"<<rent<<" to "<<owner->name<<", you have $"<<P->money<<" left"<<std::endl;
        }
        else{

        }//makes sure player can pay rent.

    }
    else{
        if (owner == nullptr){ //if property is unowned, offer property to player
            BH_BUYPROPERTY(P);
        }
    }
}

void RailRoadTile::doTileFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)) { // if owned and not mortgaged
        int rent = linkedCard->baseRent;
        int monopoly = checkMonopoly(owner, linkedCard); //determine how many rail roads owner has
        rent *= int(pow(2, (monopoly-1))); //rent is doubled for every additional rail road owner has.

        BH_CHECKBALANCE(P, rent, false, owner);
        //checkBalance(P, rent,false, owner); //makes sure player can pay rent.

        P->money = P->money - rent;
        owner->money = owner->money + rent;
        std::cout<<"You paid $"<<rent<<" to "<<owner->name<<", you have $"<<P->money<<" left"<<std::endl;
    } else {
        if (owner == nullptr){ //if property is unowned, offer to player.
            BH_BUYPROPERTY(P);
            //buyProperty(P);
        }
    }
}

void TaxTile::doTileFunction(Player * P) { //if player can pay tax, subtract from balance
    if (BOT(P)) {
        if (checkBotBalance(P, taxValue, nullptr)) {
            P->money = P->money - taxValue;
        }
    }
    else if (checkBalance(P, taxValue,false, nullptr)){
        P->money = P->money - taxValue;
    }
    std::cout << "Player " << P->name << " has $" << P->money << std::endl;
}

void UtilityTile::doTileFunction(Player * P) {
    if (owner != nullptr && (!isMortgaged)) { // if owned and not mortgaged
        int rent;
        int roll;
        int monopoly = checkMonopoly(owner, linkedCard); //check if player owns both utilities

        roll = diceRoll()+diceRoll();

        if (monopoly == 2){
            rent = roll*(linkedCard->rentTwoUtil); //if players owns both utilities, set appropriate rent

        }
        else{
            rent = roll*(linkedCard->rentOneUtil);
        }
        BH_CHECKBALANCE(P, rent, false, owner);
        //checkBalance(P,rent,false, owner);

        P->money = P->money - rent; //check that player can pay rent and charge
        owner->money = owner->money + rent;
        std::cout<<"You rolled a "<<roll<<" you paid $"<<rent<<" to "<<owner->name<<std::endl;
    } else {
        if (owner == nullptr){
            //buyProperty(P);
            BH_BUYPROPERTY(P);
        }
    }
}

void GoToJailTile::doTileFunction(Player * P) {
    //static int jailLocation = 10; //jail is the 11th space on the board, or Board[10]
    P->inJail = true;
    P->location = 10;
    std::cout<<"Player "<<P->name<<" has gone to Jail"<<std::endl;
    
    if (!BOT(P))checkGOOJ(P);
    /* if (P->DeckCards.size()) {
        std::cout << "Would you like to use a Get Out Of Jail Free card? (y/n)" << std::endl;
        char useCard;
        std::cin >> useCard;
        if (useCard == 'y') {
            std::cout << "Used Get Out Of Jail Free card." << std::endl;
            std::cout << "You are at tile #10." << std::endl;
            
            // find a card
            (P->DeckCards.begin())->second->doDeckCardFunction(P);

        }
        else {
            std::cout << "You are still in Jail." << std::endl;
        }
    } */

}

void CornerTile::doTileFunction(Player * P) {
    //std::cout << P->name << "is on tile: " << this->name << std::endl;
}

void DrawCardTile::doTileFunction(Player * P){
    DeckCard * frontCard = cardDeck->front(); //gets card from top of deck
    cardDeck->pop_front(); //removes card from deck

    if (frontCard->type == "GetOutOfJail"){ //if card is get out of jail card
        std::cout << frontCard->cardDesc << std::endl;
        P->DeckCards[frontCard->cardID] = frontCard; //gives get out of jail card to player
        std::cout << "Player " << P->name << " has " << P->DeckCards.size() << " Get Out Of Jail Free cards." << std::endl;
    }
    else{
        cardDeck->push_back(frontCard);
        frontCard->doDeckCardFunction(P); //runs appropriate card function
    }
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

    if (BOT(P)) {
        if (moneyAmount < 0 && checkBotBalance(P, (-1*moneyAmount), nullptr)) {
            P->money = P->money + moneyAmount;
        }
    }
    else if (moneyAmount < 0 && checkBalance(P,(-1*moneyAmount),false, nullptr)){ //if player needs to pay money, check their balance (returns true if they can pay) and subtract the money.
        P->money = P->money + moneyAmount; //moneyAmount will be negative if this branch is active.
    }
    else {
        P->money = P->money + moneyAmount;
    }
}

void transferMoneyCard::doDeckCardFunction(Player * P){
    std::cout << cardDesc << std::endl;
    //backs up current player
    Player * curPlayerBackup = activePlayers.currentPlayer;
    //updates to next player since we don't want to subtract money from the player who is paying
    activePlayers.next_player();
    //sets next player to temp;
    Player * tempPlayer = activePlayers.currentPlayer;

    while(tempPlayer != curPlayerBackup){

        // + pay amount = card drawer receives money, else card drawer pays to all other players
        // if card drawer is paying, check balance at each loop iteration

        if (moneyAmount > 0) {
            if (BOT(P)) {
                if (checkBotBalance(tempPlayer, moneyAmount, tempPlayer)) {
                    tempPlayer->money -= moneyAmount;
                    curPlayerBackup->money +=moneyAmount;
                }
            }
            else if (checkBalance(tempPlayer, moneyAmount,false, tempPlayer)) {
                tempPlayer->money -= moneyAmount;
                curPlayerBackup->money += moneyAmount;
            }
        }
        else {
            if (BOT(P)) {
                if (checkBotBalance(curPlayerBackup, moneyAmount, tempPlayer)) {
                    curPlayerBackup->money += moneyAmount; // payAmount < 0
                    tempPlayer->money -= moneyAmount;
                }
            }
            else if (checkBalance(curPlayerBackup, moneyAmount,false, tempPlayer)) {
                curPlayerBackup->money += moneyAmount; // payAmount < 0
                tempPlayer->money -= moneyAmount;
            }
        }

        activePlayers.next_player();
        tempPlayer = activePlayers.currentPlayer;
    }
}

void payPerBuildingCard::doDeckCardFunction(Player *P) {

    std::cout << cardDesc << std::endl;

    int totalToPay = 0;
    int numHouses = 0;
    int numHotels = 0;
    PropertyCard * tempCard;

    // traverse map OwnedCards
    for (auto it = P->ownedCards.begin(); it != P->ownedCards.end(); it++) {
        if (it->second->type == "Property") {
            tempCard = dynamic_cast<PropertyCard*>(it->second);
            numHouses += tempCard->linkTile->houseNum;
            numHotels += tempCard->linkTile->hotelNum;
            totalToPay = (numHouses * amtPerHouse) + (numHotels * amtPerHotel);
        }
    }

    std::cout << "Paying $" << totalToPay << "for " << numHouses << " houses and " << numHotels << " hotels." << std::endl;

    BH_CHECKBALANCE(P, totalToPay,false, nullptr);
    P->money -= totalToPay;

}

