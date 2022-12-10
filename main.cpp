//
// Created by mdeno on 12/7/2022.
//

#include "functions.h"

Tile * Board[40];
Player * curP;
std::unordered_map<std::string,OwnableCard*> bankCards;

std::deque<DeckCard*> chanceCards;
std::deque<DeckCard*> communityChestCards;

/*
int main(){
    std::cout<<"This Shit Compiles"<<std::endl;
}
*/

int main() {
    // Tile * Board[40];
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
        int jailCount;
        int dice1=diceRoll();
        sleep_for(2s);
        int dice2=diceRoll();
        int totalRoll=dice1+dice2;
        std::cout<<"Dice 1: "<<dice1<<" Dice 2: "<<dice2<<"\n";
        movePlayer(curP,curp->location + totalRoll,false);
        Board[curP->location]->doCardFunction();
        jailCount=1;
        
        while(dice1==dice2){
            sleep_for(1s);
            dice1=diceRoll();
            sleep_for(1s);
            dice2=diceRoll();
            totalRoll=dice1+dice2;
            std::cout<<"Dice 1: "<<dice1<<" Dice 2: "<<dice2<<"\n";
   
             /*implement move player function*/
            movePlayer(curP,curp->location + totalRoll,false);
            Board[curP->location]->doCardFunction();
            jailCount++;
            

            if(jailCount==3){//send player to jail if they roll doubles 3 times in a row
                
                movePlayer(curP,10,True);
                std::cout<<"You have rolled doubles 3 times in a row! You must be sent to jail\n";
                break;
            }
        }
        
       

      

        std::cout<<"Would you like to buy and houses or hotels before the end of your turn? Type 1 for yes and 0 for no";
        std::cin>>answer;
        if (answer==true){
            //implement buy house or hotel
        }
        activePlayers->next_player();

    }



    return 0;
}
