#include "stdafx.h"
#include "Map.h"
#include "setUp.h"
#include "mapView.h"
#include "marketView.h"
#include "playerView.h"
#include "plantsView.h"
#include "Observer.h"
#include "SaveBuilder.h"
#include "SaveLoadBuilder.h"
#include "SaveLoadDirector.h"


// Variables
bool gameOver = false;
int step = 1;
std::vector<Plants> availableMarket;

//viewers
mapView* mapViewer = new mapView;
marketView* marketViewer = new marketView;
playerView* playerViewer = new playerView;
plantsView* plantsViewer = new plantsView;

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
void displayPlantMarket(Map &map);
void displayPlayerInfo(Map &map);
void displayMapInfo(Map &map);
void quitGame();

// Allows players to load setup and start the game
void start_menu() {
	bool setupDone = false;
	int choice = 0;
	Map map;
	// Welcomes players to the game
	std::cout << "Welcome to Power Grid";
	// Loo until game starts
	while (choice != 2 || !setupDone) {
		if (choice != 2 || !setupDone)
			choice = 0;
		std::cout << "\n---------------------------"
			<< "\n1 - Game setup"
			<< "\n2 - Start game"
			<< "\n---------------------------"
			<< "\nPlease choose an option: ";
		while (choice < 1 || choice > 2) {
			std::cin >> choice;
			// Handles input exceptions
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
				choice = 0;
			}
			if (choice == 1) {
				// Takes a map passed by the setup
				map = setup_Map();
				// Sets burrent phase
				map.setCurrentPhase(1);
				step = map.getCurrentStep();
				map.attach(mapViewer);
				map.attach(playerViewer);
				map.attach(marketViewer);
				map.attach(plantsViewer);
				setupDone = true;
				std::cout << "\nSetup successfully loaded! The game is currently in Step " << map.getCurrentStep() << std::endl;
			}
			else if (choice == 2) {
				// Checks if setup has been loaded and starts the game
				if (setupDone)
					game_loop(map);
				else
					std::cout << "\nYou need to load a setup before starting a game" << std::endl;
			}
			else
				std::cout << "\nWrong option, please try again: ";
		}
	}
}

// Allows the game to be played with all its different phases.
void game_loop(Map &map) {
	int choice;
	char choice_2;
	std::srand(time(NULL));

	while (!gameOver || map.getCurrentPhase() != 1) {
		choice = 0;
		while (choice != 1) {
			if (choice != 1)
				choice = 0;
			std::cout << "\n---------------------------";
			if (map.getCurrentRound() == 0 && map.getCurrentPhase() == 1)
				std::cout << "\n1 - Start phase 1";
			else
				std::cout << "\n1 - Next Phase";
			std::cout << "\n2 - Display players information"
				<< "\n3 - Display map information"
				<< "\n4 - Quit game"
				<< "\n---------------------------"
				<< "\nPlease choose an option: ";
			while (choice < 1 || choice > 4) {
				std::cin >> choice;
				// Handles input exceptions
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore();
					choice = 0;
				}
				switch (choice) {
				case 1:
					break;
				case 2:
					displayPlayerInfo(map);
					break;
				case 3:
					displayMapInfo(map);
					break;
				case 4:
					std::cout << "\nGame only saves after phase 4, are you sure you want to quit? (Y/N): ";
					choice_2 = ' ';
					while (choice_2 != 'y' || choice_2 != 'Y' || choice_2 != 'n' || choice_2 != 'N') {
						std::cin >> choice_2;
						// handles input exceptions
						if (std::cin.fail()) {
							std::cin.clear();
							std::cin.ignore();
							choice_2 = ' ';
						}
						else if (choice_2 == 'y' || choice_2 == 'Y')
							quitGame();
						else if (choice_2 == 'n' || choice_2 == 'N')
							break;
						else {
							cout << "\nWrong option, try again: ";
							choice_2 = ' ';
						}
					}
					break;
				}
			}
		}

		switch (map.getCurrentPhase()) {
		case 1:
			std::cout << "\nPhase 1"
				<< "\n---------------------------"
				<< "\nDetermine Player Order"
				<< "\n---------------------------\n" << std::endl;
			system("pause");
			std::cout << "\n";
			phase_1(map);
			std::cout << "\n";
			system("pause");
			break;
		case 2:
			std::cout << "\nPhase 2"
				<< "\n---------------------------"
				<< "\nAuction Power Plants"
				<< "\n---------------------------\n" << std::endl;
			system("pause");
			std::cout << "\n";
			phase_2(map);
			break;
		case 3:
			std::cout << "\nPhase 3"
				<< "\n---------------------------"
				<< "\nBuy Resources"
				<< "\n---------------------------\n" << std::endl;
			system("pause");
			phase_3(map);
			break;
		case 4:
			std::cout << "\nPhase 4"
				<< "\n---------------------------"
				<< "\nBuilding"
				<< "\n---------------------------\n" << std::endl;
			system("pause");
			std::cout << "\n";
			phase_4(map);
			break;
		case 5:
			std::cout << "\nPhase 5"
				<< "\n---------------------------"
				<< "\nBureaucracy"
				<< "\n---------------------------\n" << std::endl;
			phase_5(map);
			break;
		}
		// Increase phase if it's not the last
		if (map.getCurrentPhase() < 5)
			map.setCurrentPhase(map.getCurrentPhase() + 1);
		else {
			// Increase round if it's the last
			map.setCurrentPhase(1);
			map.setCurrentRound(map.getCurrentRound() + 1);
		}
	}
}

// Determines player order depending on phase and round
void phase_1(Map &map) {
	std::vector<Player> playerRanking;
	// If first round and phase, shuffle the order
	if (map.getCurrentRound() == 0 && map.getCurrentPhase() == 1)
		map.shuffleOrder();
	else {
		// If it's not first round and phase, define order with city network and power plant value
		playerRanking = turnOrderSort(map.getPlayers());
		// Change the playerOrder vector according to the playerRanking vector
		for (int i = 0; i < map.playersSize(); i++)
			for (int j = 0; j < map.playersSize(); j++)
				if (playerRanking[i].getName() == map.getPlayerName(j))
					map.setPlayerOrder(i, j);
	}
	// Display player order
	for (int i = 0; i < map.playersSize(); i++) {
		std::cout << "Player " << i + 1 << ": " << map.getPlayerName(map.getPlayerOrder(i))
			<< " [" << map.getPlayerColour(map.getPlayerOrder(i)) << "]" << std::endl;
	}
}

