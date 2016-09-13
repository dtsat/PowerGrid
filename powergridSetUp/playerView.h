#ifndef PLAYER_V_H
#define PLAYER_V_H
#include "Observer.h"
#include "Map.h"
#include "Plants.h"
#include "Cities.h"
#include "Player.h"
#include "Market.h"
#include <vector>
#include <iostream>
#include <string>

// Observer object for observing Players
// The Observer, upon notification, will call the update function
// The update function takes a map and integer as parameters. The integer indicates the type of update to perform
// If the appropriate integer is sent to this class, it will perform update and display updated information appropriate for this Observer


class playerView : public Observer
{
public:
	playerView();
	~playerView();
	void update(Observable* ob, int j);
};

#endif