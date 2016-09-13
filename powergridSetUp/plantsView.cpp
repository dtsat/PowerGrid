#include "stdafx.h"
#include "plantsView.h"
#include "Plants.h"
#include <Windows.h>
#include <string>

// Constructor
plantsView::plantsView()
{
}

// Destructor
plantsView::~plantsView()
{
}

// Update, when called with a map object and integer indicating the type of update to perform
// Sets a string to form a message indicating all the most updated information 
// Viewer object uses the Windows API to display updated information to the screen in the form of a popup
// Any time information is changed and the appropriate notify function is called, this function will execute
void plantsView::update(Observable* ob, int j)
{
	// Check if Observable object passed is a Map
	if (auto map = dynamic_cast<Map*>(ob))
	{
		
		if (j == 3)
		{
			if (map->getCurrentStep() != 3)
			{
				string output = "";
				std::vector<int> plantDisplayed;
				for (int i = 0; i < 8; i++)
				{
					if (i == 0)
					{
						output += "Actual market";
						output += "\n---------------------------\n";
					}
					if (i == 4)
					{
						output += "---------------------------\n";
						output += "Future market";
						output += "\n---------------------------\n";
					}
					// Loop for all plants in the map
					for (int j = 0; j < map->getPlantsSize(); j++)
					{
						// Check if plant is available or it it has been displayed already
						if (map->getPlant(j).getStatus() == 0 && !(std::find(plantDisplayed.begin(), plantDisplayed.end(), map->getPlant(j).getValue()) != plantDisplayed.end()))
						{
							output += "Power plant ";
							// Add a 0 in front of values smaller than 10 for display purposes
							if (map->getPlant(j).getValue() < 10)
							{
								output += "0";
								output += to_string(map->getPlant(j).getValue());
							}
							else
							{
								output += to_string(map->getPlant(j).getValue());
							}
							// Display the value of the plant, cities powered, and resources consumed
							output += " | Powers ";
							output += to_string(map->getPlant(j).getCitiesPowered());
							output += " cities";
							output += " | Consumes [";
							output += to_string(map->getPlant(j).getCoalCost());
							output += " coal, ";
							output += to_string(map->getPlant(j).getOilCost());
							output += " oil, ";
							output += to_string(map->getPlant(j).getGarbCost());
							output += " garbage, ";
							output += to_string(map->getPlant(j).getUranCost());
							output += " uranium]\n";
							// Add the plant to displayed plants vector
							plantDisplayed.push_back(map->getPlant(j).getValue());
							break;
						}
					}
					if (i == 7) {
						output += "---------------------------\n";
					}
				}
				// Display message in Windows Dialog box
				MessageBoxA(0, output.c_str(), "Plant Market Updated!", 0);
			}
			else
			{
				string output = "";
				std::vector<Plants> availableMarket;
				std::vector<int> plantDisplayed;
				for (int i = 0; i < 8; i++)
				{
					if (i == 0) {
						output += "Actual market";
						output += "\n---------------------------\n";
					}
					if (i < 6) {
						for (int j = 0; j < map->getPlantsSize(); j++) {
							// Check if plant is available or it it has been displayed already
							if (map->getPlant(j).getStatus() == -1 && !(std::find(plantDisplayed.begin(), plantDisplayed.end(), map->getPlant(j).getValue()) != plantDisplayed.end())) {
								// Add the plant to available plants vector if it is in the actual market
								availableMarket.push_back(map->getPlant(j));
								output += "Power plant ";
								// Add a 0 in front of values smaller than 10 for display purposes
								if (map->getPlant(j).getValue() < 10)
								{
									output += "0";
									output += to_string(map->getPlant(j).getValue());
								}
								else
								{
									output += to_string(map->getPlant(j).getValue());
								}
								// Display the value of the plant, cities powered, and resources consumed
								output += " | Powers "; 
								output += to_string(map->getPlant(j).getCitiesPowered()); 
								output += " cities";
								output += " | Consumes ["; 
								output += to_string(map->getPlant(j).getCoalCost()); 
								output += " coal, ";
								output += to_string(map->getPlant(j).getOilCost());
								output += " oil, "; 
								output += to_string(map->getPlant(j).getGarbCost());
								output += " garbage, ";
								output += to_string(map->getPlant(j).getUranCost()); 
								output += " uranium]\n"; 
								// Add the plant to displayed plants vector
								plantDisplayed.push_back(map->getPlant(j).getValue());
								break;
							}
						}
					}
					if (i == 5) {
						output += "---------------------------\n";
					}
				}
				// Display message in Windows Dialog box
				MessageBoxA(0, output.c_str(), "Plant Market Updated!", 0);
			}
		}

	}
}