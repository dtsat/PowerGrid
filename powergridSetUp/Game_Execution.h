#pragma once

#include "Map.h"
#include "setUp.h"


// Game functions
void start_menu();
void game_loop(Map &map);
void phase_1(Map &map);
void phase_2(Map &map);
void phase_3(Map &map);
void phase_4(Map &map);
void phase_5(Map &map);

// Helper Functions
std::vector<Player> turnOrderSort(std::vector<Player> players);
std::vector<Player> merge(std::vector<Player> players, std::vector<Player> left, std::vector<Player> right);
void displayPlantMarket(Map &map, std::vector<Plants> &availableMarket);
void displayPlayerInfo();
void displayMapInfo();
void quitGame();