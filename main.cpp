//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"

PlayerTurn * activePlayers;
Tile * Board[40];

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

    Player * curP;


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
        //End Turn, switch to next player as currentPlayer

    }



    return 0;
}