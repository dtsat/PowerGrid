#include "stdafx.h"
#include "marketView.h"
#include "Market.h"
#include <string>
#include <Windows.h>

// Constructor
marketView::marketView()
{
}

// Destructor
marketView::~marketView()
{
}

// Update, when called with a map object and integer indicating the type of update to perform
// Sets a string to form a message indicating all the most updated information 
// Viewer object uses the Windows API to display updated information to the screen in the form of a popup
// Any time information is changed and the appropriate notify function is called, this function will execute
void marketView::update(Observable* ob, int j)
{
	// Ensure Observable object passed is a Map
	if (auto map = dynamic_cast<Map*>(ob))
	{
		if (j == 2)
		{
			string output = "";
			output += "\n---------------------------";
			output += "\nAvailable Resources";
			output += "\n---------------------------";

			output += "\n1 - Coal: ";
			// Display available coal
			for (int j = 1; j < 9; j++)
				if (map->getMarket().getCoal(j) > 0) 
				{
					output += to_string(map->getMarket().getCoal(j)); 
					output += " x [0"; 
					output += to_string(j); 
					output += "$] | ";
				}
			output += "\n2 - Oil: ";
			// Display available oil
			for (int j = 1; j < 9; j++)
				if (map->getMarket().getOil(j) > 0) {
					output += to_string(map->getMarket().getOil(j));
					output += " x [0";
					output += to_string(j);
					output += "$] | ";
				}
			output += "\n3 - Garbage: ";
			// Display available garbage
			for (int j = 1; j < 9; j++)
				if (map->getMarket().getGarbage(j) > 0) {
					output += to_string(map->getMarket().getGarbage(j)); 
					output += " x [0"; 
					output += to_string(j); 
					output += "$] | ";
				}
			output += "\n4 - Uranium: ";
			// Display available uranium
			for (int j = 1; j < 17; j++)
				if (map->getMarket().getUranium(j) > 0) {
					output += to_string(map->getMarket().getUranium(j));
					output += " x ["; 
					output += to_string(j); 
					output += "$] | ";
				}
			output += "\n---------------------------";

			// Display in Windows Dialog box
			MessageBoxA(0, output.c_str(), "Resource Market Updated!", 0);

		}
	}
}