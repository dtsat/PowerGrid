#ifndef NEW_H
#define NEW_H
#include "SaveLoadBuilder.h"
#include "Map.h"

// Concrete implementation of the SaveLoadBuilder
// The Builder object created is used to create a New game
// The Builder builds all the components for a Map using the special game initialization load files
// The created Map is stored in the Builder and can be sent to the Director when prompted

class NewGameBuilder : public SaveLoadBuilder
{
public:
	NewGameBuilder();
	~NewGameBuilder();
	void createMap(Map m);
	std::vector<Player> setPlayers();
	std::vector<Cities> setCities();
	std::vector<Plants> setPlants();
	Market setMarket();
};

#endif