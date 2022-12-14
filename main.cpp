//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"
#include "filepath.h" // ADD THIS FILE TO YOUR WORKING DIRECTORY

Tile * Board[40];
Player * curP;
std::unordered_map<std::string,OwnableCard*> bankCards;

std::deque<DeckCard*> chanceCards;
std::deque<DeckCard*> communityChestCards;
//PlayerTurn * activePlayers;
PlayerTurn activePlayers({});

/*
int main(){
    std::cout<<"This Shit Compiles"<<std::endl;
}
*/

int main() {

    makeBoard(MAKEBOARD_CSV, Board);
    makeDrawCards(MAKECHANCE_TSV, &chanceCards);
    makeDrawCards(MAKECHEST_TSV, &communityChestCards);

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


    while(activePlayers.playerList.size() !=1 /*temporary variable*/) {
        curP = activePlayers.currentPlayer;
        printStartTurn(curP);
        
        if (curP->inJail) {
            std::cout<<"You are in Jail"<<std::endl;
            //STILL NEEDS TO BE IMPLEMENTED!!!
        }
        else {
            /*implement dice roll function*/

            if (test) {
                int totalRoll;
                std::cout << "Enter board index to move current player to." << std::endl;
                std::cin >> totalRoll;
                movePlayer(curP, totalRoll, false);
                Board[curP->location]->doTileFunction(curP);
            }

            else {
                int dice1 = diceRoll();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                int dice2 = diceRoll();
                int totalRoll = dice1 + dice2;
                std::cout << "Dice 1: " << dice1 << " Dice 2: " << dice2 << "\n";
                movePlayer(curP, curP->location + totalRoll, false);

                int jailCount;

                Board[curP->location]->doTileFunction(curP);
                jailCount = 1;

                while (dice1 == dice2) {
                    std::cout << "You rolled doubles, rolling again" << std::endl;
                    jailCount = 1;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    dice1 = diceRoll();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
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
      
        std::cout<<"Money: $"<<curP->money<<std::endl;
        std::cout<<"Would you like to buy and houses or hotels before the end of your turn? (y/n)";
        std::cin>>answer;
        
        if (answer=='y'){
            
            while (answer == 'y'){
                buyHouses(curP);
                 std::cout<<"Money: $"<<curP->money<<std::endl;
                 std::cout<<"Would you like to buy more houses or hotels? (y/n)";
                  std::cin>>answer;
            }
            
        }
        activePlayers.next_player();

    }



    return 0;
}