// Auction power plants
void phase_2(Map &map) {
	char choice = ' ';
	int iterator_1, iterator_2, powerPlant, bid, maxBid, currentBidder;
	bool plantChosen, rightBid_1, rightBid_2, bidWon;
	std::vector<Player> availablePlayers;
	std::vector<Player> biddingPlayers;
	// Loop for all players
	for (int i = 0; i < map.playersSize(); i++) {
		// If not on the first round, ask each player if they wish to auction for a power plant
		if (map.getCurrentRound() > 0) {
			std::cout << "\n" << map.getPlayerName(map.getPlayerOrder(i))
				<< ", would you like to auction a power plant? (Y/N): ";
			while (choice != 'y' || choice != 'Y' || choice != 'n' || choice != 'N') {
				std::cin >> choice;
				// Handles input exceptions
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore();
					choice = ' ';
				}
				// If player wants to place a power plant, add them to available players vector, else do nothing
				if (choice == 'y' || choice == 'Y') {
					availablePlayers.push_back(map.getPlayer(map.getPlayerOrder(i)));
					break;
				}
				else if (choice == 'n' || choice == 'N')
					break;
				else
					std::cout << "\nWrong option, please try again: ";
			}
		}
		else
			availablePlayers.push_back(map.getPlayer(i));
	}
	std::cout << "\n";

	// Loop for all players
	iterator_1 = 0;
	int i;
	while (availablePlayers.size() > 0) {
		i = iterator_1 % map.playersSize();
		// Check if player is in players auctioning
		for (int m = 0; m < availablePlayers.size(); m++) {
			if (availablePlayers[m].getName() == map.getPlayerName(map.getPlayerOrder(i))) {
				// Display power plant market
				displayPlantMarket(map);
				// If player is auctioning, display their name and color
				std::cout << "\nPlayer " << i + 1 << ": " << map.getPlayerName(map.getPlayerOrder(i))
					<< " [" << map.getPlayerColour(map.getPlayerOrder(i)) << "], it's your turn" << std::endl;
				plantChosen = false;
				std::cout << "\nWhich plant do you chose to bid on: ";
				// Loop until a plant has been chosen
				while (!plantChosen) {
					std::cin >> powerPlant;
					// Handles input exceptions
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore();
						powerPlant = 0;
					}
					// Check if input is within existing plants, and if plant is in actual market
					if (powerPlant > map.getPlayerElektros(map.getPlayerOrder(i))) {
						std::cout << "\nYou do not have enough elektros to place that bid. Current elektros: "
							<< map.getPlayerElektros(map.getPlayerOrder(i))
							<< ", please try again: ";
					}
					else if (step != 3 && powerPlant >= 3 && powerPlant <= 50 && map.getPlantAtId(powerPlant).getStatus() == 0) {
						for (int j = 0; j < availableMarket.size(); j++)
							if (powerPlant == availableMarket[j].getValue())
								plantChosen = true;
						if (!plantChosen)
							std::cout << "\nChoose a plant in the actual market only, please try again: ";
					}
					else if (step == 3 && powerPlant >= 3 && powerPlant <= 50 && map.getPlantAtId(powerPlant).getStatus() == -1) {
						for (int j = 0; j < availableMarket.size(); j++)
							if (powerPlant == availableMarket[j].getValue())
								plantChosen = true;
						if (!plantChosen)
							std::cout << "\nChoose a plant in the actual market only, please try again: ";
					}
					else
						std::cout << "\nWrong option, please try again: ";
				}
				rightBid_1 = false;
				// Loop until first player's bid is available
				while (!rightBid_1) {
					// Asks player 1 how much money for bid
					std::cout << "\nHow much money would you like to bid (" << powerPlant << " or more): ";
					std::cin >> bid;
					// Handles input exceptions
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore();
						bid = 0;
					}
					if (bid > map.getPlayerElektros(map.getPlayerOrder(i))) {
						std::cout << "\nYou do not have enough elektros to place that bid. Current elektros: "
							<< map.getPlayerElektros(map.getPlayerOrder(i))
							<< ", please try again: ";
					}
					else if (bid >= powerPlant) {
						// If bid is greater, increase max Bid
						maxBid = bid;
						rightBid_1 = true;
					}
					else
						std::cout << "\nBid too small, please try again" << std::endl;
				}
				bidWon = false;
				// Empty the bidding players vector before looping with players wishing to bid
				biddingPlayers.erase(biddingPlayers.begin(), biddingPlayers.end());
				// Add all players available as bidding players
				for (int j = 0; j < availablePlayers.size(); j++)
					biddingPlayers.push_back(availablePlayers[j]);
				iterator_2 = i + 1;
				while (!bidWon) {
					currentBidder = iterator_2 % map.playersSize();
					// Check if only one player is bidding
					if (biddingPlayers.size() <= 1 || availablePlayers.size() == 1) {
						for (int k = 0; k < map.playersSize(); k++) {
							if (biddingPlayers[0].getName() == map.getPlayerName(k)) {
								// Try to add plant
								if (map.setPlayerPlant(k, powerPlant)) {
									// Change plant status to owned
									map.changePlantStatus(powerPlant);
									// Deduce player elektros
									map.setPlayerElektros(k, map.getPlayerElektros(k) - maxBid);
									std::cout << "\n" << biddingPlayers[0].getName() << " won the auction for Plant "
										<< powerPlant << " with a bid of " << maxBid << " elektros" << std::endl
										<< "He has " << map.getPlayerElektros(k) << " elektros after this purchase\n" << std::endl;
									// Remove player from players who can bid
									for (int n = 0; n < availablePlayers.size(); n++)
										if (biddingPlayers[0].getName() == availablePlayers[n].getName()) {
											availablePlayers.erase(availablePlayers.begin() + n);
											break;
										}
								}
								else {
									// Allow players to get rid of a power plant to take the new one
									int choice = 0;
									std::cout << "\n" << biddingPlayers[0].getName()
										<< " already has 3 power plants, you need to discard one" << std::endl
										<< "\nOwned Plants"
										<< "\n---------------------------\n";
									// Display all owned plants
									for (int l = 0; l < 3; l++) {
										std::cout << l + 1 << " - Power Plant " << biddingPlayers[0].getPlant(l).getValue()
											<< " | Powers " << biddingPlayers[0].getPlant(l).getCitiesPowered() << " cities"
											<< " | Consumes [" << biddingPlayers[0].getPlant(l).getCoalCost() << " coal, "
											<< biddingPlayers[0].getPlant(l).getOilCost() << " oil, "
											<< biddingPlayers[0].getPlant(l).getGarbCost() << " garbage, "
											<< biddingPlayers[0].getPlant(l).getUranCost() << " uranium]" << std::endl;
									}
									cout << "---------------------------\n";
									// Check if choice is within the ones displayed
									while (choice < 1 || choice > 3) {
										cout << "Which plant do you want to get rid of: ";
										cin >> choice;
										if (std::cin.fail()) {
											std::cin.clear();
											std::cin.ignore();
											choice = 0;
										}
										if (choice < 1 || choice > 3)
											cout << "\nWrong option, please try again\n" << endl;
										else {
											// Remove selected plant from player's plants
											map.removePlayerPlant(biddingPlayers[0].getPlayerNum(), choice - 1);
											// Add resources from discarded plant to the bank
											map.addBankResource(1, map.getPlayer(map.getPlayerOrder(biddingPlayers[0].getPlayerNum())).getPlant(choice - 1).getCoalHeld());
											map.addBankResource(2, map.getPlayer(map.getPlayerOrder(biddingPlayers[0].getPlayerNum())).getPlant(choice - 1).getOilHeld());
											map.addBankResource(3, map.getPlayer(map.getPlayerOrder(biddingPlayers[0].getPlayerNum())).getPlant(choice - 1).getGarbHeld());
											map.addBankResource(4, map.getPlayer(map.getPlayerOrder(biddingPlayers[0].getPlayerNum())).getPlant(choice - 1).getUranHeld());
											// Replace it with newly acquired plant
											map.setPlayerPlant(k, powerPlant);
											// Change plant status to owned
											map.changePlantStatus(powerPlant);
											// Deduce player's elektros
											map.setPlayerElektros(k, map.getPlayerElektros(k) - maxBid);
											std::cout << "\n" << biddingPlayers[0].getName() << " won the auction for Plant "
												<< powerPlant << " with a bid of " << maxBid << " elektros" << std::endl
												<< "He has " << map.getPlayerElektros(k) << " elektros after this purchase\n" << std::endl;
											// Remove player from players who can bid
											for (int n = 0; n < availablePlayers.size(); n++)
												if (biddingPlayers[0].getName() == availablePlayers[n].getName()) {
													availablePlayers.erase(availablePlayers.begin() + n);
													break;
												}
										}
									}
								}
								bidWon = true;
							}
						}
					}
					else {
						// Loop for all players who can bid
						for (int k = 0; k < biddingPlayers.size(); k++) {
							if (map.getPlayerName(map.getPlayerOrder(currentBidder)) == biddingPlayers[k].getName()) {
								std::cout << "\nPlayer " << currentBidder + 1 << ": " << map.getPlayerName(map.getPlayerOrder(currentBidder))
									<< " [" << map.getPlayerColour(map.getPlayerOrder(currentBidder)) << "], what is your bid (current: " << maxBid
									<< ", 0 to skip): ";
								rightBid_2 = false;
								while (!rightBid_2) {
									std::cin >> bid;
									// Handles input exceptions
									if (std::cin.fail()) {
										std::cin.clear();
										std::cin.ignore();
										bid = 1;
									}
									// Check if bid is greater than player's elektros
									if (bid > map.getPlayerElektros(map.getPlayerOrder(currentBidder))) {
										std::cout << "\nYou do not have enough elektros to place that bid. Current elektros: "
											<< map.getPlayerElektros(map.getPlayerOrder(currentBidder))
											<< "\nPlease try again: ";
									}
									// Check if bid exceeds the current best bid, replace it
									else if (bid > maxBid) {
										maxBid = bid;
										rightBid_2 = true;
										std::cout << "\n" << map.getPlayerName(map.getPlayerOrder(currentBidder))
											<< " [" << map.getPlayerColour(map.getPlayerOrder(currentBidder))
											<< "] currently has the highest bid: " << maxBid << " elektros" << std::endl;
									}
									// If bid is equal to 0, take the player out of bidding players
									else if (bid == 0) {
										rightBid_2 = true;
										for (int l = 0; l < biddingPlayers.size(); l++)
											if (biddingPlayers[l].getName() == map.getPlayerName(map.getPlayerOrder(currentBidder)))
												biddingPlayers.erase(biddingPlayers.begin() + l);
										std::cout << "\n" << map.getPlayerName(map.getPlayerOrder(currentBidder)) << " opted out of the bid" << std::endl;
									}
									else
										std::cout << "\nBid too small, please try again: ";
								}
								break;
							}
						}
					}
					iterator_2++;
				}
				break;
			}
		}
		iterator_1++;
	}
}

