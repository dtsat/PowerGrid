#ifndef MAP_H
#define MAP_H
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include "Player.h"
#include "Plants.h"
#include "Cities.h"
#include "Market.h"
#include "Observable.h"

// This class contains most of the functions used by the game.
// The map class contains vectors of players, cities and plants, and thus all accessor/mutator functions in these classes are called by functions in this class.
// The map is able to check fo adjacency between cities and areas using the adjacency matrices it has as attributes.
// The map class is able to access any of the attributes of players, cities and plants (plants not yet implemented)


class Map : public Observable
{
private:
	std::vector<Player> players;
	std::vector<Cities> cities;
	std::vector<Plants> plantsAvailable;
	int numCities;
	std::vector<std::vector<int>> adjacencyMatrix;
	std::vector<std::vector<int>> areaMatrix;
	int numAreas;
	std::vector<int> playerOrder;
	std::vector<int> validAreas;
	Market market;

	int currentPhase, currentRound, currentStep;
public:
	Map();
	Map(std::vector<Player> p, std::vector<Cities> c, std::vector<Plants> plants, Market market);
	Map(const Map& m);
	~Map();

	void createMap(int n);
	void createAreaMap(int n);
	void addEdge(int i, int j, int cost);
	void addAreaEdge(int i, int j);
	bool isAdjacent(int i, int j);
	bool areaIsAdjacent(int i, int j);
	int edgeCost(int i, int j);

	void setPlayerPower(int i, int p);

	int getCurrentRound();
	int getCurrentPhase();
	int getCurrentStep();
	void setCurrentStep(int i);
	void setCurrentRound(int i);
	void setCurrentPhase(int i);
	std::vector<Player> getPlayers();
	void setPlayerOrder(int i, int j);
	void setPlayerHouses(int i, int x);
	bool setPlayerPlant(int i, int p);
	int getPlayerHouses(int i);
	bool setCitySlot(int j, int i);
	int getCitySlot(int j, int p);
	bool isCityAvailable(int i);
	Plants getPlant(int i);
	Player getPlayer(int i);
	Market getMarket();
	void changePlantStatus(int i);
	Plants getPlantAtId(int i);
	int getPlantsSize();
	void removeResource(int i, int s, int q);
	void substractResources(int p, int r, int q);
	void removePlayerPlant(int p, int s);
	void addResource(int r, int q);
	void addBankResource(int r, int q);
	void addPlantResource(int i, int p, int r, int q);
	void removePlantResource(int i, int p, int r, int q);
	void setResources(int p, int r, int q);

	int playersSize();
	int citiesSize();
	int plantsSize();
	int areaSize();
	int getAreas();

	std::string getPlayerName(int i);
	int getPlayerElektros(int i);
	int getPlayerScore(int i);
	int getPlayerNumPlants(int i);
	int getPlayerPlayerNum(int i);
	std::string getPlayerColour(int i);
	int getPlayerHousesLeft(int i);
	void setPlayerElektros(int i, int e);
	int modPlayerElektros(int i, int e);
	void increasePlayerScore(int i);
	void shuffleOrder();
	void shufflePlants();
	int getPlayerOrder(int i);

	void setCityPlayable(int i, bool p);
	void setPlayableAreas(int i);
	int getPlayableAreas(int i);

	void showAdjacency();
	void showAreaAdjacency(int i);

	std::string getCityName(int i);
	int getCityArea(int i);
	bool getCityPlayable(int i);
	int getCityNum(int i);
	void setCitySlot(int i, int s, int p);
	bool isCityOwned(int i, int p);

	void saveGame();
};

#endif