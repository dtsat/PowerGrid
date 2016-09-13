#include "stdafx.h"
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "Player.h"
#include "Plants.h"
#include "Cities.h"
#include "Map.h"
#include "Market.h"

// Default constructor
Map::Map()
{
	numCities = -1;
	numAreas = -1;
	market = Market();
}

// Constructor taking vectors of players, cities and plants.
// Initializes player order based on number of players
Map::Map(std::vector<Player> p, std::vector<Cities> c, std::vector<Plants> plants, Market market)
{
	players = p;
	cities = c;
	plantsAvailable = plants;
	for (int i = 0; i < players.size(); i++)
	{
		playerOrder.push_back(i);
	}
	this->market = market;
}

// Copy constructor
Map::Map(const Map& m)
{
	players = m.players;
	cities = m.cities;
	plantsAvailable = m.plantsAvailable;
	playerOrder = m.playerOrder;
	numCities = m.numCities;
	numAreas = m.numAreas;
	adjacencyMatrix = m.adjacencyMatrix;
	areaMatrix = m.areaMatrix;
	validAreas = m.validAreas;
	market = m.market;
	currentPhase = m.currentPhase;
	currentRound = m.currentRound;
	currentStep = m.currentStep;
}

// Destructor
Map::~Map()
{
}

// Given an integer, number of cities, creates an nxn 2-d vector to be used as an adjacency matrix, all values initalized to 0
void Map::createMap(int n)
{
	numCities = n;
	std::vector<int> column;
	for (int i = 0; i < numCities; i++)
	{
		adjacencyMatrix.push_back(column);
		for (int j = 0; j < numCities; j++)
			adjacencyMatrix[i].push_back(0);
	}
}

// Given an integer, number of areas, creates an nxn 2-d vector to be used as an adjacency matrix, all values initalized to 0
void Map::createAreaMap(int n)
{
	numAreas = n;
	std::vector<int> column;
	for (int i = 0; i < numAreas; i++)
	{
		areaMatrix.push_back(column);
		for (int j = 0; j < numAreas; j++)
			areaMatrix[i].push_back(0);
	}
}

// Adds an edge between i and j, with the value in the matrix changing to the cost of the edge (based on save file information)
// Edges with cost of -1 are FREE edges
void Map::addEdge(int i, int j, int cost)
{
	if (i >= 0 && i < numCities && j >= 0 && j < numCities)
	{
		adjacencyMatrix[i][j] = cost;
		adjacencyMatrix[j][i] = cost;
	}
}

// Adds an edge between adjacent areas (based on save file information)
void Map::addAreaEdge(int i, int j)
{
	if (i >= 0 && i < numAreas && j >= 0 && j < numAreas)
	{
		areaMatrix[i][j] = 1;
		areaMatrix[j][i] = 1;
	}
}

// Return a boolean if i and j are adjacent
bool Map::isAdjacent(int i, int j)
{
	if (i >= 0 && i < numCities && j >= 0 && j < numCities)
	{
		if (adjacencyMatrix[i][j] == 0)
			return false;
		else
			return true;
	}
	else
		return false;
}

// Return a boolean if i and j (areas) are adjacent
bool Map::areaIsAdjacent(int i, int j)
{
	if (i >= 0 && i < numAreas && j >= 0 && j < numAreas)
	{
		if (areaMatrix[i][j] == 0)
			return false;
		else
			return true;
	}
	else
		return false;
}

// Return the edge cost of 2 cities
// Returns 0 if not adjacent
// Edge cost is the value stored in the matrix and index [i,j]
int Map::edgeCost(int i, int j)
{
	if (isAdjacent(i, j))
		return adjacencyMatrix[i][j];
	else
		return 0;
}

int Map::getCurrentRound() {
	return currentRound;
}

int Map::getCurrentPhase() {
	return currentPhase;
}

int Map::getCurrentStep()
{
	return currentStep;
}

void Map::setCurrentStep(int i)
{
	currentStep = i;
}

void Map::setCurrentRound(int i) {
	currentRound = i;
}

void Map::setCurrentPhase(int i) {
	currentPhase = i;
}

std::vector<Player> Map::getPlayers() {
	std::vector<Player> tempPlayer = players;
	return tempPlayer;
}

void Map::setPlayerOrder(int i, int j) {
	playerOrder[i] = j;
}

void Map::setPlayerHouses(int i, int x) {
	players[i].setHouses(x);
}

int Map::getPlayerHouses(int i) {
	return players[i].getHouses();
}