// Purchase resources from the market
void phase_3(Map &map) {
	bool resChoice, done, plantRoom;
	char choice;
	int resource;
	// Loops for all players
	for (int i = map.playersSize() - 1; i >= 0; i--) {
		resChoice = false;
		std::cout << "\nPlayer " << i + 1 << ": " << map.getPlayerName(map.getPlayerOrder(i))
			<< " [" << map.getPlayerColour(map.getPlayerOrder(i)) << "], it's your turn" << std::endl;
		// Loops until players says if he wants to purchase resources
		while (!resChoice) {
			std::cout << "\nDo you want to buy resources? (Y/N): ";
			std::cin >> choice;
			// handles input exceptions
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
				choice = ' ';
			}
			else if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
				resChoice = true;
		}
		done = false;
		// Loops until the player chooses to stop buying resources
		while (!done) {
			if (choice == 'y' || choice == 'Y') {
				std::cout << "\n---------------------------"
					<< "\nAvailable Resources"
					<< "\n---------------------------"
					<< std::setw(15) << std::left << "\n1 - Coal: ";
				// Display available coal
				for (int j = 1; j < 9; j++)
					if (map.getMarket().getCoal(j) > 0) {
						std::cout << map.getMarket().getCoal(j) << " x [0" << j << "$] | ";
					}
				std::cout << std::setw(15) << std::left << "\n2 - Oil: ";
				// Display available oil
				for (int j = 1; j < 9; j++)
					if (map.getMarket().getOil(j) > 0) {
						std::cout << map.getMarket().getOil(j) << " x [0" << j << "$] | ";
					}
				std::cout << std::setw(15) << std::left << "\n3 - Garbage: ";
				// Display available garbage
				for (int j = 1; j < 9; j++)
					if (map.getMarket().getGarbage(j) > 0) {
						std::cout << map.getMarket().getGarbage(j) << " x [0" << j << "$] | ";
					}
				std::cout << std::setw(15) << std::left << "\n4 - Uranium: ";
				// Display available uranium
				for (int j = 1; j < 17; j++)
					if (map.getMarket().getUranium(j) > 0) {
						std::cout << map.getMarket().getUranium(j) << " x [" << j << "$] | ";
					}
				std::cout << "\n---------------------------"
					<< "\nWhich resource would you like to purchase (0 to quit): ";
				std::cin >> resource;
				// handles input exceptions
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore();
					resource = -1;
				}
				// Checks if resource entered is within those available
				if (resource >= 0 && resource < 5) {
					// Checks for resource entered
					switch (resource) {
					case 0:
						done = true;
						break;
					case 1:
						plantRoom = true;
						// Loop for all market slots with possible coal
						for (int j = 1; j < 9; j++) {
							// Checks if slot contains resource
							if (map.getMarket().getCoal(j) > 0) {
								// Checks if player has enough elektros to buy resource
								if (map.getPlayerElektros(map.getPlayerOrder(i)) > j) {
									// Loops for all player's plant
									for (int k = 0; k < 3; k++) {
										// Checks if a plant can hold more of that resource
										if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getCoalHeld() < map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getCoalMax() && plantRoom) {
											// Removes the resource from the market slot
											map.removeResource(resource, j, 1);
											// Add that resource to the plant chosen
											map.addPlantResource(map.getPlayerOrder(i), k, resource, 1);
											map.setResources(map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue(), resource, 1);
											// Updates player's elektros
											map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) - j);
											std::cout << "\nAdded coal to plant " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue()
												<< ". It now holds " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getCoalHeld()
												<< " | max: " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getCoalMax()
												<< " | Your remaining elektros: " << map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
											plantRoom = false;
											break;
										}
										else if (k >= 2 && plantRoom) {
											std::cout << "\nYou do not have enough room in your power plants to buy this resource, transaction canceled" << std::endl;
											plantRoom = false;
										}
									}
								}
								else {
									std::cout << "You do not have enough elektros to purchase this resource. Current elektros: "
										<< map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
									break;
								}
							}
							else if (j >= 8)
								std::cout << "There is no coal left in the market, transaction canceled" << std::endl;
						}
						break;
					case 2:
						plantRoom = true;
						// Loop for all market slots with possible coal
						for (int j = 1; j < 9; j++) {
							// Checks if slot contains resource
							if (map.getMarket().getOil(j) > 0) {
								// Checks if player has enough elektros to buy resource
								if (map.getPlayerElektros(map.getPlayerOrder(i)) > j) {
									// Loops for all player's plant
									for (int k = 0; k < 3; k++) {
										// Checks if a plant can hold more of that resource
										if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getOilHeld() < map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getOilMax() && plantRoom) {
											// Removes the resource from the market slot
											map.removeResource(resource, j, 1);
											// Add that resource to the plant chosen
											map.addPlantResource(map.getPlayerOrder(i), k, resource, 1);
											map.setResources(map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue(), resource, 1);
											// Updates player's elektros
											map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) - j);
											std::cout << "\nAdded oil to plant " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue()
												<< ". It now holds " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getOilHeld()
												<< " | max: " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getOilMax()
												<< " | Your remaining elektros: " << map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
											plantRoom = false;
											break;
										}
										else if (k >= 2 && plantRoom) {
											std::cout << "\nYou do not have enough room in your power plants to buy this resource, transaction canceled" << std::endl;
											plantRoom = false;
										}
									}
								}
								else {
									std::cout << "You do not have enough elektros to purchase this resource. Current elektros: "
										<< map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
									break;
								}
							}
							else if (j >= 8)
								std::cout << "There is no oil left in the market, transaction canceled" << std::endl;
						}
						break;
					case 3:
						plantRoom = true;
						// Loop for all market slots with possible coal
						for (int j = 1; j < 9; j++) {
							// Checks if slot contains resource
							if (map.getMarket().getGarbage(j) > 0) {
								// Checks if player has enough elektros to buy resource
								if (map.getPlayerElektros(map.getPlayerOrder(i)) > j) {
									// Loops for all player's plant
									for (int k = 0; k < 3; k++) {
										// Checks if a plant can hold more of that resource
										if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getGarbHeld() < map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getGarbMax() && plantRoom) {
											// Removes the resource from the market slot
											map.removeResource(resource, j, 1);
											// Add that resource to the plant chosen
											map.addPlantResource(map.getPlayerOrder(i), k, resource, 1);
											map.setResources(map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue(), resource, 1);
											// Updates player's elektros
											map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) - j);
											std::cout << "\nAdded garbage to plant " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue()
												<< ". It now holds " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getGarbHeld()
												<< " | max: " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getGarbMax()
												<< " | Your remaining elektros: " << map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
											plantRoom = false;
											break;
										}
										else if (k >= 2 && plantRoom) {
											std::cout << "\nYou do not have enough room in your power plants to buy this resource, transaction canceled" << std::endl;
											plantRoom = false;
										}
									}
								}
								else {
									std::cout << "You do not have enough elektros to purchase this resource. Current elektros: "
										<< map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
									break;
								}
							}
							else if (j >= 8)
								std::cout << "There is no garbage left in the market, transaction canceled" << std::endl;
						}
						break;
					case 4:
						plantRoom = true;
						// Loop for all market slots with possible coal
						for (int j = 1; j < 17; j++) {
							// Checks if slot contains resource
							if (map.getMarket().getUranium(j) > 0) {
								// Checks if player has enough elektros to buy resource
								if (map.getPlayerElektros(map.getPlayerOrder(i)) > j) {
									// Loops for all player's plant
									for (int k = 0; k < 3; k++) {
										// Checks if a plant can hold more of that resource
										if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getUranHeld() < map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getUranMax() && plantRoom) {
											// Removes the resource from the market slot
											map.removeResource(resource, j, 1);
											// Add that resource to the plant chosen
											map.addPlantResource(map.getPlayerOrder(i), k, resource, 1);
											map.setResources(map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue(), resource, 1);
											// Updates player's elektros
											map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) - j);
											std::cout << "\nAdded uranium to plant " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue()
												<< ". It now holds " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getUranHeld()
												<< " | max: " << map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getUranMax()
												<< " | Your remaining elektros: " << map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
											plantRoom = false;
											break;
										}
										else if (k >= 2 && plantRoom) {
											std::cout << "\nYou do not have enough room in your power plants to buy this resource, transaction canceled" << std::endl;
											plantRoom = false;
										}
									}
								}
								else {
									std::cout << "You do not have enough elektros to purchase this resource. Current elektros: "
										<< map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
									break;
								}
							}
							else if (j >= 16)
								std::cout << "There is no uranium left in the market, transaction canceled" << std::endl;
						}
						break;
					}
				}
			}
			else
				done = true;
		}
	}
}

