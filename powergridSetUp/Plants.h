#ifndef PLANTS_H
#define PLANTS_H
#include <vector>
#include <string>
#include <iostream>




class Plants
{
private:
	int plantID;
	int value;
	int citiesPowered;
	int resourcesHeld[4];
	int resourceCost[4];
	int resourceMax[4];
	int status;

public:
	Plants();
	Plants(int id, int val, int powered, int coalCost, int oilCost, int garbCost, int uranCost);
	Plants(int id, int val, int powered, int coalCost, int oilCost, int garbCost, int uranCost, int coalHeld, int oilHeld, int garbHeld, int uranHeld, int status);
	Plants(const Plants& p);
	~Plants();

	void setValue(int v);

	int getPlantID();
	int getCitiesPowered();
	int getOilCost();
	int getCoalCost();
	int getGarbCost();
	int getUranCost();
	int getOilHeld();
	int getCoalHeld();
	int getGarbHeld();
	int getUranHeld();
	int getOilMax();
	int getCoalMax();
	int getGarbMax();
	int getUranMax();
	int getValue();
	int getStatus();

	void changeStatus();

	int addCoal(int i);
	int addOil(int i);
	int addGarb(int i);
	int addUran(int i);
	int removeCoal(int i);
	int removeOil(int i);
	int removeGarb(int i);
	int removeUran(int i);

	bool canPower();
};

#endif
