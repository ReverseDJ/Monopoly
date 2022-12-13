
#include "classes.h"

#define CORNER (1)
#define PROPERTY (2)
#define RAILROAD (3)
#define TAX (4)
#define DRAWCARD (5)
#define UTILITY (6)
#define GOTOJAIL (7)

#define COMMUNITY_CHEST (1)
#define CHANCE (2)

extern std::unordered_map<std::string,OwnableCard*> bankCards;
extern std::deque<DeckCard*> chanceCards;
extern std::deque<DeckCard*> communityChestCards;

// read property tiles
// MUST have global Board variable before calling this function!
// csv MUST be sorted by tile number
void makeBoard(std::string filename, Tile ** Board) {
    
    std::fstream fin;
    fin.open(filename);

    std::vector<std::string> row;

    // make the chance, community chest tiles
    DrawCardTile * tempChance = new DrawCardTile("Chance", &chanceCards);
    DrawCardTile * tempChest = new DrawCardTile("Community Chest", &communityChestCards);

    char templine[80];
    char temp[30];

    for (int i=0; i<40; i++) {
        row.clear();
        fin.getline(templine, 80, '\n');
        std::string line(templine);
        std::stringstream s(line);

        while (s.getline(temp, 30, ',')) {
            std::string stemp(temp);
            row.push_back(temp);
        }

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

            case DRAWCARD: {
                // making 1 chance card, 1 community chest card, and pointing multiple tile locations to them
                if (stoi(row[1]) == CHANCE) {
                    Board[i] = dynamic_cast<Tile*>(tempChance);
                }
                else if (std::stoi(row[1]) == COMMUNITY_CHEST) {
                    Board[i] = dynamic_cast<Tile*>(tempChest);
                }
                else {
                    std::cout << "Bad chance/community chest input at " << i << std::endl;
                }
                break;
            }

            // OTHER case
            case CORNER: {
                CornerTile * tempCornerTile = new CornerTile(row[1]);
                Board[i] = dynamic_cast<Tile*>(tempCornerTile);
                break;
            }

            case GOTOJAIL: {
                GoToJailTile * tempGTJTile = new GoToJailTile(row[1], i);
                Board[i] = dynamic_cast<GoToJailTile*>(tempGTJTile);
                break;
            }

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
                std::cout << "Bad tile type in input at " << i << std::endl;
                break;
            }
        } 
    }
}

