//
// Created by mdeno on 11/28/2022.
//

#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <math.h>
#include <queue>
#include <list>
#include <deque>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

class Tile; //class prototypes, allows other classes to reference before definition.
class Player;
class PlayerTurn;
class Card;
class RailRoadCard;
class PropertyCard;
class DeckCard;
class OwnableCard;
class UtilityCard;
class getMoneyCard;
class RailRoadTile;
class PropertyTile;
class UtilityTile;
class TaxTile;
class GoToJailTile;
class DrawCardTile;

// PLAYER CLASSES
class Player{
public:
    int money;
    //hash table of owned properties
    std::unordered_map<std::string, OwnableCard*> ownedCards;
    //hash table of mortgated properties
    std::unordered_map<std::string, OwnableCard*> mortCards;
    //hash table of deck cards (get out of jail free)
    std::unordered_map<std::string, DeckCard*> DeckCards;
    std::string name;
    int location;
    bool inJail;

    Player(int money = 1500, std::unordered_map<std::string, OwnableCard*> ownedCards = {}, std::unordered_map<std::string, OwnableCard*> mortCards = {}, std::string name = "", int location = 0, bool inJail = false){
        this->ownedCards = ownedCards;
        this->money = money;
        this->mortCards = mortCards;
        this->name = name;
        this->location = location;
        this->inJail = inJail;
    }


};

class PlayerTurn{ //will be instantiated as a global object that keeps track of which player is currently moving. 
public:
    std::list<Player*> playerList;  //list of all players in the game
    std::list<Player*>::iterator it;
    Player* currentPlayer;

    PlayerTurn(std::list<Player*> playerList){
        this->playerList = playerList;
        it = playerList.begin();
        currentPlayer = *it;
    }

    void next_player(){

        if (std::next(it) == playerList.end()){ //if we have reached the end of the list, start over. The player turns should be circular.
            it = playerList.begin();
        }
        //else{
        //    std::advance(it,1); //iterates to the next item on the list, which moved current player to the next player
        //}
        currentPlayer = dynamic_cast<Player*>(*it);
    }

    void RemovePlayer(Player* P){ //removes a player from the list when they go bankrupt

        if (P == currentPlayer){
            next_player();
        }
        playerList.remove(P);
    }

};


// CARD CLASSES
class Card{
public:
    std::string name;
    std::string cardID; //systematicly assigned by suit. This allows us to check for monopolies in ownedCards.
    std::string type; //note: Allow constructor to use default value for derived classes

    Card(std::string name="",std::string cardID="",std::string type=""){
        this->name = name;
        this->cardID = cardID;
        this->type = type;
    }
};

class OwnableCard : public Card{ // ABC for all property, rail road and utility cards, enables mortgage and sale values without casting
    public:
    
    int mort;
    int saleValue;

    OwnableCard(std::string name, int mort, int saleValue, std::string cardID, std::string type): Card(name,cardID, type){
        this->saleValue = saleValue;
        this->mort = mort;
    }
    virtual void mortgage() = 0;
    virtual void unmortgage() = 0;
    virtual void buy(Player * P) = 0;
};

class RailRoadCard : public OwnableCard{
public:
    int baseRent; //railroad rent is doubled for each property owned
    RailRoadTile * linkTile;

    RailRoadCard(int baseRent=25, int mort = 100, int saleValue = 200, std::string name="RR", std::string cardID="", RailRoadTile* linkTile=nullptr, std::string type="RR"):OwnableCard(name,mort,saleValue,cardID, type){
        this->baseRent = baseRent;
        this->linkTile = linkTile;
    }

    RailRoadCard(std::string name, std::string cardID, RailRoadTile* linkTile):RailRoadCard(25, 100, 200, name, cardID, linkTile, "RR") {}
    
    void mortgage(); //Defined after tile definition because it uses tile member variables. 
    void unmortgage();
    void buy(Player * P);

};

class PropertyCard : public OwnableCard{
public:
    std::string color;
    int baseRent;
    int * houseRent;
    int hotelRent;
    int houseCost;
    int hotelCost;
    PropertyTile * linkTile; //links to a property tile

    PropertyCard(std::string color, std::string name, std::string cardID, int saleValue, int baseRent, int houseRent[4], int hotelRent, int mort, int houseCost, int hotelCost, PropertyTile * linkTile, std::string type="Property"):OwnableCard(name,mort,saleValue,cardID, type){
        this->baseRent = baseRent;
        this->houseRent = houseRent; 
        this->hotelRent = hotelRent;
        this->houseCost = houseCost;
        this->hotelCost = hotelCost;
        this->linkTile = linkTile;
    }
    
    void mortgage(); //Defined after tile definition because it uses tile member variables
    void unmortgage();
    void buy(Player * P);
};

class UtilityCard : public OwnableCard{
public:
    int rentOneUtil;
    int rentTwoUtil;
    UtilityTile * linkTile; //links to a utility tile

    UtilityCard(std::string name, std::string cardID, int saleValue, int mort, int rentOneUtil, int rentTwoUtil, UtilityTile * linkTile, std::string type="Utility"):OwnableCard(name,mort,saleValue,cardID, type){
        this->rentOneUtil = rentOneUtil;
        this->rentTwoUtil = rentTwoUtil;
        this->linkTile = linkTile;
    }
    
    void mortgage(); //Defined after tile definition because it uses tile member variables
    void unmortgage();
    void buy(Player * P);
};