// Build houses on cities
void phase_4(Map &map) {
	char choice;
	int area, city;
	bool placingChoice, cityChosen, areaChosen;

	// Loop for every player from first to last
	for (int i = map.playersSize() - 1; i >= 0; i--) {
		placingChoice = false;
		std::cout << "Player " << i + 1 << ": " << map.getPlayerName(map.getPlayerOrder(i))
			<< " [" << map.getPlayerColour(map.getPlayerOrder(i)) << "], it's your turn" << std::endl;
		while (!placingChoice) {
			std::cout << "\nDo you wish to place houses? (Y/N): ";
			std::cin >> choice;
			// handles input exceptions
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
			}
			else if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
				placingChoice = true;
		}
		if (choice == 'y' || choice == 'Y') {
			areaChosen = false;
			if (map.getPlayerScore(map.getPlayerOrder(i)) <= 0 || map.getPlayerHouses(map.getPlayerOrder(i)) >= 22) {
				// Loop again until available area is chosen
				while (!areaChosen) {
					std::cout << "\nChoose an area to display cities: ";
					std::cin >> area;
					// Handles input exceptions
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore();
						area = 0;
					}
					for (int o = 0; o < map.areaSize(); o++) {
						if (area == map.getPlayableAreas(o)) {
							if (area >= 0 && area < map.getAreas()) {
								areaChosen = true;
								cityChosen = false;
								std::cout << "\n---------------------------"
									<< "\nAvailable cities in area " << area
									<< "\n---------------------------" << std::endl;
								// Display available cities in a given area
								for (int j = 0; j < map.citiesSize(); j++)
									if (map.getCityArea(j) == area && map.isCityAvailable(j))
										std::cout << map.getCityNum(j) << " - " << map.getCityName(j) << std::endl;
								std::cout << "---------------------------\n";
								// Loop again until available city is chosen
								while (!cityChosen) {
									std::cout << "Select a city to put your house on: ";
									std::cin >> city;
									// Handles input exceptions
									if (std::cin.fail()) {
										std::cin.clear();
										std::cin.ignore();
										city = -1;
									}
									// Checks if city entered is within available ones
									if (city > 0 && city < map.citiesSize() && map.isCityAvailable(city)) {
										// Checks if city entered is in area chosen
										if (map.getCityArea(city) == area) {
											// Checks if there are slots available on the city
											if (map.setCitySlot(city, map.getPlayerOrder(i))) {
												// Checks if player has enough elektros to place a house on this city
												if (map.getPlayerElektros(map.getPlayerOrder(i)) > 10) {
													// Increase player's score (number of houses)
													map.increasePlayerScore(map.getPlayerOrder(i));
													// Update player's elektros
													map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) - 10);
													cityChosen = true;
													std::cout << "\nHouse added on " << map.getCityName(city) << " for "
														<< map.getPlayerName(map.getPlayerOrder(i)) << ", whom now has "
														<< map.getPlayerHouses(i) << " houses left\n" << std::endl;
												}
												else
													std::cout << "You do not have enough elektros to buy place a house on this city. Current ekeltros: "
													<< map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
											}
											else
												std::cout << "\n" << map.getCityName(city) << " is already occupied by 3 players, try again\n" << std::endl;
										}
										else
											std::cout << "\nSelect a city in your area only, please try again\n" << std::endl;
									}
									else
										std::cout << "\nWrong option, please try again\n" << std::endl;
								}
							}
							else
								std::cout << "\nWrong option, please try again" << std::endl;
							break;
						}
					}
				}
			}
			else {
				bool found = false;
				cityChosen = false;
				std::cout << "\n---------------------------"
					<< "\nAvailable cities"
					<< "\n---------------------------" << std::endl;
				// Display available cities adjacent to a player
				vector<int> citiesDisplayed;
				for (int j = 0; j < map.citiesSize(); j++) {
					for (int k = 0; k < map.citiesSize(); k++) {
						if (!map.isCityOwned(j, map.getPlayerOrder(i)) && map.isCityOwned(k, map.getPlayerOrder(i))
							&& map.isAdjacent(j, k) && map.isCityAvailable(j) && map.getCityPlayable(j)
							&& !(std::find(citiesDisplayed.begin(), citiesDisplayed.end(), map.getCityNum(j)) != citiesDisplayed.end())) {
							std::cout << map.getCityNum(j) << " - " << map.getCityName(j) << std::endl;
							citiesDisplayed.push_back(map.getCityNum(j));
						}
					}
				}

				std::cout << "---------------------------\n";
				// Loop again until available city is chosen
				while (!cityChosen) {
					std::cout << "Select a city to put your house on: ";
					std::cin >> city;
					// Handles input exceptions
					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore();
						city = -1;
					}
					// Checks if city entered is within available ones
					if (city >= 0 && city < map.citiesSize() && map.isCityAvailable(city)) {
						found = false;
						for (int j = 0; j < map.citiesSize(); j++) {
							// Check if selected city is now currently owned
							if (!map.isCityOwned(city, map.getPlayerOrder(i))) {
								// Checks if city entered is adjacent to all owned cities
								if (map.isAdjacent(city, j) && map.isCityOwned(j, map.getPlayerOrder(i))) {
									// Check if there's still a slot left on the city
									if (map.setCitySlot(city, map.getPlayerOrder(i))) {
										// Increase player's score (number of houses)
										map.increasePlayerScore(map.getPlayerOrder(i));
										// Update player's elektros
										map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) - 10 + map.edgeCost(city, j));
										cityChosen = true;
										std::cout << "\nHouse added on " << map.getCityName(city) << " for " << map.getPlayerName(map.getPlayerOrder(i))
											<< " [" << map.getPlayerColour(map.getPlayerOrder(i)) << "], whom now has " << map.getPlayerHouses(i) << " houses left."
											<< " Current elektros: " << map.getPlayerElektros(map.getPlayerOrder(i)) << "\n" << std::endl;
										break;
									}
									else {
										std::cout << "\n" << map.getCityName(city)
											<< " is already occupied by 3 players, please try again\n" << std::endl;
										break;
									}
								}
								else if (j >= map.citiesSize() - 1) {
									std::cout << "\nCities are not adjacent, please try again\n" << std::endl;
									break;
								}
							}
							else {
								std::cout << "\nYou already own this city, please try again\n" << std::endl;
								break;
							}
						}
					}
					else
						std::cout << "\nWrong option, please try again\n" << std::endl;
				}
			}
		}
		else
			std::cout << "\n";
	}
	// Check conditions to increase step or trigger winning game conditions
	for (int i = 0; i < map.playersSize(); i++) {
		if (map.getPlayer(i).getScore() == 7 && step != 2) {
			step = 2;
			cout << "\nA player has built its 7th house. Now entering step 2" << endl;
			break;
		}
		else if (map.getPlayer(i).getScore() >= 17) {
			gameOver = true;
			cout << "A player has built its 17th house. The game will end next phase\n" << endl;
			break;
		}
	}
}

