#pragma once
#include <string>
#include "utils/commonItems.h"


class Game;


//list of random planet names
const std::string planetNames[10] = { "Pagnetune","Unenia","Nangomia","Gucurn","Kiutis","Luestea","Detupra","Zohucarro","Sadus 76T","Llyria 32E3" };


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
	

	planetStats() {
		violenceLevel = (eViolenceLevel)randomIntRange(0, 2);
		switch (violenceLevel) { //The higher the violence level, the more food and water the planet should have (to compensate)
		case eViolenceLevel::LOW:
			waterLevel = randomIntRange(0,2);
			foodLevel = randomIntRange(1, 2);
			medLevel = randomIntRange(0, 1);
			break;
		case eViolenceLevel::MID:
			waterLevel = randomIntRange(1, 3);
			foodLevel = randomIntRange(1, 3);
			medLevel = randomIntRange(0, 2);
			break;
		case eViolenceLevel::HIGH:
			waterLevel = randomIntRange(2, 3);
			foodLevel = randomIntRange(2, 3);
			medLevel = randomIntRange(1, 3);
			break;
		}			
	};
};

struct planetData {
	std::string name;
	eplanetType type;
	int iconNum;
	int travelDays;
	planetStats stats;
	int maxDaysInPlanet;

	planetData() {
		name= planetNames[randomInt()];		
		type = (eplanetType)randomIntRange(0, 1);
		iconNum = randomIntRange(0, 3);
		travelDays = randomIntRange(3, 10);
		stats = planetStats();
		maxDaysInPlanet = randomIntRange(1,3);
	};
};


class survivalActions{

	
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
	
	survivalActions():gameInstance(Game::instance), charHandler(Game::instance->getCharacterHandler()), invHandler(Game::instance->getInventoryHandler()) {};
	
	std::vector<planetData>& getPlanetData() {
		return possibleTargetList;
	}
	planetData* getTargetPlanetData() {
		return this->targetPlanet;
	}
	
	bool isAtPlanet() {
		return this->atPlanet;
	};
	bool getIsDepartingPlanet() {
		return this->isDepartingPlanet;
	};
	void setIsDepartingPlanet(bool val) {
		this->isDepartingPlanet = val;
	};
	
};

