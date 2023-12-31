//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"
#include "filepath.h" // ADD THIS FILE TO YOUR WORKING DIRECTORY
#include <cstring>

Tile * Board[40];
Player * curP;
std::unordered_map<std::string,OwnableCard*> bankCards;
bool curPisBankrupt=false;

std::deque<DeckCard*> chanceCards;
std::deque<DeckCard*> communityChestCards;
//PlayerTurn * activePlayers;
PlayerTurn activePlayers({});

int botMoneyStart; // blame Carlson


/*
int main(){
    std::cout<<"This Shit Compiles"<<std::endl;
}
*/

int main() {


    //makeBoard(MAKEBOARD_CSV, Board);
    //makeDrawCards(MAKECHANCE_TSV, &chanceCards);
    //makeDrawCards(MAKECHEST_TSV, &communityChestCards);

    makeBoard("makeboard.csv", Board);
    makeDrawCards("makechance.tsv", &chanceCards);
    makeDrawCards("makechest.tsv", &communityChestCards);

    std::cout<<"Welcome to Monopoly!\n";
    int players;
    std::cout<<"How many players?\n";
    std::cin>>players;
    char answer;

    std::list<Player*> playerList;

    for (int i = 1; i <= players; i++){
        std::string playerName;
        std::cout << "\nEnter player " << i << " name: ";
        std::cin >> playerName;
        Player * P = new Player();
        P->name = playerName;
        playerList.push_back(P);
    }

    // activePlayers = new PlayerTurn(playerList);
    activePlayers.playerList = playerList;
    activePlayers.it = activePlayers.playerList.begin();
    activePlayers.currentPlayer = *(activePlayers.it);

    bool test;
    std::cout << "Test or run game with dice rolls? 1: input board index manually, 0: roll dice." << std::endl;
    std::cin >> test;
    if (test) {
        std::cout << "Test mode. Rolling doubles is disabled." << std::endl;
    }
    else {
        std::cout << "Running game, shuffling chance & community chest." << std::endl;
        mixCards(chanceCards);
        mixCards(communityChestCards);
    }

    // bool lateGame;

    bool justVisiting;

    while(activePlayers.playerList.size() !=1 /*temporary variable*/) {
        justVisiting = false;
        curPisBankrupt = false;
        curP = activePlayers.currentPlayer;
        printStartTurn(curP);

        if BOT(curP) {
            // init bot money spending tracker
            botMoneyStart = curP->money;

            if(curP->inJail){
                int roll1 = diceRoll();
                int roll2 = diceRoll();
                // set justVisiting if bot has gotten out of jail
                if(roll1 == roll2){
                    justVisiting = true;
                    curP->inJail = false;
                }
                else if(!(curP->DeckCards.empty())){
                    justVisiting = true;
                    curP->inJail = false;
                    if(curP->DeckCards.begin()->second->type == "CC"){
                        communityChestCards.push_back(curP->DeckCards.begin()->second);
                        curP->DeckCards.erase(curP->DeckCards.begin()->first);
                    }
                    else if(curP->DeckCards.begin()->second->type == "CH"){
                        chanceCards.push_back(curP->DeckCards.begin()->second);
                        curP->DeckCards.erase(curP->DeckCards.begin()->first);
                    }
                }
                else{

                }
            }



            if (!(curP->inJail) && !justVisiting) { // normal (non-jail) bot turn

                if (test) {
                    int totalRoll;
                    std::cout << "Enter board index to move current player to." << std::endl;
                    std::cin >> totalRoll;
                    movePlayer(curP, totalRoll, false);
                    Board[curP->location]->doTileFunction(curP);


                } else {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    int dice1 = diceRoll();
                    std::this_thread::sleep_for(std::chrono::seconds(4));
                    int dice2 = diceRoll();
                    int totalRoll = dice1 + dice2;
                    std::cout << "Dice 1: " << dice1 << " Dice 2: " << dice2 << "\n";
                    movePlayer(curP, curP->location + totalRoll, false);

                    int jailCount;

                    Board[curP->location]->doTileFunction(curP);
                    if (curPisBankrupt) {
                        break;
                    }
                    jailCount = 1;

                    while (dice1 == dice2) {
                        std::cout << "You rolled doubles, rolling again" << std::endl;
                        jailCount = 1;
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                        dice1 = diceRoll();
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        dice2 = diceRoll();
                        totalRoll = dice1 + dice2;
                        std::cout << "Dice 1: " << dice1 << " Dice 2: " << dice2 << "\n";

                        /*implement move player function*/
                        movePlayer(curP, curP->location + totalRoll, false);
                        Board[curP->location]->doTileFunction(curP);
                        jailCount++;


                        if (jailCount == 3) {//send player to jail if they roll doubles 3 times in a row

                            movePlayer(curP, 10, true);
                            std::cout << "You have rolled doubles 3 times in a row! You must be sent to jail\n";
                            break;
                        }
                    }
                }
            }

        // Bot Optional Actions: Unmortgaging properties, then (if no mortgaged properties left) buying houses --------
            if (curPisBankrupt) break;
            else {
                for(auto i: curP->mortCards) {
                    if (trackBotSpending(curP)) {
                        i.second->unmortgage();
                    }
                    else {
                        std::cout << "Spending limit reached while unmortgaging" << std::endl;
                        break;
                    }
                }

                if (curP->mortCards.empty()) {
                    botBuyHouses(curP);
                }
            }

        } // End Bot Turn --------------------------------------------------------------


        // HUMAN PLAYER TURN ------------------------------------------------------------------------------------------
        else{


            if (curP->inJail) { // try to get out of jail AFTER doing all the non-in-jail stuff
                if (inJail(curP)) { // if you got out
                    justVisiting = true;
                }
            }

            if (!(curP->inJail) && !justVisiting) {
                /*implement dice roll function*/

                if (test) {
                    int totalRoll;
                    std::cout << "Enter board index to move current player to." << std::endl;
                    std::cin >> totalRoll;
                    movePlayer(curP, totalRoll, false);
                    Board[curP->location]->doTileFunction(curP);
                } else {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    int dice1 = diceRoll();
                    std::this_thread::sleep_for(std::chrono::seconds(4));
                    int dice2 = diceRoll();
                    int totalRoll = dice1 + dice2;
                    std::cout << "Dice 1: " << dice1 << " Dice 2: " << dice2 << "\n";
                    movePlayer(curP, curP->location + totalRoll, false);

                    int jailCount;

                    Board[curP->location]->doTileFunction(curP);
                    if (curPisBankrupt) {
                        break;
                    }
                    jailCount = 1;

                    while (dice1 == dice2) {
                        std::cout << "You rolled doubles, rolling again" << std::endl;
                        jailCount = 1;
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                        dice1 = diceRoll();
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        dice2 = diceRoll();
                        totalRoll = dice1 + dice2;
                        std::cout << "Dice 1: " << dice1 << " Dice 2: " << dice2 << "\n";

                        /*implement move player function*/
                        movePlayer(curP, curP->location + totalRoll, false);
                        Board[curP->location]->doTileFunction(curP);
                        jailCount++;


                        if (jailCount == 3) {//send player to jail if they roll doubles 3 times in a row

                            movePlayer(curP, 10, true);
                            std::cout << "You have rolled doubles 3 times in a row! You must be sent to jail\n";
                            break;
                        }
                    }
                }


            }

            if (curPisBankrupt) {
                break;
            } else {
                std::cout << "Money: $" << curP->money << std::endl;
                std::cout << "Would you like to unmortgage a property? (y/n)";
                std::cin >> answer;
                std::string unmortID;
                if (answer == 'y') {

                    while (answer == 'y') {
                        displayMortgaged(curP);
                        std::cout << "Which Property would you like to unmortgage " << std::endl;
                        std::cin >> unmortID;

                        if (curP->mortCards.find(unmortID) != curP->mortCards.end()) {
                            curP->mortCards[unmortID]->unmortgage();
                        } else {
                            std::cout << "Card not found" << std::endl;
                        }
                        std::cout << "Would you like to unmortgage another property? (y/n)";
                        std::cin >> answer;
                    }

                }

                std::cout << "Money: $" << curP->money << std::endl;
                std::cout << "Would you like to buy and houses or hotels before the end of your turn? (y/n)";
                std::cin >> answer;

                if (answer == 'y') {

                    while (answer == 'y') {
                        buyHouses(curP);
                        std::cout << "Money: $" << curP->money << std::endl;
                        std::cout << "Would you like to buy more houses or hotels? (y/n)";
                        std::cin >> answer;
                    }

                }
            }

        }
        // END HUMAN PLAYER TURN ---------------------------------------------------------------------------------------

        activePlayers.next_player();

    }


    std::cout<<"Player "<<curP->name<<" Has Won"<<std::endl;
    return 0;
}
