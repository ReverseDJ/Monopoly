//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"

Tile * Board[40];

Player * curP;

/*
int main(){
    std::cout<<"This Shit Compiles"<<std::endl;
}
*/

int main() {
    Tile * Board[40];
    std::cout<<"Welcome to Monopoly!\n";
    int players;
    std::cout<<"How many players?\n";
    std::cin>>players;
    bool answer;
    
    std::list<Player*> playerList;
    
    for (int i = 1; i <= players; i++){    
        std::string playerName;
        std::cout << "\nEnter player " << i << " name: ";
        std::cin >> playerName;
        Player * P = new Player();
        P->name = playerName;
        playerList.push_back(P);
    }
        
    PlayerTurn * activePlayers = new PlayerTurn(playerList);


    while(activePlayers->playerList.size() !=1 /*temporary variable*/) {
        curP = activePlayers->currentPlayer;
        displayProperties(curP);
        if (curP->inJail) {
            //implement in jail function
            ;
        }
        /*implement dice roll function*/
        
        /*implement move player function*/

        Board[curP->location]->doCardFunction();

        std::cout<<"Would you like to buy and houses or hotels before the end of your turn? Type 1 for yes and 0 for no";
        std::cin>>answer;
        if (answer==true){
            //implement buy house or hotel
        }
        activePlayers->next_player();

    }



    return 0;
}
