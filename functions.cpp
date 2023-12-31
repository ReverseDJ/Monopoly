// functions.cpp -- function implementations

#include "functions.h"

extern PlayerTurn activePlayers;
extern Tile * Board[];
extern std::unordered_map<std::string,OwnableCard*> bankCards;
extern bool curPisBankrupt;


void displayProperties(Player * P) {
    std::cout << "You own the following properties:\n";

    std::unordered_map<std::string, OwnableCard *>::iterator it = P->ownedCards.begin(); //iterates over all cards player owns

    std::cout << std::setw(25) << std::left << "Card Name";
    std::cout << std::setw(10) << std::left << "Card ID";
    std::cout << std::setw(15) << std::left << "Mortgage Value";
    std::cout << std::setw(10) << std::left << "Houses";
    std::cout << std::setw(10) << std::left << "Hotels" << std::endl;
    while (it != P->ownedCards.end()) {

        std::string propertyID = it->first; //property ID
        std::string propertyName = it->second->name;
        int mortgageValue = it->second->mort; //mortgage value

        if (it->second->type == "Property") {
            int numHouses = (dynamic_cast<PropertyCard *>(it->second))->linkTile->houseNum; //gets number of houses
            int numHotels = (dynamic_cast<PropertyCard *>(it->second))->linkTile->hotelNum; //gets number of hotels
            std::cout << std::setw(25) << std::left << propertyName;
            std::cout << std::setw(10) << std::left << propertyID;
            std::cout << std::setw(15) << std::left << mortgageValue;
            std::cout << std::setw(10) << std::left << numHouses;
            std::cout << std::setw(10) << std::left << numHotels << std::endl;
        } else {
            std::cout << std::setw(25) << std::left << propertyName;
            std::cout << std::setw(10) << std::left << propertyID;
            std::cout << std::setw(15) << std::left << mortgageValue << std::endl;

        }
        it++;
    }
}

void displayMortgaged(Player * P) {
    std::cout << "You have the following mortgaged properties:" << std::endl;

    auto it = P->mortCards.begin();
    std::cout << std::setw(25) << std::left << "Card Name";
    std::cout << std::setw(10) << std::left << "Card ID";
    std::cout << std::setw(15) << std::left << "Mortgage Value" << std::endl;

    while (it != P->ownedCards.end()) {

        std::string propertyID = it->first; //property ID
        std::string propertyName = it->second->name;
        int mortgageValue = it->second->mort; //mortgage value

        std::cout << std::setw(25) << std::left << propertyName;
        std::cout << std::setw(10) << std::left << propertyID;
        std::cout << std::setw(15) << std::left << mortgageValue << std::endl;

        it++;
    }
}




bool checkBalance(Player * P, int money, bool optional, Player * debtor = nullptr){
    if (P->money >= money){ //if player has enough money to pay rent
        return true;
    }
    else if(!(P->ownedCards.empty())) { //if player does not have enough money but can mortgage properties or sell houses
        std::cout << "You don't have enough money to pay this balance.\n";
        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n";

        while((P->money < money) && (P->ownedCards.empty() == false)){ //while player can sell houses/mortgage properties

            displayProperties(P);

            std::cout << "Which property would you like to mortgage or sell houses on?\n";
            std::cout << "Enter the ID: ";

            std::string playerResponse;
            
            if (optional){
                std::cout << "\nOr enter q to exit.\n";
            }

            std::cin >> playerResponse;
            
             if (optional && (playerResponse == "q")){
               return false;
            }

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

                        if ((sellHouse > 0) && (sellHouse <= playerTile->houseNum)){ //if player wants to sell houses
                            playerTile->houseNum = playerTile->houseNum - sellHouse;
                            P->money = P->money + (playerCard->houseCost)*sellHouse*0.5; //gives the player back half the house purchase value
                            std::cout << "You have sold " << sellHouse << " houses, worth " << (playerCard->houseCost)*sellHouse*0.5 << "\n";
                        }
                    }
                    else{
                        P->money = P->money + (P->ownedCards[playerResponse])->mort;

                        (P->ownedCards[playerResponse])->mortgage();

                        P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                        P->ownedCards.erase(playerResponse);

                        std::cout << playerResponse << " has been mortgaged.\n";
                    }

                }
                else{ //if not a property type (cannot have houses and hotels), mortgages property.
                 P->money = P->money + (P->ownedCards[playerResponse])->mort;

                 (P->ownedCards[playerResponse])->mortgage();

                  P->mortCards[playerResponse] = P->ownedCards[playerResponse];

                  P->ownedCards.erase(playerResponse);

                  std::cout << playerResponse << " has been mortgaged.\n";
                }

            }

        std::cout << "current balance: " << P->money << "\n";
        std::cout << "amount due: " << money << "\n";

         if (P->money >= money){ //checks if player now has enough money to pay rent. If not, continues loop
            return true;
          }

        std::cout << "You don't have enough money to pay this balance.\n";

        }

        if (!optional){
            Bankruptcy(P, activePlayers, debtor); //if value of houses/properties cannot cover rent, player goes bankrupt.
        }
         return false;
    }

    else{                   // can't mortgage or sell anything
        if (optional == false){
            Bankruptcy(P, activePlayers, debtor); //needs to reference a global PlayerTurn object, which will be in the game loop
            curPisBankrupt = true;
        }
        return false;
    }
}

