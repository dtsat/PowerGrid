#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include "Plants.h"
#include "Player.h"

// Default constructor
Player::Player()
{
	name = "NULL";
	playerNum = -1;
	colour = "NULL";
	elektros = -1;
	score = -1;
	housesMax = -1;
	citiesPowered = 0;
}

// Constructor takes a string (name) an int (player number) and another strong (colour)
// Max houses initialized to h, and initial score is s
// Initial Elektros is e
Player::Player(std::string n, int p, std::string c, int h, int e, int s)
{
	name = n;
	playerNum = p;
	colour = c;
	elektros = e;
	score = s;
	housesMax = h;
	citiesPowered = 0;
}

// copy constructor
Player::Player(const Player& p)
{
	name = p.name;
	playerNum = p.playerNum;
	colour = p.colour;
	elektros = p.elektros;
	score = p.score;
	housesMax = p.housesMax;
	for (int i = 0; i < 3; i++)
		plants[i] = p.plants[i];
	citiesPowered = p.citiesPowered;
}

// Destructor
Player::~Player()
{
}

// Accessors for player attributes
std::string Player::getName()
{
	return name;
}

int Player::getElektros()
{
	return elektros;
}

int Player::getScore()
{
	return score;
}

/// SIMON added
int Player::getCitiesPowered() {
	return citiesPowered;
}

void Player::setCitiesPowered(int i) {
	citiesPowered = i;
}

bool Player::isPlantOwned(int i) {
	for (int j = 0; j < 3; j++)
		if (plants[j].getValue() == i)
			return true;
	return false;
}

void Player::removePlant(int s) {
	plants[s].setValue(0);
}
/// END

int Player::getNumPlants()
{
	int j = 0;
	for (int i = 0; i < 3; i++)
	{
		if (plants[i].getValue() > 0)
			j++;
	}
	return j;
}

int Player::getBestPlant() {
	int highestValue = 0;
	for (int i = 0; i < 3; i++) {
		if (plants[i].getValue() == 0)
			highestValue = 0;
		else if (plants[i].getValue() > highestValue)
			highestValue = plants[i].getValue();
	}
	return highestValue;
}

void Player::setHouses(int x) {
	housesMax = x;
}

int Player::getHouses() {
	return housesMax;
}

bool Player::addPlant(Plants p) {
	for (int i = 0; i < 3; i++)
		if (plants[i].getValue() == 0) {
			plants[i] = p;
			return true;
		}
	return false;
}

Plants Player::getPlant(int i) {
	Plants tempPlant = plants[i];
	return tempPlant;
}

void Player::addPlantResource(int p, int r, int q) {
	switch (r) {
	case 1:
		plants[p].addCoal(q);
		break;
	case 2:
		plants[p].addOil(q);
		break;
	case 3:
		plants[p].addGarb(q);
		break;
	case 4:
		plants[p].addUran(q);
		break;
	}
}

void Player::removePlantResource(int p, int r, int q) {
	switch (r) {
	case 1:
		plants[p].removeCoal(q);
		break;
	case 2:
		plants[p].removeOil(q);
		break;
	case 3:
		plants[p].removeGarb(q);
		break;
	case 4:
		plants[p].removeUran(q);
		break;
	}
}

int Player::getPlayerNum()
{
	return playerNum;
}

std::string Player::getColour()
{
	return colour;
}

int Player::getHousesLeft()
{
	return housesMax;
}

// Mutators for player attributes
void Player::setElektros(int e)
{
	elektros = e;
}

int Player::modElektros(int c)
{
	if ((elektros + c) < 0)
		return -1;
	else
		elektros += c;
	return elektros;
}

void Player::increaseScore()
{
	score++;
	housesMax--;
}

