#ifndef BUILDER_H
#define BUILDER_H
#include "Map.h"

// This abstract class is used to implement the Builder pattern to create objects that can Save/Load/Start new games
// The class of objects created create Map objeccts (except for Save, which only saves the current map)
// Builder objects store a Map, and set the value of this Map using the createMap() function
// Since a Map object contains many components, each component is created using the helper functions for creating Players, Cities, Plants and Markets
// The Builder can return the created Map to the Director when prompted to do so

class SaveLoadBuilder
{
protected:
	Map map;
public:
	SaveLoadBuilder();
	virtual ~SaveLoadBuilder();
	Map getMap();
	virtual void createMap(Map m) = 0;
	virtual std::vector<Player> setPlayers() = 0;
	virtual std::vector<Cities> setCities() = 0;
	virtual std::vector<Plants> setPlants() = 0;
	virtual Market setMarket() = 0;
};

#endif