// DECK CARDS
class DeckCard : public Card{ // ABC for all cards in chance/community chest queues
    public:
    std::string cardDesc; //will be printed during the game

    DeckCard(std::string name, std::string cardDesc, std::string cardID, std::string type): Card(name,cardID, type){
        this->cardDesc = cardDesc;
    }
    virtual void doDeckCardFunction(Player * P) = 0;
};

class getMoneyCard : public DeckCard{ //get money from bank or pay money to bank
    int moneyAmount; //negative if player pays to bank

public:
    getMoneyCard(int moneyAmount, std::string cardDesc, std::string name, std::string cardID, std::string type="getMoney"):DeckCard(name,cardDesc,cardID,type){
        this->moneyAmount = moneyAmount;
    }

    void doDeckCardFunction(Player * P) override;
};

class transferMoneyCard : public DeckCard{ //get money from other player(s) or pay money to other player(s)
    int moneyAmount; //negative if player pays money

public:
    transferMoneyCard(int moneyAmount, std::string cardDesc, std::string name, std::string cardID, std::string type="transferMoney"):DeckCard(name,cardDesc,cardID,type){
        this->moneyAmount = moneyAmount; // positive if player recieves money, negative if player pays money;
    }

    void doDeckCardFunction(Player * P);
    
};

class getOutOfJailCard : public DeckCard{
public:
    getOutOfJailCard(std::string cardDesc, std::string name, std::string cardID, std::string type="GOOJ"):DeckCard(name, cardDesc, cardID, type ){}
    void doDeckCardFunction(Player * P) override;
};

class movePlayerCard : public DeckCard{
public:
    int dest;
    bool inst;
    movePlayerCard(int dest, bool inst, std::string cardDesc, std::string name="",std::string cardID="", std::string type="movePlayer"): DeckCard(name, cardDesc, cardID, type){
        this->dest = dest;
        this->inst = inst;
    }
    void doDeckCardFunction(Player * P) override;
};

class payPerBuildingCard : public DeckCard{
public:
    int amtPerHouse;
    int amtPerHotel;

    payPerBuildingCard(int amtPerHouse, int amtPerHotel, std::string cardDesc, std::string name="",std::string cardID="", std::string type="payPerBuilding"): DeckCard(name, cardDesc, cardID, type){
        this->amtPerHotel = amtPerHotel;
        this->amtPerHouse = amtPerHouse;
    }
    void doDeckCardFunction(Player * P) override;
};


// TILE CLASSES
class Tile{ //Abstract base class for all tile types. Allows us to store every tile in a common board array for player traversal
public:
    std::string name;
    std::string type;

    Tile(std::string name, std::string type = ""){
        this->name = name;
        this->type = type;
    }

    virtual void doTileFunction(Player * P) = 0; //will allow each tile to perform actions when players land on it. 

};

class PropertyTile : public Tile{
public:
    PropertyCard * linkedCard;
    int houseNum;
    int hotelNum;
    Player * owner;
    bool isMortgaged;

    PropertyTile(std::string name, PropertyCard * linkedCard, int houseNum = 0, int hotelNum = 0, Player * owner = nullptr, bool isMortgaged = false, std::string type="Property") : Tile(name, type){
            this->linkedCard = linkedCard;
            this->houseNum = houseNum;
            this->hotelNum = hotelNum;
            this->owner = owner;
            this->isMortgaged = isMortgaged;
    }

    void doTileFunction(Player * P);
};

class RailRoadTile : public Tile{
public:
    RailRoadCard * linkedCard;
    Player * owner;
    bool isMortgaged;

    RailRoadTile(std::string name, RailRoadCard * linkedCard, Player * owner = nullptr, bool isMortgaged = false, std::string type="RR") : Tile(name, type) {
        this->linkedCard = linkedCard;
        this->owner = owner;
        this->isMortgaged = isMortgaged;
    }

    void doTileFunction(Player * P);
};

class TaxTile : public Tile{
public:
    int taxValue;

    TaxTile(std::string name, int taxValue=0, std::string type="Tax") : Tile(name, type){
        this->taxValue = taxValue;
    }

    void doTileFunction(Player * P);

};

class UtilityTile : public Tile{
public:
    UtilityCard * linkedCard;
    Player * owner;
    bool isMortgaged;

    UtilityTile(std::string name, UtilityCard * linkedCard, Player * owner = nullptr, bool isMortgaged = false, std::string type="Utility") : Tile(name, type) {
            this->linkedCard = linkedCard;
            this->owner = owner;
            this->isMortgaged = isMortgaged;
    }

    void doTileFunction(Player * P);
};

class GoToJailTile : public Tile{
public:
    int jailLocation;
    
    GoToJailTile(std::string name, int jailLocation, std::string type="GotoJail") : Tile(name, type){
        this->jailLocation = jailLocation;
    }
    
    void doTileFunction(Player * P);
};

class CornerTile : public Tile{
public:

    CornerTile(std::string name, std::string type = "Corner") : Tile(name, type) { }
    void doTileFunction(Player * P);

};

class DrawCardTile : public Tile{
public:
    std::string name;
    std::deque<DeckCard*> * cardDeck;
    
    DrawCardTile(std::string name, std::deque<DeckCard*>* cardDeck, std::string type="DrawCard") : Tile(name, type) {
        this->cardDeck = cardDeck;
    }
    
    void doTileFunction(Player * P);
};

//Free Parking, Jail, and Go will just be implementations of Card and do not have dervied classes.
//Their functionality will be done in the game loop

#endif