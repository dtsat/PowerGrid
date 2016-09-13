#include "stdafx.h"
#include "Cities.h"
#include <string>
#include <vector>

// City default constructor
Cities::Cities()
{
	name = "NULL";
	for (int i = 0; i < 3; i++)
		slot[i] = -1;
	area = -1;
	playable = false;
	cityNum = -1;
	visited = false;
}

// City contruction, takes a string (name) and 2 integers (area and city number)
// Cities are initialized as not playable, and with no owners
Cities::Cities(std::string n, int a, int c)
{
	name = n;
	for (int i = 0; i < 3; i++)
		slot[i] = -1;
	area = a;
	cityNum = c;
	playable = false;
	visited = false;
}

// City copy contructor
Cities::Cities(const Cities& c)
{
	name = c.name;
	for (int i = 0; i < 3; i++)
		slot[i] = c.slot[i];
	area = c.area;
	cityNum = c.cityNum;
	playable = c.playable;
	visited = c.visited;
}

Cities::Cities(std::string n, int a, int c, int s1, int s2, int s3)
{
	name = n;
	slot[0] = s1;
	slot[1] = s2;
	slot[2] = s3;
	area = a;
	cityNum = c;
	playable = false;
	visited = false;
}

// Destructor
Cities::~Cities()
{
}

// Return city name
std::string Cities::getName()
{
	return name;
}

// Return owner of slot 1
int Cities::getSlot(int i)
{
	return slot[i];
}

void Cities::setSlot(int i, int p)
{
	slot[i] = p;
}

bool Cities::isAvailable() {
	for (int i = 0; i < 3; i++)
		if (getSlot(i) == -1)
			return true;
	return false;
}

// Returns the area of the city
int Cities::getArea()
{
	return area;
}

// Returns whether or not the city is currently playable
bool Cities::getPlayable()
{
	return playable;
}

// Returns city number
int Cities::getCityNum()
{
	return cityNum;
}

// Not yet used, returned if city was "visited", which is used for graph traversals
bool Cities::getVisited()
{
	return visited;
}

// Set city as playable
void Cities::setPlayable(bool i)
{
	playable = i;
}

// Set as visited
void Cities::setVisited(bool i)
{
	visited = i;
}

// returns true or false depending on if player p owners city
bool Cities::ownedBy(int p)
{
	for (int i = 0; i < 3; i++)
		if (getSlot(i) == p)
			return true;
	return false;
}