#pragma once
#include "game.h"

enum class eplanetType {
	EARTHLIKE,
	DESERT,
};

enum class eViolenceLevel
{
	LOW,
	MID,
	HIGH,
};

struct planetStats {
	int waterLevel;
	int foodLevel;
	int medLevel;
	eViolenceLevel violenceLevel;
	

	planetStats();
};

struct planetData {
	std::string name;
	eplanetType type;
	int travelDays;
	planetStats stats;
	int maxDaysInPlanet;

	planetData();
};


class survivalActions
{

	
private:
	Game* gameInstance;
	characterHandler& charHandler;
	inventoryHandler& invHandler;
	int traveledDays = 0;
	int daysAtPlanet = 0;
	bool isDepartingPlanet = true;
	bool atPlanet = false;

	planetData* targetPlanet=nullptr;

	std::vector<planetData> possibleTargetList;
	
	int totalDays = 0;
public:
	
	void advanceToNextDay();
	void choosePlanet(int planetPos);
	void generatePossibleTargets();
	void consumeItem(int plNum, eItemType type);
	void explorePlanet(int plNum, planetData& pData, bool hasGun);
	
	survivalActions(characterHandler& charHandler, inventoryHandler& invHandler):charHandler(charHandler), invHandler(invHandler) {
		this->gameInstance = Game::instance;
		
	};
	
	std::vector<planetData>& getPlanetData() {
		return possibleTargetList;
	}
	planetData* getTargetPlanetData() {
		return this->targetPlanet;
	}
	
};

