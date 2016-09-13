#include "stdafx.h"
#include "mapView.h"
#include "Map.h"
#include <Windows.h>
#include <string>

// Constructor
mapView::mapView()
{
}

// Destructor
mapView::~mapView()
{
}

// Update, when called with a map object and integer indicating the type of update to perform
// Sets a string to form a message indicating all the most updated information 
// Viewer object uses the Windows API to display updated information to the screen in the form of a popup
// Any time information is changed and the appropriate notify function is called, this function will execute
void mapView::update(Observable* ob, int j)
{
	// Check if observable object passed is a Map
	if (auto map = dynamic_cast<Map*>(ob))
	{
		if (j == 1)
		{
			string output = "";
			output += "Current Map Statistics\n";
			output += "---------------------------------------------------------------------------------------------\n";
			for (int j = 0; j < map->areaSize(); j++)
			{
				output += "AREA "; 
				output += to_string(map->getPlayableAreas(j)); 
				output += "\n";
				for (int i = 0; i < map->citiesSize(); i++)
				{
					if (map->getCityArea(i) == map->getPlayableAreas(j))
					{
						output += "-----------------------------------------------------\n";
						output += to_string(map->getCityNum(i)); 
						output += ": "; 
						output += map->getCityName(i); 
						output += "\n";
						string slots[3];
						int houses = 0;
						for (int k = 0; k < 3; k++)
						{
							if (map->getCitySlot(k, i) == -1)
								slots[k] = "AVAILABLE";
							else
							{
								slots[k] = map->getPlayerName(map->getCitySlot(k, i));
								houses++;
							}
						}
						output += "Owners: [1] "; 
						output += slots[0]; 
						output += ", [2] "; 
						output += slots[1]; 
						output += ", [3] "; 
						output += slots[2]; 
						output += "\n";
						output += "Currently "; 
						output += to_string(houses); 
						output += " Houses in city.\n"; 
						output += "Adjacent to: \n"; 
						for (int l = 0; l < map->citiesSize(); l++)
						{
							if (map->isAdjacent(i, l) && map->getCityPlayable(l) == true)
							{
								if (map->edgeCost(i, l) == -1)
								{
									output += map->getCityName(l); 
									output += " (cost: FREE), ";
								}
								else
								{
									output += map->getCityName(l); 
									output += " (cost: "; 
									output += to_string(map->edgeCost(i, l)); 
									output += "), ";
								}
							}
						}
						output += "\n";
					}
				}
				output += "-----------------------------------------------------\n";
				// Display message in Windows Dialog Box
				MessageBoxA(0, output.c_str(), "Current Map", 0);
				output = "";
			}
			
		}
		
	}

}