// Bureaucracy
void phase_5(Map &map) {
	// Loop for all players in order
	for (int i = 0; i < map.playersSize(); i++) {
		// Let the player know it is his turn
		int citiesOwned = 0, choice = -1, plantPower = 0, profits;
		std::cout << "\nPlayer " << i + 1 << ": " << map.getPlayerName(map.getPlayerOrder(i))
			<< " [" << map.getPlayerColour(map.getPlayerOrder(i)) << "], your turn to power cities"
			<< "\n---------------------------"
			<< "\nCities Owned"
			<< "\n---------------------------" << std::endl;

		// Display all cities owned by the player, add them to the total of cities they own
		for (int j = 0; j < map.citiesSize(); j++) {
			for (int k = 0; k < 3; k++) {
				if (map.getCitySlot(k, j) == map.getPlayerOrder(i)) {
					std::cout << map.getCityName(j) << std::endl;
					citiesOwned++;
					break;
				}
			}
		}
		// Display the amount o cities that can be powered by the player's power plants and resources
		std::cout << "---------------------------"
			<< "\nPlants power: ";
		for (int j = 0; j < map.getPlantsSize(); j++) {
			if (map.getPlayer(map.getPlayerOrder(i)).isPlantOwned(map.getPlant(j).getValue()) && map.getPlant(j).canPower())
				plantPower += map.getPlant(j).getCitiesPowered();
		}
		std::cout << plantPower
			<< "\n---------------------------" << std::endl;

		while (choice < 0 || choice > citiesOwned || choice > plantPower) {
			std::cout << "\nHow many cities would you like to power: ";
			std::cin >> choice;
			// Handles input exceptions
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
				choice = 0;
			}
			// Check if choice entered is correct
			if (choice < 0)
				std::cout << "\nEnter a number larger than or equal to 0, please try again";
			else if (choice > citiesOwned)
				std::cout << "\nYou don't have enough cities, please try again";
			else if (choice > plantPower)
				std::cout << "\nYou can't power that many cities with the powerplants/resources you own, please try again";
			else {
				// Set the player's total power attribute
				map.setPlayerPower(choice, map.getPlayerOrder(i));
				// Loop for all plants in map
				for (int j = 0; j < map.getPlantsSize(); j++) {
					// Check if plant is owned by player
					if (choice > 0 && map.getPlayer(map.getPlayerOrder(i)).isPlantOwned(map.getPlant(j).getValue())) {
						// Check if player can afford resource cost to produce power
						if (map.getPlant(j).getCoalCost() > 0 && map.getPlant(j).getCoalHeld() >= map.getPlant(j).getCoalCost()) {
							// Remove resource from plant powering from player
							map.substractResources(map.getPlant(j).getValue(), 1, map.getPlant(j).getCoalCost());
							// Remove resource from plant powering from map
							for (int k = 0; k < 3; k++) {
								if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue() == map.getPlant(j).getValue())
									map.removePlantResource(map.getPlayerOrder(i), k, 1, map.getPlant(j).getCoalCost());
							}
							std::cout << "\n" << map.getPlant(j).getCoalCost() << " coal removed from plant " << map.getPlant(j).getValue()
								<< " | Left: " << map.getPlant(j).getCoalHeld();
						}
						// Check if player can afford resource cost to produce power
						if (map.getPlant(j).getOilCost() > 0 && map.getPlant(j).getOilHeld() >= map.getPlant(j).getOilCost()) {
							// Remove resource from plant powering from player
							map.substractResources(map.getPlant(j).getValue(), 2, map.getPlant(j).getOilCost());
							// Remove resource from plant powering from map
							for (int k = 0; k < 3; k++) {
								if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue() == map.getPlant(j).getValue())
									map.removePlantResource(map.getPlayerOrder(i), k, 2, map.getPlant(j).getOilCost());
							}
							std::cout << "\n" << map.getPlant(j).getOilCost() << " oil removed from plant " << map.getPlant(j).getValue()
								<< " | Left: " << map.getPlant(j).getOilHeld();
						}
						// Check if player can afford resource cost to produce power
						if (map.getPlant(j).getGarbCost() > 0 && map.getPlant(j).getGarbHeld() >= map.getPlant(j).getGarbCost()) {
							// Remove resource from plant powering from player
							map.substractResources(map.getPlant(j).getValue(), 3, map.getPlant(j).getGarbCost());
							// Remove resource from plant powering from map
							for (int k = 0; k < 3; k++) {
								if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue() == map.getPlant(j).getValue())
									map.removePlantResource(map.getPlayerOrder(i), k, 3, map.getPlant(j).getGarbCost());
							}
							std::cout << "\n" << map.getPlant(j).getGarbCost() << " garbage removed from plant " << map.getPlant(j).getValue()
								<< " | Left: " << map.getPlant(j).getGarbHeld();
						}
						// Check if player can afford resource cost to produce power
						if (map.getPlant(j).getUranCost() > 0 && map.getPlant(j).getUranHeld() >= map.getPlant(j).getUranCost()) {
							// Remove resource from plant powering from player
							map.substractResources(map.getPlant(j).getValue(), 4, map.getPlant(j).getUranCost());
							// Remove resource from plant powering from map
							for (int k = 0; k < 3; k++) {
								if (map.getPlayer(map.getPlayerOrder(i)).getPlant(k).getValue() == map.getPlant(j).getValue())
									map.removePlantResource(map.getPlayerOrder(i), k, 4, map.getPlant(j).getUranCost());
							}
							std::cout << "\n" << map.getPlant(j).getUranCost() << " uranium removed from plant " << map.getPlant(j).getValue()
								<< " | Left: " << map.getPlant(j).getUranHeld() << std::endl;
						}
					}
				}
				// Check how much money the player makes from powering their cities
				switch (choice) {
				case 0: profits = 10; break;
				case 1: profits = 22; break;
				case 2: profits = 33; break;
				case 3: profits = 44; break;
				case 4: profits = 54; break;
				case 5: profits = 64; break;
				case 6: profits = 73; break;
				case 7: profits = 82; break;
				case 8: profits = 90; break;
				case 9: profits = 98; break;
				case 10: profits = 105; break;
				case 11: profits = 112; break;
				case 12: profits = 118; break;
				case 13: profits = 124; break;
				case 14: profits = 129; break;
				case 15: profits = 134; break;
				case 16: profits = 138; break;
				case 17: profits = 142; break;
				case 18: profits = 145; break;
				case 19: profits = 148; break;
				case 20: profits = 150; break;
				}

				map.setPlayerElektros(map.getPlayerOrder(i), map.getPlayerElektros(map.getPlayerOrder(i)) + profits);
				std::cout << "\nYou just made " << profits << " elektros by powering " << choice << " cities | Total: "
					<< map.getPlayerElektros(map.getPlayerOrder(i)) << std::endl;
			}
		}
	}

	// Check for winning conditions
	if (gameOver) {
		int max = 0;
		int winners[2];
		// Display all players score
		cout << "\nLast round is over, here's the score rundown"
			<< "\n---------------------------" << std::endl;
		for (int i = 0; i < map.playersSize(); i++) {
			cout << "Player " << i + 1 << " - " << map.getPlayerName(i)
				<< " | Cities powered: " << map.getPlayer(i).getCitiesPowered()
				<< " | Elektros: " << map.getPlayerElektros(i)
				<< " | Cities owned: " << map.getPlayerScore(i) << endl;
		}
		std::wcout << "---------------------------\n";
		// Evaluate which player is the winner
		for (int i = 0; i < map.playersSize(); i++) {
			// Compare cities powered
			if (map.getPlayer(i).getCitiesPowered() == max) {
				winners[1] = i;
				max = 0;
				// If tied, compare elektros
				if (map.getPlayerElektros(winners[0]) == map.getPlayerElektros(winners[1])) {
					// If tied, compare cities owned
					if (map.getPlayerScore(winners[0]) == map.getPlayerScore(winners[1])) {
						std::cout << "\nThere is a tie between " << map.getPlayer(winners[0]).getName()
							<< " and " << map.getPlayerName(winners[1]) << ". Congratulation!";
						quitGame();
					}
					else if (map.getPlayerScore(winners[0]) > map.getPlayerScore(winners[1])) {
						std::cout << "\nThe winner is " << map.getPlayer(winners[0]).getName() << ". Congratulation!";
						quitGame();
					}
					else {
						std::cout << "\nThe winner is " << map.getPlayer(winners[1]).getName() << ". Congratulation!";
						quitGame();
					}
				}
				else if (map.getPlayerElektros(winners[0]) > map.getPlayerElektros(winners[1])) {
					std::cout << "\nThe winner is " << map.getPlayer(winners[0]).getName() << ". Congratulation!";
					quitGame();
				}
				else {
					std::cout << "\nThe winner is " << map.getPlayer(winners[1]).getName() << ". Congratulation!";
					quitGame();
				}
			}
			else if (max < map.getPlayer(i).getCitiesPowered()) {
				max = map.getPlayer(i).getCitiesPowered();
				winners[0] = i;
			}
		}
		std::cout << "\nThe winner is " << map.getPlayer(winners[0]).getName() << ". Congratulation!";
		quitGame();
	}

	std::cout << "\nReallocating resources from bank to market"
		<< "\n---------------------------";

	int quantities[5];
	// Set quantities of resources to be reallocated to different markets depending on
	// Number of players and step of the game
	switch (map.playersSize()) {
	case 2:
		switch (step) {
		case 1:
			quantities[1] = 3;
			quantities[2] = 2;
			quantities[3] = 1;
			quantities[4] = 1;
			break;
		case 2:
			quantities[1] = 4;
			quantities[2] = 2;
			quantities[3] = 2;
			quantities[4] = 1;
			break;
		case 3:
			quantities[1] = 3;
			quantities[2] = 4;
			quantities[3] = 3;
			quantities[4] = 1;
			break;
		}
		break;
	case 3:
		switch (step) {
		case 1:
			quantities[1] = 4;
			quantities[2] = 2;
			quantities[3] = 1;
			quantities[4] = 1;
			break;
		case 2:
			quantities[1] = 5;
			quantities[2] = 3;
			quantities[3] = 2;
			quantities[4] = 1;
			break;
		case 3:
			quantities[1] = 3;
			quantities[2] = 4;
			quantities[3] = 3;
			quantities[4] = 1;
			break;
		}
		break;
	case 4:
		switch (step) {
		case 1:
			quantities[1] = 5;
			quantities[2] = 3;
			quantities[3] = 2;
			quantities[4] = 1;
			break;
		case 2:
			quantities[1] = 6;
			quantities[2] = 4;
			quantities[3] = 3;
			quantities[4] = 2;
			break;
		case 3:
			quantities[1] = 4;
			quantities[2] = 5;
			quantities[3] = 4;
			quantities[4] = 2;
			break;
		}
		break;
	case 5:
		switch (step) {
		case 1:
			quantities[1] = 5;
			quantities[2] = 4;
			quantities[3] = 3;
			quantities[4] = 2;
			break;
		case 2:
			quantities[1] = 7;
			quantities[2] = 5;
			quantities[3] = 3;
			quantities[4] = 3;
			break;
		case 3:
			quantities[1] = 5;
			quantities[2] = 6;
			quantities[3] = 5;
			quantities[4] = 2;
			break;
		}
		break;
	case 6:
		switch (step) {
		case 1:
			quantities[1] = 7;
			quantities[2] = 5;
			quantities[3] = 3;
			quantities[4] = 2;
			break;
		case 2:
			quantities[1] = 9;
			quantities[2] = 6;
			quantities[3] = 5;
			quantities[4] = 3;
			break;
		case 3:
			quantities[1] = 6;
			quantities[2] = 7;
			quantities[3] = 6;
			quantities[4] = 3;
			break;
		}
		break;
	}

	// Add resources to corresponding markets
	for (int i = 1; i < 5; i++) {
		map.addResource(i, quantities[i]);
	}
	std::cout << "\n---------------------------\n";

	// Remove highest or lowest value Power plant from market depending on step of the game
	int index = 0, lowest = 100;
	for (int i = 0; i < map.plantsSize(); i++) {
		if (step != 3 && map.getPlant(i).getStatus() == 0) {
			index++;
		}
		else if (step == 3 && map.getPlant(i).getStatus() == -1) {
			if (lowest > map.getPlant(i).getValue()) {
				lowest = map.getPlant(i).getValue();
			}
		}
		if (step != 3 && index == 8 && map.getPlant(i).getStatus() == 0) {
			map.changePlantStatus(map.getPlant(i).getValue());
			map.changePlantStatus(map.getPlant(i).getValue());
			cout << "Removed plant " << map.getPlant(i).getValue() << " from the future market\n";
			break;
		}
	}
	if (step == 3) {
		for (int i = 0; i < map.plantsSize(); i++) {
			if (map.getPlant(i).getValue() == lowest) {
				map.changePlantStatus(map.getPlant(i).getValue());
				cout << "Removed plant " << map.getPlant(i).getValue() << " from the market\n";
			}
		}
	}
	SaveLoadDirector director;
	SaveBuilder* builder = new SaveBuilder;
	director.setBuilder(builder);
	director.buildMap(map);
	delete builder;
}

