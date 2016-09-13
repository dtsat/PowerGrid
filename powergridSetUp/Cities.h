#ifndef CITIES_H
#define CITIES_H
#include <string>

// Header file for Cities class
// These functions are called by the Map class to maipulate city objects.
// Cities are always held in the Map class, and have indicators that show who they belong to. The Player object never directly holds a City.
// Cities have attributes to show who owns them, whether they are playable (in a playable area), which area they belong to, and a name.
// There is also a "visited" flag which will be used later for graph traversals.

class Cities
{
private:
	std::string name;
	int slot[3];
	int area;
	bool playable;
	int cityNum;
	bool visited;

public:
	Cities();
	Cities(std::string n, int a, int c);
	Cities(std::string n, int a, int c, int s1, int s2, int s3);
	Cities(const Cities& c);
	~Cities();

	std::string getName();
	bool ownedBy(int p);

	int getSlot(int i);
	void setSlot(int i, int p);
	bool isAvailable();

	int getArea();
	bool getPlayable();
	int getCityNum();
	bool getVisited();

	void setPlayable(bool i);
	void setVisited(bool i);

};

#endif