#include "stdafx.h"
#include "LoadBuilder.h"
#include <fstream>

// Constructor
LoadBuilder::LoadBuilder()
{
}

// Destructor
LoadBuilder::~LoadBuilder()
{
}

// Creates a Map object using the parameters detailed in a saved Load Game file
// Map object pass as argument is not used, only for being able to override the function
// Object is created from saved game load files for new games
void LoadBuilder::createMap(Map m)
{
	using namespace std;
	cout << "You have chosen to load a previously saved game." << endl;
	cout << "All game settings, including Player, City, Plant and Resource settings, will be loaded from a previous state.\n" << endl;
	system("pause");
	// create player, cities, plants and market
	std::vector<Player> players = setPlayers();
	std::vector<Cities> cities = setCities();
	std::vector<Plants> plants = setPlants();
	Market market = setMarket();

	Map gameMap(players, cities, plants, market);
	ifstream inputFile;
	inputFile.open("map_Saved.txt");
	int numCities, step;
	inputFile >> step;
	inputFile >> numCities;
	gameMap.createMap(numCities);
	gameMap.setCurrentStep(step);
	string matrix = "";
	while (matrix.compare("AREAS") != 0)
	{
		inputFile >> matrix;
	}
	int numAreas, value;
	inputFile >> numAreas;
	gameMap.createAreaMap(numAreas);
	for (int i = 0; i < numAreas; i++)
	{
		for (int j = 0; j < numAreas; j++)
		{
			inputFile >> value;
			if (value == 1)
				gameMap.addAreaEdge(i, j);
		}
	}
	inputFile.close();
	// City adjacency matrix setup
	inputFile.open("map_Saved.txt");
	while (matrix.compare("MATRIX") != 0)
	{
		inputFile >> matrix;
	}
	for (int i = 0; i < numCities; i++)
	{
		for (int j = 0; j < numCities; j++)
		{
			inputFile >> value;
			gameMap.addEdge(i, j, value);
		}
	}
	inputFile.close();
	inputFile.open("map_Saved.txt");
	while (matrix.compare("VALID") != 0)
	{
		inputFile >> matrix;
	}
	int validAreas, area;
	inputFile >> validAreas;
	for (int i = 0; i < validAreas; i++)
	{
		inputFile >> area;
		gameMap.setPlayableAreas(area);
	}
	inputFile.close();
	cout << "The playable areas during the most recent saved game were: ";
	for (int i = 0; i < gameMap.areaSize(); i++)
	{
		cout << gameMap.getPlayableAreas(i) << " ";
	}
	cout << "\nAll other areas will not be playable in this game.\n" << endl;
	for (int i = 0; i < cities.size(); i++)
	{
		for (int j = 0; j < gameMap.areaSize(); j++)
		{
			if (gameMap.getPlayableAreas(j) == cities[i].getArea())
				gameMap.setCityPlayable(i, true);
		}
	}
	system("pause");
	std::cout << std::endl;
	// set up plant ownership
	inputFile.open("player_Saved.txt");
	int numPlayers;
	string name, colour;
	int num, elektros, score, houses, plantsNum, plantID;
	inputFile >> numPlayers;
	for (int i = 0; i < numPlayers; i++)
	{
		inputFile >> num;
		inputFile >> name;
		inputFile >> colour;
		inputFile >> elektros;
		inputFile >> score;
		inputFile >> houses;
		inputFile >> plantsNum;
		inputFile >> plantID;
		if (plantID > -1)
			gameMap.setPlayerPlant(i, plants[plantID].getValue());
		inputFile >> plantID;
		if (plantID > -1)
			gameMap.setPlayerPlant(i, plants[plantID].getValue());
		inputFile >> plantID;
		if (plantID > -1)
			gameMap.setPlayerPlant(i, plants[plantID].getValue());
	}
	inputFile.close();
	// display stats at current state of game
	cout << "The stats of each player when the game was last saved will be displayed below: " << endl;
	for (int i = 0; i < players.size(); i++)
	{
		cout << "-------------------------------------------------------------------" << endl;
		cout << "Player " << players[i].getPlayerNum() + 1 << ": " << players[i].getColour() << " (" << players[i].getName() << ")" << endl;
		cout << "Number of houses currently owned: " << players[i].getScore() << endl;
		cout << "Number of houses remaining: " << players[i].getHousesLeft() << endl;
		cout << "Number of plants currently owned: " << gameMap.getPlayerNumPlants(i) << endl;
		cout << "Elektros: " << players[i].getElektros() << endl;
		cout << "Cities owned by " << players[i].getPlayerNum() + 1 << ": ";
		for (int j = 0; j < cities.size(); j++)
		{
			if (cities[j].getSlot(0) == players[i].getPlayerNum())
				cout << cities[j].getName() << ", ";
		}
		cout << endl;
		cout << "-------------------------------------------------------------------" << endl;
		system("pause");
	}
	cout << "\n\nPower plants have been loaded from the file based on most recently saved game." << endl;
	cout << "The initial starting power plant market is as follows: " << endl;
	system("pause");
	cout << "-------------------------------------------------------------------" << endl;
	cout << "Available for auction:" << endl;
	int printed = 0;
	for (int i = 0; i < plants.size(); i++)
	{
		if (plants[i].getStatus() == 0)
		{
			cout << endl;
			cout << i << ": Minimum Bid " << plants[i].getValue() << endl;
			cout << "Houses Powered: " << plants[i].getCitiesPowered() << endl;
			cout << "Resource cost: " << plants[i].getCoalCost() << " Coal, " << plants[i].getOilCost() << " Oil, " << plants[i].getGarbCost() << " Garbage, " << plants[i].getUranCost() << " Uranium" << endl;
			cout << "Resource max storage: " << plants[i].getCoalMax() << " Coal, " << plants[i].getOilMax() << " Oil, " << plants[i].getGarbMax() << " Garbage, " << plants[i].getUranMax() << " Uranium" << endl;
			printed++;
			if (printed == 4)
			{
				cout << "-------------------------------------------------------------------" << endl;
				system("pause");
				cout << "\nNot yet available for auction:" << endl;
			}
			if (printed == 8)
				break;
		}
	}
	cout << "-------------------------------------------------------------------" << endl;
	system("pause");
	cout << "\nWe will display the playable map information below." << endl;
	system("pause");
	gameMap.showAdjacency();
	system("pause");
	cout << "The resource market holds the following resources in the bank: " << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "Bank: [Coal: " << market.getBankResource(0) << ", Oil: " << market.getBankResource(1) << ", Garbage: " << market.getBankResource(2) << ", Uranium: " << market.getBankResource(3) << "]" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "Resources available for purchase: " << endl;
	cout << "Coal: ";
	for (int i = 0; i < 9; i++)
	{
		if (i == 0)
			continue;
		cout << market.getCoal(i) << " [$" << i << "] | ";
	}
	cout << endl;
	cout << "Oil: ";
	for (int i = 0; i < 9; i++)
	{
		if (i == 0)
			continue;
		cout << market.getOil(i) << " [$" << i << "] | ";
	}
	cout << endl;
	cout << "Garbage: ";
	for (int i = 0; i < 9; i++)
	{
		if (i == 0)
			continue;
		cout << market.getGarbage(i) << " [$" << i << "] | ";
	}
	cout << endl;
	cout << "Uranium: ";
	for (int i = 0; i < 17; i++)
	{
		if (i == 0 || i == 9 || i == 11 || i == 13 || i == 15)
			continue;
		cout << market.getUranium(i) << " [$" << i << "] | ";
	}
	cout << endl;
	system("pause");
	// set round and return map
	gameMap.setCurrentRound(1);
	cout << "It is time to play POWER GRID!" << endl;
	map = gameMap;
}