// Splits the players into different arrays in order to sort
std::vector<Player> turnOrderSort(std::vector<Player> players) {
	// Base case, if the distance between start/end and middle is 0
	if (players.size() == 1)
		return players;
	// Split the vector in 2
	std::vector<Player>::iterator middle = players.begin() + (players.size() / 2);
	std::vector<Player> left(players.begin(), middle);
	std::vector<Player> right(middle, players.end());

	// split left and right side
	left = turnOrderSort(left);
	right = turnOrderSort(right);

	// Sort both left and right side
	return merge(players, left, right);
}

// Rearranges players in correct order (number of cities and power plants value)
std::vector<Player> merge(std::vector<Player> players, std::vector<Player> left, std::vector<Player> right) {
	// Create return vector
	std::vector<Player> result;
	// Create right and left indexes
	unsigned l_it = 0;
	unsigned r_it = 0;

	// Check wether or not the indexes have been reached
	while (l_it < left.size() && r_it < right.size()) {
		if (left.at(l_it).getScore() == right.at(r_it).getScore()) {
			// If score is the same, compare plants value
			if (left.at(l_it).getBestPlant() > right.at(r_it).getBestPlant()) {
				result.push_back(left.at(l_it));
				l_it++;
			}
			else {
				result.push_back(right.at(r_it));
				r_it++;
			}
		}
		else {
			// Compare score if it's different
			if (left.at(l_it).getScore() > right.at(r_it).getScore()) {
				result.push_back(left.at(l_it));
				l_it++;
			}
			else {
				result.push_back(right.at(r_it));
				r_it++;
			}
		}
	}

	// Reorder the left vector
	while (l_it < left.size()) {
		result.push_back(left.at(l_it));
		l_it++;
	}

	// Reorder the right vector
	while (r_it < right.size()) {
		result.push_back(right.at(r_it));
		r_it++;
	}
	players = result;
	return players;
}

