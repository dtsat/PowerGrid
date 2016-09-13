POWER GRID
Team V
David Tsatsoulis
Simon Jacques
Jonathan Cardone
Theo Grimond

This program contains a text-based console implementation of the Power Grid board-game
Setup is fully customizable using input files that determine starting settings for players, cities, market and power plants
This program supports new game, load previous game, and will automatically save the current game state, which can later be loaded, after round 5 of play
Game play is supported for 2 to 6 players
Phases 1 to 5 of the game are implemented, including determining player order, auctioning of power plants, purchasing of resources, building in cities and bureaucracy.

Phase 1 will determine the order of Players based on the game rules

Phase 2 allows players to bid on and purchase power plants. Players can hold up to 3 power plants and will be prompted to discard a plant if they already own 3 plants.
Resources stored in discarded plants are re-allocated to their plants if there is space. All other resources are returned to the bank.

Phase 3 allows players to purchase resources provided that they have space in their power plants to be able to store these resources.
Each plant is capable of holding up to 2 times the resource cost of the plant.

Phase 4 allows players to purchase houses in cities.
Players can only build in cities that are adjacent to an owned city, and the cost depends on the slot being build into, and the connection cost.

Phase 5, the bureaucracy stage, has players consume resources in their power plants and power the maximum number of cities possible based on the Plant value.
Consumed resources are returned to the Market bank.
The market may be refilled depending on current Market resource levels.
The Power Plant market will be adjusted, with the highest value plant being discarded.

The game supports 3 Steps, in which certain rules are modified.
The game begins in Step 1, and transitions to Step 2 when 7 houses have been built.
Step 3 begins when the special Step 3 card has been pulled in the Power Plant auction market.
The Plant auction market is changed, with all plants being shuffled and the players being able to purchase any of the available plants.

The game has a winning condition where upon the purchase of a 17th house, the player who can power the most houses at this point will win the game.

Structure:
The game is built using a Player, Plants, Cities, Market and Map class. The Map class contains each of the previous 4 class objects and is the main driver object of the program
All city and plant objects are held by the map. The map holds the current market, and current player vector as well.
Plants hold resources, and players reference the plants they own. Cities reference the players that currently own them as well, via a player ID number.
The market holds and controls the distribution of resources into the game. Resources are finite.
The number of cities that can be powered is determined by the plants held, and the resources held in each plant. 
Players are awarded Elektros at the end of Phase 5 depending on the number of resources held in each plant.

The Map is created from an input file and implements a connected graph using an adjacency matrix.
This allows for quick and low cost checking of adjacency between cities.

The game implements a number of design patters.

Builder Pattern:
The game implements a Builder Pattern to be able to Save, Load and create a New game.
Creating a SaveLoadDirector, and setting the desired Builder type, allows for a Builder to create and return a Map using the method detailed by the Builder.
There are builders for Saving the game, Loading a previous game, and creating a New game.

Observer Pattern:
The game has an Observable Object, the Map, which holds Players, Plants, Cities and a Market.
The Map is Observed by a mapView, playerView, marketView and plantsView. Each view is attached to the Map and waits for notify() calls.
Upon the appropriate notification, the View object will populate the most up to date information.
Because implementing a GUI was not possible within the timeframe, the information is displayed via Windows Dialog popup window.

Setup:
Starting a new game creates all game objects creating using settings determined from 4 input files (for players, plants, market and cities)
Loading a game creates all game objects using information from a previous game saved to file
The game is saved automatically after phase 5 of game play. Currently the files are saved as player.txt, plants.txt, cities.txt and market.txt.
To be loaded, they must be changed to player_Saved.txt, plants_Saved.txt, map_Saved.txt and market_Saved.txt. We kept them separate to preserve our text cases
The game will always begin at Phase 1, as the previous saved states are based on the end of Phase 5
The current game Step is stored in an input file and the current Step is set before the game begins.

Game Play:
The game currently implements 5 phases
Phase 1 is used to determine player order based on the game rules
Phase 2 is used to auction power plants, giving each player a chance to bid or pass on making a bid
Phase 3 is used to purchase as many resources as the player can afford and has power plant space to store
Phase 4 is used to purchase a house in a city. The player can purchase an initial house anywhere, and subsequent houses must be purchased adjacent to a currently owned house
Phase 5 powers cities and awards Elektros based on the game rules, ressupplies the market depending on the number of players and step of the game, and removes the strongest or weakest card from the market depending on the step