// Create Player component from Load File
std::vector<Player> LoadBuilder::setPlayers()
{
	using namespace std;
	ifstream inputFile;
	inputFile.open("player_Saved.txt");
	int numPlayers;
	inputFile >> numPlayers;
	std::vector<Player> players;
	string name, colour;
	int num, elektros, score, houses, plants, skipper;
	for (int i = 0; i < numPlayers; i++)
	{
		inputFile >> num;
		inputFile >> name;
		inputFile >> colour;
		inputFile >> elektros;
		inputFile >> score;
		inputFile >> houses;
		inputFile >> plants;
		inputFile >> skipper;
		inputFile >> skipper;
		inputFile >> skipper;
		players.push_back(Player(name, num, colour, houses, elektros, score));
	}
	return players;
}

// Create Cities component from Load File
std::vector<Cities> LoadBuilder::setCities()
{
	using namespace std;
	ifstream inputFile;
	inputFile.open("map_Saved.txt");
	int numCities, step;
	vector<Cities> cities;
	inputFile >> step;
	inputFile >> numCities;
	string name;
	int area, cityNum, s1, s2, s3;
	for (int i = 0; i < numCities; i++)
	{
		getline(inputFile, name);
		getline(inputFile, name);
		inputFile >> area;
		inputFile >> cityNum;
		inputFile >> s1;
		inputFile >> s2;
		inputFile >> s3;
		cities.push_back(Cities(name, area, cityNum, s1, s2, s3));
	}
	inputFile.close();
	return cities;
}

// Create Plants component from Load File
std::vector<Plants> LoadBuilder::setPlants()
{
	using namespace std;
	vector<Plants> plants;
	ifstream inputFile;
	inputFile.open("plants_Saved.txt");
	int total;
	inputFile >> total;
	for (int i = 0; i < total; i++)
	{
		int id, value, powered, coalCost, oilCost, garbageCost, uraniumCost, coalHeld, oilHeld, garbHeld, uranHeld, s;
		inputFile >> id;
		inputFile >> value;
		inputFile >> powered;
		inputFile >> coalCost;
		inputFile >> oilCost;
		inputFile >> garbageCost;
		inputFile >> uraniumCost;
		inputFile >> coalHeld;
		inputFile >> oilHeld;
		inputFile >> garbHeld;
		inputFile >> uranHeld;
		inputFile >> s;
		plants.push_back(Plants::Plants(id, value, powered, coalCost, oilCost, garbageCost, uraniumCost, coalHeld, oilHeld, garbHeld, uranHeld, s));
	}
	inputFile.close();
	return plants;
}

// Create Market component from Load File
Market LoadBuilder::setMarket()
{
	using namespace std;
	ifstream inputFile;
	inputFile.open("market_Saved.txt");
	int coalBank, oilBank, garbBank, uranBank, value;
	inputFile >> coalBank;
	inputFile >> oilBank;
	inputFile >> garbBank;
	inputFile >> uranBank;
	Market market(coalBank, oilBank, garbBank, uranBank);
	for (int i = 0; i < 9; i++)
	{
		inputFile >> value;
		market.setCoal(i, value);
	}
	for (int i = 0; i < 9; i++)
	{
		inputFile >> value;
		market.setOil(i, value);
	}
	for (int i = 0; i < 9; i++)
	{
		inputFile >> value;
		market.setGarbage(i, value);
	}
	for (int i = 0; i < 17; i++)
	{
		inputFile >> value;
		market.setUranium(i, value);
	}
	return market;
}