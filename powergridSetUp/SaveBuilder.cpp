#include "stdafx.h"
#include "SaveBuilder.h"

// Constructor
SaveBuilder::SaveBuilder()
{
}

// Destructor
SaveBuilder::~SaveBuilder()
{
}

// Take a map, call the saveGame() function from map class
void SaveBuilder::createMap(Map m)
{
	map = m;
	m.saveGame();
}

// These are all dummy functions, only used to be able to override the virtual functions
std::vector<Player> SaveBuilder::setPlayers()
{
	std::vector<Player> p;
	return p;
}

std::vector<Cities> SaveBuilder::setCities()
{
	std::vector<Cities> c;
	return c;
}

std::vector<Plants> SaveBuilder::setPlants()
{
	std::vector<Plants> p;
	return p;
}

Market SaveBuilder::setMarket()
{
	Market m;
	return m;
}