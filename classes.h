//
// Created by mdeno on 11/28/2022.
//

#include <iostream>
#include <string>
#include <unordered_map>


class Tile;
class Player;
class Card;

class Player{
public:
    int money;
    //hash table of owned properties
    std::unordered_map<std::string, Card*> ownedCards;
    //hash table of mortgated properties
    std::unordered_map<std::string, Card*> mortCards;
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

    Card(std::string name="",std::string cardID){
        this->name = name;
        this->cardID = cardID;
    }
};

class RailRoadCard : Card{
public:
    int baseRent; //railroad rent is doubled for each property owned
    int mortValue;
    Tile * linkTile;

    RailRoadCard(int baseRent=25, int mortValue = 100, std::string name="RR", std::string cardID,Tile* link= nullptr):Card(name,cardID){
        this->baseRent = baseRent;
        this->mortValue = mortValue;
        this->name = name;
        this->linkTile = link;
        this->cardID = cardID;
    }

};

class PropertyCard : Card{
public:
    std::string color;
    int saleValue;
    int baseRent;
    int * houseRent;
    int hotelRent;
    int mort;
    int houseCost;
    int hotelCost;
    Tile * linkTile;

    PropertyCard(std::string color, std::string name, std::string cardID, int saleValue, int baseRent, int houseRent[4], int hotelRent, int mort, int houseCost, int hotelCost, Tile * linkTitle): Card(name,cardID){
        this->saleValue = saleValue;
        this->baseRent = baseRent;
        this->houseRent = houseRent; //Test that this works!!!
        this->hotelRent = hotelRent;
        this->mort = mort;
        this->houseCost = houseCost;
        this->hotelCost = hotelCost;
        this->linkTile = linkTitle;
        this->cardID = cardID;
    }
};

class UtilityCard : Card{
public:
    int saleValue;
    int mort;
    int rentOneUtil;
    int rentTwoUtil;

    UtilityCard(std::string name, std::string cardID, int saleValue, int mort, int rentOneUtil, int rentTwoUtil): Card(name,cardID){
        this->saleValue = saleValue;
        this->mort = mort;
        this->rentOneUtil = rentOneUtil;
        this->rentTwoUtil = rentTwoUtil;
        this->cardID = cardID;
    }
};

class getMoneyCard : Card{
    int moneyAmount;
    std::string cardDesc;

    getMoneyCard(int moneyAmount, std::string cardDesc){
        this->moneyAmount = moneyAmount;
        this->cardDesc = cardDesc;
    }
};

class Tile{
public:
    std::string name;

    Tile(std::string name){
        this->name = name;
    }

    virtual doCardFunction() = 0;
};

class PropertyTile : Tile{
public:
    Card * linkedCard;
    int houseNum;
    int hotelNum;
    Player * owner;
    bool isMonopoly;
    bool isMortgaged;

    PropertyTile(std::string name, Card * linkedCard, int houseNum = 0, int hotelNum = 0, Player * owner = nullptr, bool isMonopoly = false, bool isMortgaged = false) : Tile(name){
        this->linkedCard = linkedCard;
        this->houseNum = houseNum;
        this->hotelNum = hotelNum;
        this->owner = owner;
        this->isMonopoly = isMonopoly;
        this->isMortgaged = isMortgaged;
    }

    void doCardFunction(Player * P) {
        if (owner != nullptr){
            int rent;
            
            if (hotelNum != 0){
                rent = linkedCard->hotelRent;
            }
            else if ((hotelNum == 0) && (houseNum != 0)){
                rent = linkedCard->houseRent[houseNum];
            }
            else{
                rent = linkedCard->baseRent;
                int monopoly = checkMonopoly(P, linkedCard);
                
                if (monopoly >= linkedCard->int(cardID[1])){
                    rent = rent*2;
                }
            }
            P->money = P->money - rent;
            owner->money = owner->money + rent;
        }
        else{
            buyProperty(P);
        }
    }
    };

class RailRoadTile : Tile{
public:
    //impliment me!
};

class DrawCardTile : Tile{
public:
    //impliment me!
};

class TaxTile : Tile{
public:
    //impliment me!
};

class UtilityTile : Tile{
public:
    //impliment me!
};

class GoToJailTile : Tile{
public:
    //impliment me!
};

//Free Parking, Jail, and Go will just be implementations of Card and do not have dervied classes.
//Their functionality will be done in the game loop



