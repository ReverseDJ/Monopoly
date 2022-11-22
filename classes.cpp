#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Tile;
class Player;
class Card;

class Player{
public:
    int money;
    //hash table of owned properties
    unordered_map<string, Card*> ownedCards;
    //hash table of mortgated properties
    unordered_map<string, Card*> mortCards;
    string name;
    string piece;
    int locatoin;
    bool inJail;

};

class Card{
public:
    string name;

    Card(string name=""){
        name = name;
    }
};

class railRoads : Card{
public:
    int baseRent; //railroad rent is doubled for each property owned
    int mortValue;
    Tile * linkTile;

    railRoads(int baseRent=25, int mortValue = 100, string name="RR", Tile* link= nullptr):Card(name){
        this->baseRent = baseRent;
        this->mortValue = mortValue;
        this->name = name;
        this->linkTile = link;
    }

};

class Property : Card{
public:
    string color;
    int saleValue;
    int baseRent;
    int * houseRent;
    int hotelRent;
    int mort;
    int houseCost;
    int hotelCost;
    Tile * linkTile;

    Property(string color, string name, int saleValue, int baseRent, int houseRent[4], int hotelRent, int mort, int houseCost, int hotelCost, Tile * linkTitle): Card(name){
        this->saleValue = saleValue;
        this->baseRent = baseRent;
        this->houseRent = houseRent; //Test that this works!!!
        this->hotelRent = hotelRent;
        this->mort = mort;
        this->houseCost = houseCost;
        this->hotelCost = hotelCost;
        this->linkTile = linkTitle;
    }
};

class Utility : Card{
public:
    int saleValue;
    int mort;
    int rentOneUtil;
    int rentTwoUtil;

    Utility(string name, int saleValue, int mort, int rentOneUtil, int rentTwoUtil): Card(name){
        this->saleValue = saleValue;
        this->mort = mort;
        this->rentOneUtil = rentOneUtil;
        this->rentTwoUtil = rentTwoUtil;
    }
};

class getMoneyCard : Card{
    int moneyAmount;
    string cardDesc;

    getMoneyCard(int moneyAmount, string cardDesc){
        this->moneyAmount = moneyAmount;
        this->cardDesc = cardDesc;
    }
};

class Tile{
public:
    string name;

    Tile(string name){
        this->name = name;
    }

    virtual void doCardFunction(Player *) = 0;
};

class PropertyTile : Tile{
public:
    Card * linkedCard;
    int houseNum;
    int hotelNum;
    Player * owner;
    bool isMonopoly;
    bool isMortgaged;

    PropertyTile(string name, Card * linkedCard, int houseNum = 0, int hotelNum = 0, Player * owner = nullptr, bool isMonopoly = false, bool isMortgaged = false) : Tile(name){
        this->linkedCard = linkedCard;
        this->houseNum = houseNum;
        this->hotelNum = hotelNum;
        this->owner = owner;
        this->isMonopoly = isMonopoly;
        this->isMortgaged = isMortgaged;
    }

    void doCardFunction(Player * P) {

    }
};

int main() {

    return 0;
}
