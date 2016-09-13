#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <iostream>
#include "Plants.h"

// The player class represents each player.
// A player object has a name, number, colour, # of elektros, score (number of houses), vector of plants (not used) and number of houses remaining (max 21)
// The functions in this class are mostly called by the map class

class Player
{
private:
	std::string name;
	int playerNum;
	std::string colour;
	int elektros;
	int score;
	Plants plants[3];
	int housesMax;
	int citiesPowered;
public:
	Player();
	Player(std::string n, int p, std::string c, int h, int e, int s);
	Player(const Player& p);
	~Player();

	std::string getName();
	int getElektros();
	int getScore();

	int getNumPlants();

	int getCitiesPowered();
	void setCitiesPowered(int i);
	bool isPlantOwned(int i);
	void removePlant(int s);
	int getBestPlant();
	void setHouses(int x);
	int getHouses();
	bool addPlant(Plants plant);
	Plants getPlant(int i);
	void addPlantResource(int p, int r, int q);
	void removePlantResource(int p, int r, int q);

	int getPlayerNum();
	std::string getColour();

	int getHousesLeft();

	void setElektros(int e);

	int modElektros(int c);
	void increaseScore();












};

#endif