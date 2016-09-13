#ifndef LOAD_H
#define LOAD_H
#include "SaveLoadBuilder.h"

// Concrete implementation of the SaveLoadBuilder
// The Builder object created is used to create a game from a Load file
// The Builder builds all the components for a Map using the saved previous game load files
// The created Map is stored in the Builder and can be sent to the Director when prompted

class LoadBuilder : public SaveLoadBuilder
{
public:
	LoadBuilder();
	~LoadBuilder();
	void createMap(Map m);
	std::vector<Player> setPlayers();
	std::vector<Cities> setCities();
	std::vector<Plants> setPlants();
	Market setMarket();
};

#endif