// Shows current and future power plant market
void displayPlantMarket(Map &map) {
	// Create a vector to keep track of displayed plants
	std::vector<int> plantDisplayed;
	// Empty the availableMarket vector to store new elements in it
	availableMarket.erase(availableMarket.begin(), availableMarket.end());
	// Loop for the market size
	for (int i = 0; i < 8; i++) {
		if (step != 3) {
			if (i == 0) {
				std::cout << "Actual market"
					<< "\n---------------------------" << std::endl;
			}
			if (i == 4) {
				std::cout << "---------------------------"
					<< "\nFuture Market"
					<< "\n---------------------------" << std::endl;
			}
			// Loop for all plants in the map
			for (int j = 0; j < map.getPlantsSize(); j++) {
				// Check if plant is available or it it has been displayed already
				if (map.getPlant(j).getStatus() == 0 && !(std::find(plantDisplayed.begin(), plantDisplayed.end(), map.getPlant(j).getValue()) != plantDisplayed.end())) {
					// Add the plant to available plants vector if it is in the actual market
					if (i < 4)
						availableMarket.push_back(map.getPlant(j));
					if (step != 3) {
						std::cout << "Power plant ";
						// Add a 0 in front of values smaller than 10 for display purposes
						if (map.getPlant(j).getValue() < 10)
							std::cout << "0" << map.getPlant(j).getValue();
						else
							std::cout << map.getPlant(j).getValue();
						// Display the value of the plant, cities powered, and resources consumed
						std::cout << " | Powers " << map.getPlant(j).getCitiesPowered() << " cities"
							<< " | Consumes [" << map.getPlant(j).getCoalCost() << " coal, "
							<< map.getPlant(j).getOilCost() << " oil, " << map.getPlant(j).getGarbCost()
							<< " garbage, " << map.getPlant(j).getUranCost() << " uranium]" << std::endl;
					}
					else
						cout << "Step 3 Card | Game is about to change |" << endl;
					// Add the plant to displayed plants vector
					plantDisplayed.push_back(map.getPlant(j).getValue());
					if (map.getPlant(j).getValue() == 99) {
						cout << "\nYou have just drawn the STEP 3 CARD! This is the start of the final step of the game\n"
							<< "\nCards previously discarded will be shuffled and added to the current market\n" << endl;
						step = 3;
						map.shufflePlants();
						displayPlantMarket(map);
					}
					break;
				}
			}
			if (i == 7) {
				std::cout << "---------------------------" << std::endl;
			}
		}
		else {
			if (i == 0) {
				std::cout << "Actual market"
					<< "\n---------------------------" << std::endl;
			}
			if (i < 6) {
				for (int j = 0; j < map.getPlantsSize(); j++) {
					// Check if plant is available or it it has been displayed already
					if (map.getPlant(j).getStatus() == -1 && !(std::find(plantDisplayed.begin(), plantDisplayed.end(), map.getPlant(j).getValue()) != plantDisplayed.end())) {
						// Add the plant to available plants vector if it is in the actual market
						availableMarket.push_back(map.getPlant(j));
						std::cout << "Power plant ";
						// Add a 0 in front of values smaller than 10 for display purposes
						if (map.getPlant(j).getValue() < 10)
							std::cout << "0" << map.getPlant(j).getValue();
						else
							std::cout << map.getPlant(j).getValue();
						// Display the value of the plant, cities powered, and resources consumed
						std::cout << " | Powers " << map.getPlant(j).getCitiesPowered() << " cities"
							<< " | Consumes [" << map.getPlant(j).getCoalCost() << " coal, "
							<< map.getPlant(j).getOilCost() << " oil, " << map.getPlant(j).getGarbCost()
							<< " garbage, " << map.getPlant(j).getUranCost() << " uranium]" << std::endl;
						// Add the plant to displayed plants vector
						plantDisplayed.push_back(map.getPlant(j).getValue());
						break;
					}
				}
			}
			if (i == 5) {
				std::cout << "---------------------------" << std::endl;
			}
		}
	}
}

// Displays players possesions
void displayPlayerInfo(Map &map) {
	map.notify(0);
}

// Displays map with cities and their adjacencies
void displayMapInfo(Map &map) {
	map.notify(1);
}

// Exits the game
void quitGame() {
	std::cout << "\nThank you for playing Power Grid!\n" << std::endl;
	// Deletes all heap allocated memory
	delete mapViewer;
	delete marketViewer;
	delete playerViewer;
	delete plantsViewer;
	exit(0);
}