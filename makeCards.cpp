
#include "functions.h"


#define COMMUNITY_CHEST (1)
#define CHANCE (2)

extern std::deque<DeckCard*> chanceCards;
extern std::deque<DeckCard*> communityChestCards;

// read in chance, community chest from csv file 
// uses a double-ended queue (deque) to pop, push, and swap in O(1)
// add shuffle function later

#define GET_MONEY (2)
#define TRANSFER_MONEY (3)
#define GOOJ (4)
#define MOVE (1)
#define PAY_BUILDING (5)

void makeDrawCards(std::string filename, std::deque<DeckCard*> * deck) {
    
    std::fstream fin;
    fin.open(filename);

    std::vector<std::string> row;

    char templine[170];
    char temp[160];

    //for (int i=0; i<2; i++) {
    while (fin.getline(templine, 170, '\n')) {
        row.clear();
        //fin.getline(templine, 170, '\n');
        std::string line(templine);
        std::stringstream s(line);

        while (s.getline(temp, 160, '\t')) {
            std::string stemp(temp);
            row.push_back(stemp);
        }

        switch (stoi(row[0])) {
            
            case GET_MONEY: {
                // std::cout << row[2] << std::endl; //debug
                getMoneyCard * tempCard = new getMoneyCard( std::stoi(row[2]), // $amount
                                                            row[1],
                                                            "",
                                                            "" );
                deck->push_back(tempCard);
                break;
            }
            
            case TRANSFER_MONEY: {
                break;
            }

            case GOOJ: {
                break;
            }

            case MOVE: {
                break;
            }

            case PAY_BUILDING: {
                break;
            }

            default:
                std::cout << "Bad card type" << std::endl;
                break;
        }

    }
}
