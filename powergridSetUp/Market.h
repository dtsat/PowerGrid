#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "stdafx.h"

using namespace std;

class Market
{
	int coal[9];
	int oil[9];
	int garbage[9];
	int uranium[17];
	int resourceBank[4];
public:
	Market();
	Market(int c, int o, int g, int u);
	/// [SIMON] Added for map: getMarket
	Market(const Market &m);
	~Market();

	int getBankResource(int slot);
	void setBankResource(int slot, int quantity);

	int getCoal(int slot);
	void setCoal(int slot, int quantity);
	void addCoal(int quantity);
	void removeCoal(int slot, int quantity);

	int getOil(int slot);
	void setOil(int slot, int quantity);
	void addOil(int quantity);
	void removeOil(int slot, int quantity);

	int getGarbage(int slot);
	void setGarbage(int slot, int quantity);
	void addGarbage(int quantity);
	void removeGarbage(int slot, int quantity);

	int getUranium(int slot);
	void setUranium(int slot, int quantity);
	void addUranium(int quantity);
	void removeUranium(int slot, int quantity);
};