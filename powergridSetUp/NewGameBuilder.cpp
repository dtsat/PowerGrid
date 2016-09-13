#include "stdafx.h"
#include "NewGameBuilder.h"
#include <fstream>

// Constructor
NewGameBuilder::NewGameBuilder()
{
}

// Destructor
NewGameBuilder::~NewGameBuilder()
{
}

// Creates a Map object using the New Game parameters
// Map object pass as argument is not used, only for being able to override the function
// Object is created from initial game load files for new games
void NewGameBuilder::createMap(Map m)
{
	using namespace std;
	cout << "You have selected to start a new game!" << endl;
	cout << "The game will begin after we choose a few initial settings.\n" << endl;
	system("pause");
	// create player, cities, plants, market
	std::vector<Player> players = setPlayers();
	std::vector<Cities> cities = setCities();
	std::vector<Plants> plants = setPlants();
	Market market = setMarket();

	Map gameMap(players, cities, plants, market);
	ifstream inputFile;
	inputFile.open("map_Input.txt");
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
	inputFile.open("map_Input.txt");
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
	// Choose playable areas and eliminate those not in play
	cout << "\n\nBased on the number of players, you must now select the Areas valid for play." << endl;
	int choice, areasToUse;
	vector<int> selected;
	switch (players.size())
	{
	case 2:
	case 3:
	{
		areasToUse = 3;
		break;
	}
	case 4:
	{
		areasToUse = 4;
		break;

	}
	case 5:
	case 6:
	{
		areasToUse = 5;
		break;
	}
	}
	cout << "For " << players.size() << " players, you must choose " << areasToUse << " areas for play." << endl;
	cout << "There are " << numAreas << " areas to choose from." << endl;
	cout << "Please choose an area from 0 to " << numAreas - 1 << ":" << endl;
	cin >> choice;
	while (choice < 0 || choice >= numAreas)
	{
		cout << "Invalid. Please choose an area from 0 to " << numAreas - 1 << ":" << endl;
		cin >> choice;
	}
	selected.push_back(choice);
	gameMap.setPlayableAreas(choice);
	for (int i = 0; i < areasToUse - 1; i++)
	{
		cout << "You have chosen area " << choice << ". You must now choose an adjacent area." << endl;
		cout << "You have the following choices: " << endl;
		gameMap.showAreaAdjacency(choice);
		cout << "Please select your next area:" << endl;
		cin >> choice;
		while (gameMap.areaIsAdjacent(selected[i], choice) == false)
		{
			cout << "This area is not adjacent! Please choose again!" << endl;
			cin >> choice;
		}
		bool inside = true;
		while (inside)
		{
			inside = false;
			for (int j = 0; j < selected.size(); j++)
			{
				if (choice == selected[j])
					inside = true;
			}
			if (inside)
			{
				cout << "You have already selected this area! Please choose again!" << endl;
				cin >> choice;
			}
		}
		selected.push_back(choice);
		gameMap.setPlayableAreas(choice);
	}
	cout << "Areas selected: ";
	for (int i = 0; i < selected.size(); i++)
		cout << selected[i] << ", ";
	cout << "All other areas will not be playable in this game.\nWe will display the playable map information." << endl;
	// enable valid playable cities
	for (int i = 0; i < cities.size(); i++)
	{
		for (int j = 0; j < selected.size(); j++)
		{
			if (selected[j] == cities[i].getArea())
				gameMap.setCityPlayable(i, true);
		}
	}
	system("pause");
	std::cout << std::endl;
	gameMap.showAdjacency();
	system("pause");
	// display resource market
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
	gameMap.setCurrentRound(0);
	cout << "It is time to play POWER GRID!" << endl;
	map = gameMap;
}

