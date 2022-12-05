#include <vector>
#include <fstream>
#include <string>
#include "classes.h"
#include <sstream>

// read property tiles
// MUST have global Board variable before calling this function!
// csv MUST be sorted by tile number
void makeBoard(std::string filename, Tile ** Board) {
    std::fstream fin;
    fin.open(filename);

    std::vector<std::string> row;
    std::string line, word;

    for (int i=0; i<40; i++) {
        row.clear();
        getline(fin, line);
        std::stringstream s(line);
        while (getline(s, word, ',')) {
            row.push_back(word);
        }

        switch (stoi(row[0])) {
            case 'P': {
                PropertyTile *tempPropTile;
                //row[1] = name
                //nullptr = linked card
                tempPropTile = new PropertyTile(row[1], nullptr);
                int *a = new int[4]{stoi(row[6]), stoi(row[7]), stoi(row[8]), stoi(row[9])};

                PropertyCard * tempPropCard = new PropertyCard(row[3],  // color
                                                              row[1],  // name
                                                              row[2],  // cardID
                                                              stoi(row[4]),  // saleValue int
                                                              stoi(row[5]),  // baseRent int
                                                              a,       // houseRent[4]
                                                              stoi(row[10]), // hotelRent
                                                              stoi(row[11]), // mort
                                                              stoi(row[12]), // houseCost
                                                              stoi(row[13]), // hotelCost
                                                              tempPropTile// linkTitle
                );
                tempPropTile->linkedCard = tempPropCard;
                Board[i] = dynamic_cast<Tile*>(tempPropTile);
                break;
            }
            default: {
                std::cout << i << " " << row[0] << std::endl;
                break;
            }
        }
    }
}

int main(void) {
    Tile * Board[40];
    makeBoard("makeboard.csv", Board);
    std::cout << dynamic_cast<PropertyTile*>(Board[1])->linkedCard->cardID << std::endl;
    std::cout << dynamic_cast<PropertyTile*>(Board[39])->linkedCard->saleValue << std::endl;

    return 0;
}

