//
// Created by mdeno on 11/28/2022.
//

#include <iostream>
#include <string>
#include <unordered_map>
#include <math.h>
#include <queue>


class Tile;
class Player;
class Card;
class RailRoadCard;
class PropertyCard;
class DeckCard;
class UtilityCard;
class getMoneyCard;
class RailRoadTile;
class PropertyTile;
class UtilityTile;
class TaxTile;
class GoToJailTile;
class DrawCardTile;

class Player{
public:
    int money;
    //hash table of owned properties
    std::unordered_map<std::string, Card*> ownedCards;
    //hash table of mortgated properties
    std::unordered_map<std::string, Card*> mortCards;
    //hash table of deck cards (get out of jail free)
    std::unordered_map<std::string, DeckCard*> DeckCards;
    std::string name;
    int location;
    bool inJail;

    Player(int money = 1500, std::unordered_map<std::string, Card*> ownedCards = {}, std::unordered_map<std::string, Card*> mortCards = {}, std::string name = "", int location = 0, bool inJail = false){
        this->ownedCards = ownedCards;
        this->money = money;
        this->mortCards = mortCards;
        this->name = name;
        this->location = location;
        this->inJail = inJail;
    }


};

class Card{
public:
    std::string name;
    std::string cardID;
    std::string type; //note: Allow constructor to use default value for derived classes

    Card(std::string name="",std::string cardID="",std::string type=""){
        this->name = name;
        this->cardID = cardID;
    }
};

class RailRoadCard : public Card{
public:
    int baseRent; //railroad rent is doubled for each property owned
    int mortValue;
    RailRoadTile * linkTile;

    RailRoadCard(int baseRent=25, int mortValue = 100, std::string name="RR", std::string cardID="", RailRoadTile* linkTile=nullptr, std::string type="RR"):Card(name,cardID, type){
        this->baseRent = baseRent;
        this->mortValue = mortValue;
        this->name = name;
        this->linkTile = linkTile;
        this->cardID = cardID;
    }

};

class PropertyCard : public Card{
public:
    std::string color;
    int saleValue;
    int baseRent;
    int * houseRent;
    int hotelRent;
    int mort;
    int houseCost;
    int hotelCost;
    PropertyTile * linkTile;

    PropertyCard(std::string color, std::string name, std::string cardID, int saleValue, int baseRent, int houseRent[4], int hotelRent, int mort, int houseCost, int hotelCost, PropertyTile * linkTile, std::string type="Property"): Card(name,cardID,type){
        this->saleValue = saleValue;
        this->baseRent = baseRent;
        this->houseRent = houseRent; //Test that this works!!!
        this->hotelRent = hotelRent;
        this->mort = mort;
        this->houseCost = houseCost;
        this->hotelCost = hotelCost;
        this->linkTile = linkTile;
        this->cardID = cardID;
    }
};

class UtilityCard : public Card{
public:
    int saleValue;
    int mort;
    int rentOneUtil;
    int rentTwoUtil;
    UtilityTile * linkTile;

    UtilityCard(std::string name, std::string cardID, int saleValue, int mort, int rentOneUtil, int rentTwoUtil, UtilityTile * linkTile, std::string type="Utility"): Card(name,cardID, type){
        this->saleValue = saleValue;
        this->mort = mort;
        this->rentOneUtil = rentOneUtil;
        this->rentTwoUtil = rentTwoUtil;
        this->linkTile = linkTile;
        this->cardID = cardID;
    }
};

class DeckCard : public Card{ // ABC for all cards in chance/community chest queues
    public:
    std::string cardDesc;

    DeckCard(std::string name, std::string cardDesc, std::string cardID, std::string type): Card(name,cardID, type){
        this->cardDesc = cardDesc;
    }
    virtual void doDeckCardFunction(Player * P) = 0;
    };

class getMoneyCard : public DeckCard{ //get money from bank or pay money to bank
    int moneyAmount; //negative if player pays to bank
    std::string cardDesc;

    getMoneyCard(int moneyAmount, std::string cardDesc, std::string name, std::string cardID, std::string type="getMoney"):DeckCard(name,cardDesc,cardID,type){
        this->moneyAmount = moneyAmount;
        this->cardDesc = cardDesc;
    }

    void doDeckCardFunction(Player * P){
        P->money = P->money + moneyAmount;
    }
};

class getOutOfJailCard : public DeckCard{
    
    void doDeckCardFunction(Player * P){
        P->inJail = false;
        P->DeckCards.erase(name);
    }
};


class Tile{
public:
    std::string name;

    Tile(std::string name){
        this->name = name;
    }

    virtual void doCardFunction() = 0;
};

class PropertyTile : public Tile{
public:
    PropertyCard * linkedCard;
    int houseNum;
    int hotelNum;
    Player * owner;
    bool isMortgaged;

PropertyTile(std::string name, PropertyCard * linkedCard, int houseNum = 0, int hotelNum = 0, Player * owner = nullptr, bool isMortgaged = false) : Tile(name){
        this->linkedCard = linkedCard;
        this->houseNum = houseNum;
        this->hotelNum = hotelNum;
        this->owner = owner;
        this->isMortgaged = isMortgaged;
}

void doCardFunction(Player * P) {
    if (owner != nullptr && (isMortgaged == false)){
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
        buyProperty(P);
    }
}
};

class RailRoadTile : public Tile{
public:
    RailRoadCard * linkedCard;
    Player * owner;
    bool isMortgaged;

    RailRoadTile(std::string name, RailRoadCard * linkedCard, Player * owner = nullptr, bool isMortgaged = false) : Tile(name) {
        this->linkedCard = linkedCard;
        this->owner = owner;
        this->isMortgaged = isMortgaged;
    }

    void doCardFunction(Player * P) {
        if (owner != nullptr && (isMortgaged == false)) {
            int rent = linkedCard->baseRent;
            int monopoly = checkMonopoly(P, linkedCard);

            rent = rent * pow(2, (monopoly - 1));

            checkBalance(P, rent);
            P->money = P->money - rent;
            owner->money = owner->money + rent;
        } else {
            buyProperty(P);
        }
    }
};

class TaxTile : public Tile{
public:
    int taxValue;

    TaxTile(std::string name, int taxValue=0): Tile(name) {
        this->taxValue = taxValue;
    }

    void doCardFunction(Player * P) {
        checkBalance(P);
        P->money = P->money - taxValue;
    }

};

class UtilityTile : public Tile{
public:
    UtilityCard * linkedCard;
    Player * owner;
    bool isMortgaged;

    UtilityTile(std::string name, UtilityCard * linkedCard, Player * owner = nullptr, bool isMortgaged = false) : Tile(name) {
            this->linkedCard = linkedCard;
            this->owner = owner;
            this->isMortgaged = isMortgaged;
    }

    void doCardFunction(Player * P) {
        if (owner != nullptr && (isMortgaged == false)) {
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
            buyProperty(P);
        }
    }
};

class GoToJailTile : public Tile{
public:
    int jailLocation;
    
    GoToJailTile(std::string name, int jailLocation):Tile(name){
        this->jailLocation = jailLocation;
    }
    
    void doCardFunction(Player * P) {
        P->inJail = true;
        P->location = jailLocation;
    }
};

class DrawCardTile : public Tile{
public:
    std::queue<DeckCard*> cardDeck;
    
    DrawCardTile(std::queue<DeckCard*> cardDeck) : Tile(name) {
        this->cardDeck = cardDeck;
    }
    
    void doCardFunction(Player * P){
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
};

//Free Parking, Jail, and Go will just be implementations of Card and do not have dervied classes.
//Their functionality will be done in the game loop



