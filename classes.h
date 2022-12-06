//
// Created by mdeno on 11/28/2022.
//

#include <iostream>
#include <string>
#include <unordered_map>
#include <math.h>
#include <queue>
#include <list>


class Tile;
class Player;
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

class PlayerTurn{
    public:
    std::list<Player*> playerList;
    std::list<Player*>::iterator it;
    Player* currentPlayer;
    
    PlayerTurn(std::list<Player*> playerList){
        this->playerList = playerList;
        it = playerList.begin();
        currentPlayer = *it;
    }
    
    void next_player(){
        if (it == playerList.end()){
            it = playerList.begin();
        }
        else{
            std::advance(it,1);
        }
        currentPlayer = *it;
    }
    
    void RemovePlayer(Player* P){  
        
        if (P == currentPlayer){
            next_player();
        }
        playerList.remove(P);
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
    
    void mortgage(); //Defined after tile definition because it uses tile member variables
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
    PropertyTile * linkTile;

    PropertyCard(std::string color, std::string name, std::string cardID, int saleValue, int baseRent, int houseRent[4], int hotelRent, int mort, int houseCost, int hotelCost, PropertyTile * linkTile, std::string type="Property"):OwnableCard(name,mort,saleValue,cardID, type){
        this->baseRent = baseRent;
        this->houseRent = houseRent; //Test that this works!!!
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
    UtilityTile * linkTile;

    UtilityCard(std::string name, std::string cardID, int saleValue, int mort, int rentOneUtil, int rentTwoUtil, UtilityTile * linkTile, std::string type="Utility"):OwnableCard(name,mort,saleValue,cardID, type){
        this->rentOneUtil = rentOneUtil;
        this->rentTwoUtil = rentTwoUtil;
        this->linkTile = linkTile;
    }
    
    void mortgage(); //Defined after tile definition because it uses tile member variables
    void unmortgage();
    void buy(Player * P);
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
        
        if (moneyAmount < 0 && checkBalance(P,(-1*moneyAmount))){
            P->money = P->money + moneyAmount;
        }
        else{
            P->money = P->money + moneyAmount;
        }
    }
};

class transferMoneyCard : public DeckCard{ //get money from other player(s) or pay money to other player(s)
    int moneyAmount; //negative if player pays money
    std::vector<Player*> playerList;
    std::string cardDesc;

    transferMoneyCard(int moneyAmount, std::vector<Player*> playerList, std::string cardDesc, std::string name, std::string cardID, std::string type="getMoney"):DeckCard(name,cardDesc,cardID,type){
        this->moneyAmount = moneyAmount; // positive if player recieves money, negative if player pays money;
        this->cardDesc = cardDesc;
        this->playerList = playerList;
    }

    void doDeckCardFunction(Player * P){
    
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

void PropertyCard::mortgage(){
    linkTile->isMortgaged = true;
}
void PropertyCard::unmortgage(){
    linkTile->isMortgaged = false;
}
void PropertyCard::buy(Player * P){
    linkTile->owner = P;
}

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

void RailRoadCard::mortgage(){
    linkTile->isMortgaged = true;
}
void RailRoadCard::unmortgage(){
    linkTile->isMortgaged = false;
}
void RailRoadCard::buy(Player * P){
    linkTile->owner = P;
}

class TaxTile : public Tile{
public:
    int taxValue;

    TaxTile(std::string name, int taxValue=0): Tile(name) {
        this->taxValue = taxValue;
    }

    void doCardFunction(Player * P) {
        if (checkBalance(P, taxValue) == true){
            P->money = P->money - taxValue;
        }
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

void UtilityCard::mortgage(){
    linkTile->isMortgaged = true;
}
void UtilityCard::unmortgage(){
    linkTile->isMortgaged = false;
}
void UtilityCard::buy(Player * P){
    linkTile->owner = P;
}

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