void Bankruptcy(Player* P, PlayerTurn Turn, Player * debtor) {
    if(debtor != nullptr){
        debtor->money += P->money;
        for(auto i : P->mortCards){
            if(i.second->type == "Property"){
                dynamic_cast<PropertyCard*>(i.second)->linkTile->owner = debtor;
            }
            else if(i.second->type == "RailRoad"){
                dynamic_cast<RailRoadCard*>(i.second)->linkTile->owner = debtor;
            }
            else if(i.second->type == "Utility"){
                dynamic_cast<UtilityCard*>(i.second)->linkTile->owner = debtor;
            }
            debtor->mortCards[i.first] = i.second;
        }
    }
    else{
        for(auto i : P->mortCards){
            if(i.second->type == "Property"){
                dynamic_cast<PropertyCard*>(i.second)->linkTile->owner = debtor;
                dynamic_cast<PropertyCard*>(i.second)->linkTile->isMortgaged = false;
            }
            else if(i.second->type == "RailRoad"){
                dynamic_cast<RailRoadCard*>(i.second)->linkTile->owner = debtor;
                dynamic_cast<RailRoadCard*>(i.second)->linkTile->isMortgaged = false;
            }
            else if(i.second->type == "Utility"){
                dynamic_cast<UtilityCard*>(i.second)->linkTile->owner = debtor;
                dynamic_cast<UtilityCard*>(i.second)->linkTile->isMortgaged = false;
            }

        }
    }

    while(!(P->DeckCards.empty())){
        (P->DeckCards.begin())->second->doDeckCardFunction(P);
    }
    std::cout << P->name << " has gone bankrupt!\n";
    Turn.RemovePlayer(P);  //removes player from playerTurn object
    std::cout<<activePlayers.playerList.size()<<std::endl;
}

int checkMonopoly(Player * P, Card * C){

    std::string suitCard = C->cardID; //gets ID of current card
    int numCards = int(C->cardID[2]) - 48; //the third letter of the cardID is the total number of cards in the suit
    int cardNum = int(C->cardID[3]) - 48; //the fourth letter is the number of the card in the suit (0, 1, 2, 3, etc.)
    int monopolyCount = 0;

    for (int i = 0; i < numCards; i++){ //Searches ownedProperties for other cards in the suit. We can find their cardIDs systematically.
        if (i != cardNum) {
            suitCard[3] = char(i + 48);
            if(P->ownedCards.find(suitCard) != P->ownedCards.end()){
                monopolyCount = monopolyCount + 1;
            }
        }
    }
    return monopolyCount+1; //returns the number of other cards in the suit the player owns
}

