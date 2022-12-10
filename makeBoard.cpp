#include <vector>
#include <fstream>
#include <string>
#include "classes.h"
#include <sstream>

#define OTHER (1)
#define PROPERTY (2)
#define RAILROAD (3)
#define TAX (4)
#define DRAWCARD (5)
#define UTILITY (6)

#define COMMUNITY_CHEST (1)
#define CHANCE (2)

extern std::unordered_map<std::string,OwnableCard*> bankCards;

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

        //std::cout << std::stoi(row[0]) << std::endl;

        switch (std::stoi(row[0])) {
            case PROPERTY: {
                PropertyTile *tempPropTile;
                //row[1] = name
                //nullptr = linked card
                tempPropTile = new PropertyTile(row[1], nullptr);
                int *a = new int[4]{std::stoi(row[6]), std::stoi(row[7]), std::stoi(row[8]), std::stoi(row[9])};

                PropertyCard * tempPropCard = new PropertyCard(row[3],  // color
                                                              row[1],  // name
                                                              row[2],  // cardID
                                                              std::stoi(row[4]),  // saleValue int
                                                              std::stoi(row[5]),  // baseRent int
                                                              a,       // houseRent[4]
                                                              std::stoi(row[10]), // hotelRent
                                                              std::stoi(row[11]), // mort
                                                              std::stoi(row[12]), // houseCost
                                                              std::stoi(row[13]), // hotelCost
                                                              tempPropTile// linkTitle
                );

                bankCards.insert(std::make_pair(row[2], tempPropCard));

                tempPropTile->linkedCard = tempPropCard;
                Board[i] = dynamic_cast<Tile*>(tempPropTile);
                break;
            }

            case RAILROAD: {
                RailRoadTile * tempRRTile = new RailRoadTile(row[1], nullptr);
                RailRoadCard * tempRRCard = new RailRoadCard(row[1], row[2], tempRRTile);
                bankCards.insert(std::make_pair(row[2], tempRRCard));
                tempRRTile->linkedCard = tempRRCard;
                Board[i] = dynamic_cast<Tile*>(tempRRTile);
                break;
            }

            case TAX: {
                TaxTile * tempTTile = new TaxTile(row[1], std::stoi(row[2]));
                Board[i] = dynamic_cast<Tile*>(tempTTile);
                break;
            }

            // DRAWCARD case
            // OTHER case

            case UTILITY: {
                UtilityTile * tempUTile = new UtilityTile(row[1], nullptr);
                UtilityCard * tempUCard = new UtilityCard(  row[1], // name
                                                            row[2], // ID
                                                            stoi(row[3]), // sale
                                                            stoi(row[4]), // mort
                                                            stoi(row[5]), // rent1
                                                            stoi(row[6]), // rent2
                                                            tempUTile);       // linkedTile
                tempUTile->linkedCard = tempUCard;
                bankCards.insert(std::make_pair(row[2], tempUCard));
                Board[i] = dynamic_cast<Tile*>(tempUTile);
                break;                                      
            }

            default: {
                // std::cout << i << " " << row[1] << std::endl; // for testing as functionality is added later
                break;
            }
        } 
    }
}

