#ifndef INITIAL_H
#define INITIAL_H
#include <vector>
#include <string>
#include <algorithm>
#include "Player.h"
#include "Cities.h"
#include "Plants.h"
#include "Map.h"
#include "Market.h"

// The setUp file contains a function that returns a Map object
// The function uses the Builder pattern to create a SaveLoadDirector object, which decides the process for how the Map is created
// The user selects the type of Load they wish to perform, and the Director sets the appropriate builder, which creates and returns a Map object ready for play
// User can load a game from file, or start a new game


Map setup_Map();

#endif
