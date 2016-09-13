#include "stdafx.h"
#include "playerView.h"
#include "Player.h"
#include <Windows.h>
#include <string>

// Constructor
playerView::playerView()
{
}

// Destructor
playerView::~playerView()
{
}

// Update, when called with a map object and integer indicating the type of update to perform
// Sets a string to form a message indicating all the most updated information 
// Viewer object uses the Windows API to display updated information to the screen in the form of a popup
// Any time information is changed and the appropriate notify function is called, this function will execute
void playerView::update(Observable* ob, int j)
{
	// Check if observable object passed is a Map
	if (auto map = dynamic_cast<Map*>(ob))
	{
		if (j == 0)
		{
			string output = "";
			for (int i = 0; i < map->playersSize(); i++) 
			{
				output += "\nPlayer ";
				output += to_string(i + 1); 
				output += ": ";
				output += map->getPlayerName(i); 
				output += " ["; 
				output += map->getPlayerColour(i); 
				output += "]";
				output += "\n---------------------------";
				output += "\nElektros: "; 
				output += to_string(map->getPlayerElektros(i));
				output += "\nScore: "; 
				output += to_string(map->getPlayerScore(i));
				output += "\nPlants:\n";
				for (int j = 0; j < 3; j++)
				{
					
					if (map->getPlayer(i).getPlant(j).getValue() == 0);
					else
					{
						output += "Plant #";
						output += to_string(map->getPlayer(i).getPlant(j).getValue());
						output += ": ";
						output += "Coal: ";
						output += to_string(map->getPlayer(i).getPlant(j).getCoalHeld());
						output += "/";
						output += to_string(map->getPlayer(i).getPlant(j).getCoalMax());
						output += " | ";
						output += "Oil: ";
						output += to_string(map->getPlayer(i).getPlant(j).getOilHeld());
						output += "/";
						output += to_string(map->getPlayer(i).getPlant(j).getOilMax());
						output += " | ";
						output += "Garbage: ";
						output += to_string(map->getPlayer(i).getPlant(j).getGarbHeld());
						output += "/";
						output += to_string(map->getPlayer(i).getPlant(j).getGarbMax());
						output += " | ";
						output += "Uranium: ";
						output += to_string(map->getPlayer(i).getPlant(j).getUranHeld());
						output += "/";
						output += to_string(map->getPlayer(i).getPlant(j).getUranMax());
						output += "\n";
					}
				}
				output += "Houses remaining: "; 
				output += to_string(map->getPlayerHouses(i));
				output += "\nOwned Cities:\n";
				output += "| ";
				for (int k = 0; k < map->citiesSize(); k++)
				{
					if (map->isCityOwned(k, i))
					{
						output += map->getCityName(k);
						output += " | ";
					}
				}
				output += "\n";
			}
			// Display message in Windows Dialog box
			MessageBoxA(0, output.c_str(), "Current Player Information", 0);
		}
	}
}