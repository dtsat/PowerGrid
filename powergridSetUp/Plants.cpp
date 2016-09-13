#include "stdafx.h"
#include "Plants.h"


Plants::Plants()
{
	plantID = -1;
	value = 0;
	citiesPowered = -1;
	resourceCost[0] = 0;
	resourceCost[1] = 0;
	resourceCost[2] = 0;
	resourceCost[3] = 0;
	resourcesHeld[0] = 0;
	resourcesHeld[1] = 0;
	resourcesHeld[2] = 0;
	resourcesHeld[3] = 0;
	resourceMax[0] = 0;
	resourceMax[1] = 0;
	resourceMax[2] = 0;
	resourceMax[3] = 0;
	status = 0;
}

Plants::Plants(int id, int v, int powered, int cCost, int oCost, int gCost, int uCost)
{
	plantID = id;
	value = v;
	citiesPowered = powered;
	resourceCost[0] = cCost;
	resourceCost[1] = oCost;
	resourceCost[2] = gCost;
	resourceCost[3] = uCost;
	resourcesHeld[0] = 0;
	resourcesHeld[1] = 0;
	resourcesHeld[2] = 0;
	resourcesHeld[3] = 0;
	resourceMax[0] = 2 * cCost;
	resourceMax[1] = 2 * oCost;
	resourceMax[2] = 2 * gCost;
	resourceMax[3] = 2 * uCost;
	status = 0;
}

Plants::Plants(int id, int val, int powered, int coalCost, int oilCost, int garbCost, int uranCost, int coalHeld, int oilHeld, int garbHeld, int uranHeld, int s)
{
	plantID = id;
	value = val;
	citiesPowered = powered;
	resourceCost[0] = coalCost;
	resourceCost[1] = oilCost;
	resourceCost[2] = garbCost;
	resourceCost[3] = uranCost;
	resourcesHeld[0] = coalHeld;
	resourcesHeld[1] = oilHeld;
	resourcesHeld[2] = garbHeld;
	resourcesHeld[3] = uranHeld;
	resourceMax[0] = 2 * coalCost;
	resourceMax[1] = 2 * oilCost;
	resourceMax[2] = 2 * garbCost;
	resourceMax[3] = 2 * uranCost;
	status = s;
}

Plants::Plants(const Plants& p)
{
	plantID = p.plantID;
	value = p.value;
	citiesPowered = p.citiesPowered;
	resourceCost[0] = p.resourceCost[0];
	resourceCost[1] = p.resourceCost[1];
	resourceCost[2] = p.resourceCost[2];
	resourceCost[3] = p.resourceCost[3];
	resourcesHeld[0] = p.resourcesHeld[0];
	resourcesHeld[1] = p.resourcesHeld[1];
	resourcesHeld[2] = p.resourcesHeld[2];
	resourcesHeld[3] = p.resourcesHeld[3];
	resourceMax[0] = p.resourceMax[0];
	resourceMax[1] = p.resourceMax[1];
	resourceMax[2] = p.resourceMax[2];
	resourceMax[3] = p.resourceMax[3];
	status = p.status;
}


Plants::~Plants()
{
}

///SIMON added
void Plants::setValue(int v) {
	this->value = v;
}

int Plants::getPlantID()
{
	return plantID;
}

int Plants::getCitiesPowered()
{
	return citiesPowered;
}

int Plants::getOilCost()
{
	return resourceCost[1];
}

int Plants::getCoalCost()
{
	return resourceCost[0];
}

int Plants::getGarbCost()
{
	return resourceCost[2];
}

int Plants::getUranCost()
{
	return resourceCost[3];
}

int Plants::getOilHeld()
{
	return resourcesHeld[1];
}

int Plants::getCoalHeld()
{
	return resourcesHeld[0];
}

int Plants::getGarbHeld()
{
	return resourcesHeld[2];
}

int Plants::getUranHeld()
{
	return resourcesHeld[3];
}

int Plants::getOilMax()
{
	return resourceMax[1];
}

int Plants::getCoalMax()
{
	return resourceMax[0];
}

int Plants::getGarbMax()
{
	return resourceMax[2];
}

int Plants::getUranMax()
{
	return resourceMax[3];
}

int Plants::getValue()
{
	return value;
}

int Plants::getStatus()
{
	return status;
}

void Plants::changeStatus() {
	if (status == 0)
		status = 1;
	else if (status == 1)
		status = -1;
}

int Plants::addCoal(int i)
{
	if (resourcesHeld[0] + i > resourceMax[0])
		return -1;
	resourcesHeld[0] += i;
	return resourcesHeld[0];
}

int Plants::addOil(int i)
{
	if (resourcesHeld[1] + i > resourceMax[1])
		return -1;
	resourcesHeld[1] += i;
	return resourcesHeld[1];
}

int Plants::addGarb(int i)
{
	if (resourcesHeld[2] + i > resourceMax[2])
		return -1;
	resourcesHeld[2] += i;
	return resourcesHeld[2];
}

int Plants::addUran(int i)
{
	if (resourcesHeld[3] + i > resourceMax[3])
		return -1;
	resourcesHeld[3] += i;
	return resourcesHeld[3];
}

int Plants::removeCoal(int i)
{
	if (resourcesHeld[0] - i < 0)
		return -1;
	resourcesHeld[0] -= i;
	return resourcesHeld[0];
}

int Plants::removeOil(int i)
{
	if (resourcesHeld[1] - i < 0)
		return -1;
	resourcesHeld[1] -= i;
	return resourcesHeld[1];
}

int Plants::removeGarb(int i)
{
	if (resourcesHeld[2] - i < 0)
		return -1;
	resourcesHeld[2] -= i;
	return resourcesHeld[2];
}

int Plants::removeUran(int i)
{
	if (resourcesHeld[3] - i < 0)
		return -1;
	resourcesHeld[3] -= i;
	return resourcesHeld[3];
}

bool Plants::canPower() {
	for (int i = 0; i < 4; i++)
		if (resourceCost[i] > 0 && resourcesHeld[i] >= resourceCost[i])
			return true;
	return false;
}