bool Map::setPlayerPlant(int i, int p) {
	for (int j = 0; j < plantsAvailable.size(); j++)
		if (plantsAvailable[j].getValue() == p)
			return players[i].addPlant(plantsAvailable[j]);
}

int Map::getCitySlot(int j, int i)
{
	return cities[i].getSlot(j);
}

bool Map::setCitySlot(int j, int p)
{
	for (int i = 0; i < 3; i++) {
		if (cities[j].getSlot(i) == -1) {
			cities[j].setSlot(i, p);
			notify(1);
			return true;
		}
	}
	return false;
}

bool Map::isCityAvailable(int i) {
	return cities[i].isAvailable();
}

Plants Map::getPlant(int i) {
	Plants tempPlant = plantsAvailable[i];
	return tempPlant;
}

Player Map::getPlayer(int i) {
	Player tempPlayer = players[i];
	return tempPlayer;
}

Market Map::getMarket() {
	Market tempMarket = market;
	return tempMarket;
}

void Map::changePlantStatus(int i) {
	for (int j = 0; j < plantsAvailable.size(); j++)
		if (plantsAvailable[j].getValue() == i)
		{
			plantsAvailable[j].changeStatus();
			notify(3);
		}
}

Plants Map::getPlantAtId(int i) {
	Plants tempPlant;
	for (int j = 0; j < plantsAvailable.size(); j++)
		if (plantsAvailable[j].getValue() == i)
			tempPlant = plantsAvailable[j];
	return tempPlant;
}

int Map::getPlantsSize() {
	return plantsAvailable.size();
}

void Map::removeResource(int i, int s, int q) {
	switch (i) {
	case 1:
		market.removeCoal(s, q);
		notify(2);
		break;
	case 2:
		market.removeOil(s, q);
		notify(2);
		break;
	case 3:
		market.removeGarbage(s, q);
		notify(2);
		break;
	case 4:
		market.removeUranium(s, q);
		notify(2);
		break;
	}
}

void Map::substractResources(int p, int r, int q) {
	switch (r) {
	case 1:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].removeCoal(q);
				break;
			}
	case 2:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].removeOil(q);
				break;
			}
	case 3:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].removeGarb(q);
				break;
			}
	case 4:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].removeUran(q);
				break;
			}
	}
}

void Map::removePlayerPlant(int p, int s) {
	players[p].removePlant(s);
}

void Map::setPlayerPower(int i, int p) {
	players[p].setCitiesPowered(i);
}

void Map::addResource(int r, int q) {
	switch (r) {
	case 1:
		market.addCoal(q);
		break;
	case 2:
		market.addOil(q);
		break;
	case 3:
		market.addGarbage(q);
		break;
	case 4:
		market.addUranium(q);
		break;
	}
}

void Map::addBankResource(int r, int q) {
	switch (r) {
	case 1:
		market.setBankResource(0, market.getBankResource(0) + q);
		break;
	case 2:
		market.setBankResource(1, market.getBankResource(1) + q);
		break;
	case 3:
		market.setBankResource(2, market.getBankResource(2) + q);
		break;
	case 4:
		market.setBankResource(3, market.getBankResource(3) + q);
		break;
	}
}

void Map::addPlantResource(int i, int p, int r, int q) {
	players[i].addPlantResource(p, r, q);
}

void Map::removePlantResource(int i, int p, int r, int q) {
	players[i].removePlantResource(p, r, q);
}

void Map::setResources(int p, int r, int q) {
	switch (r) {
	case 1:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].addCoal(q);
				break;
			}
		break;
	case 2:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].addOil(q);
				break;
			}
		break;
	case 3:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].addGarb(q);
				break;
			}
		break;
	case 4:
		for (int i = 0; i < plantsAvailable.size(); i++)
			if (plantsAvailable[i].getValue() == p) {
				plantsAvailable[i].addUran(q);
				break;
			}
		break;
	}
}

// Returns the size of the player, cities, plants, playable arears and all areas in the map
int Map::playersSize()
{
	return players.size();
}

int Map::citiesSize()
{
	return cities.size();
}

int Map::plantsSize()
{
	return plantsAvailable.size();
}

int Map::areaSize()
{
	return validAreas.size();
}

int Map::getAreas()
{
	return numAreas;
}

// The next few functions are accessors and mutators for players
// The map holds a vector of players, and given an index, the appropraite accessor/mutator from the player class is called for that index
std::string Map::getPlayerName(int i)
{
	if (i < 0 || i > players.size())
		return "";
	return players[i].getName();
}

