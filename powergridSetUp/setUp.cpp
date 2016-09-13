#include "stdafx.h"
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "Player.h"
#include "Cities.h"
#include "Plants.h"
#include "Map.h"
#include "setUp.h"
#include "Market.h"
#include "SaveLoadDirector.h"
#include "NewGameBuilder.h"
#include "LoadBuilder.h"


// This class is used to set up the Map used for play
// The user is prompted to either start a new game or load game from file
// A SaveLoadDirector object is created, along with the appropriate type of Builder, which then creates the Map
// The function returns the created Map object with all components and returns it, ready for play

Map setup_Map()
{
	using namespace std;
	cout << "Welcome to Power Grid! The game of ultimate Power!" << endl;
	cout << "Please choose one of the following 2 options:" << endl;
	cout << "----------------------------------------------" << endl;
	cout << "1:\tStart New Game" << endl;
	cout << "2:\tLoad Previously Saved Game" << endl;
	cout << "----------------------------------------------" << endl;
	int choice;
	SaveLoadDirector director;
	Map m;
	cout << "Make your choice now:" << endl;
	cin >> choice;
	while (choice < 1 || choice > 2)
	{
		cout << "You have make an invalid choice. Please try again." << endl;
		cin >> choice;
	}
	// start new game
	if (choice == 1)
	{
		// New Game Builder
		NewGameBuilder* builder = new NewGameBuilder;
		director.setBuilder(builder);
		director.buildMap(m);
		m = director.getMap();
		delete builder;
		return m;

	}
	else if (choice == 2)
	{
		// Load Game Builder
		LoadBuilder* builder = new LoadBuilder;
		director.setBuilder(builder);
		director.buildMap(m);
		m = director.getMap();
		delete builder;
		return m;
	}

	Map errorMap;
	return errorMap;
}

