#ifndef SAVE_H
#define SAVE_H
#include "SaveLoadBuilder.h"

// Concrete implementation of the SaveLoadBuilder
// The Builder object created is used to Save the game
// This object is special in it's implementation in that no object is actually created
// The object passed to the Builder is used to save the game. The creation functions in this class are not used

class SaveBuilder : public SaveLoadBuilder
{
public:
	SaveBuilder();
	~SaveBuilder();
	void createMap(Map m);
	std::vector<Player> setPlayers();
	std::vector<Cities> setCities();
	std::vector<Plants> setPlants();
	Market setMarket();
};

#endif

