#ifndef DIRECTOR_H
#define DIRECTOR_H
#include "SaveLoadBuilder.h"
#include "Map.h"

// This class is the Director object used in the Builder pattern
// The Director is used to create SaveLoad objects which can either start a New game, Load a previous game or Save a current game
// The Director sets the type of Builder depending on the need and uses it to perform the appropriate functions

class SaveLoadDirector
{
private:
	SaveLoadBuilder* builder;
public:
	SaveLoadDirector();
	~SaveLoadDirector();
	void setBuilder(SaveLoadBuilder* b);
	Map getMap();
	void buildMap(Map m);
};

#endif