// Create the Players component
std::vector<Player> NewGameBuilder::setPlayers()
{
	using namespace std;
	ifstream inputFile;
	inputFile.open("player_Input.txt");
	int elektros;
	int housesMax;
	inputFile >> elektros;
	inputFile >> housesMax;
	vector<string> colours = { "Red", "Green", "Blue", "Yellow", "Purple", "Black", "White" };
	vector<Player> players;
	inputFile.close();
	cout << "The first step is to choose the number of players that will be playing the game! The minimum is 2 and the maximum is 6." << endl;
	cout << "Please choose now [min 2, max 6]:" << endl;
	int num;
	cin >> num;
	while (num > 6 || num < 2)
	{
		cout << "Invalid entry. Please choose now [min 2, max 6]:" << endl;
		cin >> num;
	}
	cout << "We will play the game with " << num << " players!" << endl;
	cout << "-------------------------------------------------------------------------------------------\n" << endl;
	cout << "We will now ask each player to enter their names." << endl;
	for (int i = 0; i < num; i++)
	{
		cout << "Player " << i + 1 << ", please enter your name:" << endl;
		string name;
		cin >> name;
		srand(time(0));
		int colourIndex = rand() % (colours.size() - 1);
		swap(colours[colourIndex], colours.back());
		players.push_back(Player(name, i, colours.back(), housesMax, elektros, 0));
		colours.pop_back();
		cout << "Player " << i + 1 << ": Your name is " << players[i].getName() << " and you have been randomly assigned the colour " << players[i].getColour() << "!\n" << endl;
	}
	cout << "\nEach player has been given " << elektros << " Elektros to start the game!" << endl;
	cout << "We have also placed a house on the scoring track. Each player has " << housesMax << " houses remaining that can be used to build in a city." << endl;
	cout << "The stats to begin the game are as follows: " << endl;
	cout << "-------------------------------------------------------------------" << endl;
	for (int i = 0; i < players.size(); i++)
	{
		cout << "Player " << players[i].getPlayerNum() + 1 << ": " << players[i].getColour() << " (" << players[i].getName() << ")" << endl;
		cout << "Number of houses currently owned: " << players[i].getScore() << endl;
		cout << "Number of houses remaining: " << players[i].getHousesLeft() << endl;
		cout << "Number of plants currently owned: " << players[i].getNumPlants() << endl;
		cout << "Elektros: " << players[i].getElektros() << endl;
		cout << "-------------------------------------------------------------------" << endl;
	}
	system("pause");
	return players;
}

// Create the Cities component
std::vector<Cities> NewGameBuilder::setCities()
{
	using namespace std;
	ifstream inputFile;
	inputFile.open("map_Input.txt");
	int numCities, step;
	vector<Cities> cities;
	inputFile >> step;
	inputFile >> numCities;
	string name;
	int area, cityNum;
	for (int i = 0; i < numCities; i++)
	{
		getline(inputFile, name);
		getline(inputFile, name);
		inputFile >> area;
		inputFile >> cityNum;
		cities.push_back(Cities(name, area, cityNum));
	}
	inputFile.close();
	return cities;
}

// Create the Plants component
std::vector<Plants> NewGameBuilder::setPlants()
{
	using namespace std;
	cout << "\n\nPower plants have been loaded and created from the file." << endl;
	vector<Plants> plants;
	ifstream inputFile;
	inputFile.open("plants_Input.txt");
	int total;
	inputFile >> total;
	for (int i = 0; i < total; i++)
	{
		int id, value, powered, coal, oil, garbage, uranium;
		inputFile >> id;
		inputFile >> value;
		inputFile >> powered;
		inputFile >> coal;
		inputFile >> oil;
		inputFile >> garbage;
		inputFile >> uranium;
		plants.push_back(Plants(id, value, powered, coal, oil, garbage, uranium));
	}
	inputFile.close();
	cout << "The initial starting power plant market is as follows: " << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "Available for auction:" << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << endl;
		cout << i << ": Minimum Bid " << plants[i].getValue() << endl;
		cout << "Houses Powered: " << plants[i].getCitiesPowered() << endl;
		cout << "Resource cost: " << plants[i].getCoalCost() << " Coal, " << plants[i].getOilCost() << " Oil, " << plants[i].getGarbCost() << " Garbage, " << plants[i].getUranCost() << " Uranium" << endl;
		cout << "Resource max storage: " << plants[i].getCoalMax() << " Coal, " << plants[i].getOilMax() << " Oil, " << plants[i].getGarbMax() << " Garbage, " << plants[i].getUranMax() << " Uranium" << endl;
	}
	cout << "-------------------------------------------------------------------" << endl;
	system("pause");
	cout << "\nNot yet available for auction:" << endl;
	for (int i = 4; i < 8; i++)
	{
		cout << endl;
		cout << i << ": Minimum Bid " << plants[i].getValue() << endl;
		cout << "Houses Powered: " << plants[i].getCitiesPowered() << endl;
		cout << "Resource cost: " << plants[i].getCoalCost() << " Coal, " << plants[i].getOilCost() << " Oil, " << plants[i].getGarbCost() << " Garbage, " << plants[i].getUranCost() << " Uranium" << endl;
		cout << "Resource max storage: " << plants[i].getCoalMax() << " Coal, " << plants[i].getOilMax() << " Oil, " << plants[i].getGarbMax() << " Garbage, " << plants[i].getUranMax() << " Uranium" << endl;
	}
	cout << "-------------------------------------------------------------------" << endl;
	system("pause");
	return plants;
}

// Create the Market component
Market NewGameBuilder::setMarket()
{
	using namespace std;
	ifstream inputFile;
	inputFile.open("market_Input.txt");
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