int Map::getPlayerElektros(int i)
{
	if (i < 0 || i > players.size())
		return -1;
	return players[i].getElektros();
}

int Map::getPlayerScore(int i)
{
	if (i < 0 || i > players.size())
		return -1;
	return players[i].getScore();
}

int Map::getPlayerNumPlants(int i)
{
	if (i < 0 || i > players.size())
		return -1;
	return players[i].getNumPlants();
}

int Map::getPlayerPlayerNum(int i)
{
	if (i < 0 || i > players.size())
		return -1;
	return players[i].getPlayerNum();
}

std::string Map::getPlayerColour(int i)
{
	if (i < 0 || i > players.size())
		return "";
	return players[i].getColour();
}

int Map::getPlayerHousesLeft(int i)
{
	if (i < 0 || i > players.size())
		return -1;
	return players[i].getHousesLeft();
}

void Map::setPlayerElektros(int i, int e)
{
	if (i < 0 || i > players.size());
	{
		players[i].setElektros(e);
		notify(0);
	}
}

int Map::modPlayerElektros(int i, int e)
{
	if (i < 0 || i > players.size())
		return -1;
	return players[i].modElektros(e);
}

void Map::increasePlayerScore(int i)
{
	if (i < 0 || i > players.size());

	players[i].increaseScore();
}

// Shuffles the order of the player order vector
void Map::shuffleOrder()
{
	random_shuffle(playerOrder.begin(), playerOrder.end());
}

void Map::shufflePlants() {
	random_shuffle(plantsAvailable.begin(), plantsAvailable.end());
}

// Return player order at index i
// For example, if index 0 is player 2, that means player 2 goes first.
int Map::getPlayerOrder(int i)
{
	return playerOrder[i];
}

// Sets a city as platable, meaning it is in a valid area to be used
void Map::setCityPlayable(int i, bool p)
{
	cities[i].setPlayable(p);
}

// Stores the valid areas that will be used in this particular game
void Map::setPlayableAreas(int i)
{
	validAreas.push_back(i);
}

// Return the playable area at index i in the playable area vector
int Map::getPlayableAreas(int i)
{
	return validAreas[i];
}

// Cycle through all cities, return those that are currently playable, and also list the cities adjacent to this city, with the edge costs
void Map::showAdjacency()
{
	using namespace std;
	cout << "Each city in a playable area and their direct adjacency will be printed below: " << endl;
	cout << "------------------------------------------------------------" << endl;
	for (int i = 0; i < cities.size(); i++)
	{
		if (cities[i].getPlayable())
		{
			cout << cities[i].getName() << ": ";
			for (int j = 0; j < cities.size(); j++)
			{
				if (isAdjacent(i, j) && cities[j].getPlayable() == true)
				{
					int cost = edgeCost(i, j);
					if (cost < 0)
						cost = 0;
					cout << cities[j].getName() << " (cost: " << cost << "), ";
				}
			}
			cout << endl;
			cout << endl;
		}
	}
}

// Show areas adjacent to area i
void Map::showAreaAdjacency(int i)
{
	using namespace std;
	cout << "Adjacent to " << i << ": ";
	for (int j = 0; j < numAreas; j++)
	{
		if (areaIsAdjacent(i, j))
			cout << j << ", ";
	}
	cout << endl;
}

// The next few functions are accessor/mutators for the city objects
// The map holds a vector of cities, and given an index, these functions call the appropraite functions from the city class for that index
std::string Map::getCityName(int i)
{
	return cities[i].getName();
}

int Map::getCityArea(int i)
{
	return cities[i].getArea();
}

bool Map::getCityPlayable(int i)
{
	return cities[i].getPlayable();
}

int Map::getCityNum(int i)
{
	return cities[i].getCityNum();
}

void Map::setCitySlot(int i, int s, int p)
{
	cities[i].setSlot(s, p);
}