void buyProperty(Player * P){
    Tile* purchaseTile = Board[P->location]; //gets the tils to purchase from the player's location.
    std::cout<<"Money: $"<<P->money<<std::endl;
    std::cout << "Purchase property " << purchaseTile->name << "? (y/n)\n";

    std::string playerResponse;

    std::cin >> playerResponse;

    if (playerResponse == "y"){

        if (purchaseTile->type == "Property"){
            PropertyTile* purchaseProperty = dynamic_cast<PropertyTile*>(purchaseTile); //casts to PropertyTile to use the linkedCard member

            OwnableCard* purchaseCard = purchaseProperty->linkedCard; //gets the linked card to insert to ownedProperties

            if (checkBalance(P,purchaseCard->saleValue,true, nullptr)){ //if player has enough money to buy property
                purchaseProperty->owner = P;
                P->money -= purchaseProperty->linkedCard->saleValue;
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

            if (checkBalance(P,purchaseCard->saleValue,true, nullptr)){
                purchaseProperty->owner = P;
                P->money -= purchaseProperty->linkedCard->saleValue;
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

            if (checkBalance(P,purchaseCard->saleValue,true, nullptr)){
                purchaseProperty->owner = P;
                P->money -= purchaseProperty->linkedCard->saleValue;
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

    std::cout << P->name << " moved to tile# " << P->location << ": " << Board[P->location]->name << std::endl;

}

int diceRoll(){
    srand(time(0));
    int roll=(rand()%6) +1;
    return roll;
}

void buyHouses(Player * P) { //called at the end of player turn
    
    std::unordered_map<std::string, PropertyCard*> houseCards; //a new map of cards that the player can buy houses on
    
    std::cout << "You can buy houses/hotels on the following properties:\n";

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
        std::cout<<"There are no properties that you can buy houses on"<<std::endl;
    }
    else {

        std::string playerResponse;

        std::cout << "Which property would you like to buy a house/hotel on?\n";

        std::cin >> playerResponse;

        if (houseCards.find(playerResponse) != houseCards.end()) { //if the player's choice is in their ownedProperties deck

            PropertyCard *playerCard = houseCards[playerResponse]; //gets correct property card based on player response

            PropertyTile *playerTile = playerCard->linkTile; //gets linked tile

            if (playerTile->houseNum == 4) { //if player can buy a hotel
                std::cout << "Would you like to buy a hotel? (y/n)\n";

                std::string buyHotel;
                std::cin >> buyHotel;

                if ((buyHotel == "y") && checkBalance(P, playerCard->hotelCost, true, nullptr)) {
                    playerTile->hotelNum = 1;
                    playerTile->houseNum = 0;
                    P->money = P->money - (playerCard->hotelCost);
                    std::cout << "You have bought a hotel";
                }
            } 
            else {
                std::cout << "How many houses would you like to buy? (1-4)\n"; //if player cant buy a hotel but can buy houses

                int buyHouse;
                std::cin >> buyHouse;

                //if player wants to and can buy houses
                if ((buyHouse <= (4 - playerTile->houseNum)) && checkBalance(P, ((playerCard->houseCost) * buyHouse), true)) {
                    playerTile->houseNum = playerTile->houseNum + buyHouse;
                    //deducts the house purchase value from player balance
                    P->money = P->money - (playerCard->houseCost) * buyHouse;
                    std::cout << "You have bought " << buyHouse << " houses.\n";
                }
                else{
                    std::cout<<"Invalid House Number"<<std::endl;
                }
            }
        }
    }
}


void printStartTurn(Player * P) {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "PLAYER " << P->name << "'S TURN" << std::endl;
    std::cout << "Player " << P->name << " is on tile # " << P->location << std::endl;
    std::cout << "Player " << P->name << " has $" << P->money << std::endl;
    displayProperties(P);
}

//handles player turns while player is in jail. Returns true if player gets out of jail. False otherwise.
bool inJail(Player * P){
    P->location = 10;
    P->inJail = true;
    std::string playerResponse = "";
    
    std::cout<<"You are in Jail"<<std::endl;
    
    if (checkGOOJ(P)) return true;

    /* if (P->DeckCards.find("GetOutOfJail") != P->DeckCards.end()){ 
        std::cout << "You have a get out of jail free card. Would you like to use it? (y/n)\n";
        std::cin >> playerResponse;
        
        if (playerResponse == "y"){
            (P->DeckCards["GetOutOfJail"])->doDeckCardFunction(P); 
            std::cout << "You are out of jail.\n";
            P->turnsInJail = 0;
            P->DeckCards.erase("GetOutOfJail");
            return true;
        }
    } */
    
    std::cout<<"Rolling to get out of Jail"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
     int dice1 = diceRoll();
     std::this_thread::sleep_for(std::chrono::seconds(4));
     int dice2 = diceRoll();
     int totalRoll = dice1 + dice2;
     std::cout << "Dice 1: " << dice1 << " Dice 2: " << dice2 << "\n";
     
     if (dice1 == dice2){
         std::cout << "You rolled doubles and got out of jail.\n";
         P->inJail = false;
         P->turnsInJail = 0;
         return true;
     }
     else{
         std::cout << "Would you like to pay $50 to get out of jail? (y/n)\n ";
         std::cin >> playerResponse;
         
         if (playerResponse == "y"){
             if (checkBalance(P,50,true)){
                 P->money = P->money - 50;
                 P->inJail = false;
                 P->turnsInJail = 0;
                 return true;
             }
             else{
                 std::cout << "You don't have enough money to get out of jail.\n";
             }
         }
     }
     
     if (P->inJail == true){
         P->turnsInJail += 1;
         
         if (P->turnsInJail > 2){
             std::cout << "You have been in jail for three turns and must pay to get out.\n";
             
             if (checkBalance(P,50,false, nullptr)){
                 P->money = P->money - 50;
                 P->inJail = false;
                 P->turnsInJail = 0;
                 return true;
             }
             else{
                 std::cout << "You don't have enough money to get out of jail and went bankrupt.\n";
             }
         }
         else{
            std::cout << "You are still in jail.\n";
            return false;
         }
     }
     return false;
}

bool checkGOOJ(Player * P) {
    if (P->DeckCards.size()) {
        std::cout << "You have a get out of jail free card. Would you like to use it? (y/n)\n";
        char useCard;
        std::cin >> useCard;
        if (useCard == 'y') {
            std::cout << "Used Get Out Of Jail Free card." << std::endl;
            std::cout << "You are out of Jail at tile #10." << std::endl;
            
            // find a card
            (P->DeckCards.begin())->second->doDeckCardFunction(P);
            return true;
        }
        else {
            std::cout << "You are still in Jail." << std::endl;
            return false;
        }
    }
    else{
        return true;
    }
}