void Map::saveGame()
{
	using namespace std;
	ofstream fileWrite("player.txt");
	if (fileWrite.is_open())
	{
		fileWrite << playersSize() << "\n";
		for (int i = 0; i < playersSize(); i++)
		{
			fileWrite << players[i].getPlayerNum() << " ";
			fileWrite << players[i].getName() << " ";
			fileWrite << players[i].getColour() << " ";
			fileWrite << players[i].getElektros() << " ";
			fileWrite << players[i].getScore() << " ";
			fileWrite << players[i].getHousesLeft() << " \t";
			fileWrite << players[i].getNumPlants() << " ";
			for (int j = 0; j < 3; j++)
			{
				Plants temp = players[i].getPlant(j);
				if (temp.getValue() > 0)
					fileWrite << temp.getPlantID() << " ";
				else
					fileWrite << "-1 ";
			}
			fileWrite << "\n";
		}
	}
	else
		cout << "ERROR, CANNOT OPEN FILE" << endl;
	fileWrite.close();
	ofstream fileWrite2("plant.txt");
	if (fileWrite2.is_open())
	{
		fileWrite2 << plantsSize() << "\n";
		for (int i = 0; i < plantsSize(); i++)
		{
			fileWrite2 << plantsAvailable[i].getPlantID() << "\t";
			fileWrite2 << plantsAvailable[i].getValue() << " ";
			fileWrite2 << plantsAvailable[i].getCitiesPowered() << "\t";
			fileWrite2 << plantsAvailable[i].getCoalCost() << " ";
			fileWrite2 << plantsAvailable[i].getOilCost() << " ";
			fileWrite2 << plantsAvailable[i].getGarbCost() << " ";
			fileWrite2 << plantsAvailable[i].getUranCost() << "\t\t";
			fileWrite2 << plantsAvailable[i].getCoalHeld() << " ";
			fileWrite2 << plantsAvailable[i].getOilHeld() << " ";
			fileWrite2 << plantsAvailable[i].getGarbHeld() << " ";
			fileWrite2 << plantsAvailable[i].getUranHeld() << "\t\t";
			fileWrite2 << plantsAvailable[i].getStatus() << "\n";
		}
	}
	else
		cout << "ERROR, CANNOT OPEN FILE" << endl;
	fileWrite2.close();
	ofstream fileWrite3("cities.txt");
	if (fileWrite3.is_open())
	{
		fileWrite3 << currentStep << "\n";
		fileWrite3 << citiesSize() << "\n";
		for (int i = 0; i < citiesSize(); i++)
		{
			fileWrite3 << cities[i].getName() << "\n";
			fileWrite3 << cities[i].getArea() << " ";
			fileWrite3 << cities[i].getCityNum() << " ";
			fileWrite3 << cities[i].getSlot(0) << " ";
			fileWrite3 << cities[i].getSlot(1) << " ";
			fileWrite3 << cities[i].getSlot(2) << "\n";
		}
		fileWrite3 << "\n";
		fileWrite3 << "AREAS\n";
		fileWrite3 << getAreas() << "\n";
		for (int j = 0; j < getAreas(); j++)
		{
			for (int k = 0; k < getAreas(); k++)
			{
				if (areaIsAdjacent(j, k))
					fileWrite3 << "1\t";
				else
					fileWrite3 << "0\t";
			}
			fileWrite3 << "\n";
		}
		fileWrite3 << "\n";
		fileWrite3 << "VALID\n";
		fileWrite3 << areaSize() << "\n";
		for (int i = 0; i < areaSize(); i++)
		{
			fileWrite3 << validAreas[i] << " ";
		}
		fileWrite3 << "\n\n";
		fileWrite3 << "MATRIX\n";
		for (int j = 0; j < citiesSize(); j++)
		{
			for (int k = 0; k < citiesSize(); k++)
			{
				if (isAdjacent(j, k))
					fileWrite3 << edgeCost(j, k) << "\t";
				else
					fileWrite3 << "0\t";
			}
			fileWrite3 << "\n";
		}
	}
	else
		cout << "ERROR, CANNOT OPEN FILE" << endl;
	fileWrite3.close();
	ofstream fileWrite4("market.txt");
	if (fileWrite4.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			fileWrite4 << market.getBankResource(i) << "\n";
		}
		fileWrite4 << "\n";
		for (int i = 0; i < 9; i++)
			fileWrite4 << market.getCoal(i) << " ";
		fileWrite4 << "\n";
		for (int i = 0; i < 9; i++)
			fileWrite4 << market.getOil(i) << " ";
		fileWrite4 << "\n";
		for (int i = 0; i < 9; i++)
			fileWrite4 << market.getGarbage(i) << " ";
		fileWrite4 << "\n";
		for (int i = 0; i < 17; i++)
			fileWrite4 << market.getUranium(i) << " ";
		fileWrite4 << "\n";
	}
	else
		cout << "ERROR, CANNOT OPEN FILE" << endl;
	fileWrite4.close();
}

bool Map::isCityOwned(int i, int p)
{
	return cities[i].ownedBy(p);
}