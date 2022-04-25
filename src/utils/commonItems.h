#pragma once



#define randomInt() (rand()%10)
#define randomIntRange(min,max) (min + ( rand() % ( max - min + 1 ) ))
#define mapValue(x,minIn,maxIn,minOut,maxOut) ((x - minIn) * (maxOut - minOut) / (maxIn - minIn) + minOut);

const int MAX_PLANET_OPTIONS = 3;
const int MAX_PC_MENU = 3;
const int MAX_STAT_LEVEL = 5;

enum class holdingIcon { //Mainly used in uiHandler
	EMPTY,
	FOOD,
	WATER,
	MEDS,
	GUN,
	NA,

};

enum class eItemType //Mainly used in inventoryHandler
{
	EMPTY,
	FOOD,
	WATER,
	MEDS,
	WEAPON,
};

enum class stageType { //Mainly used in stage
	COUNTDOWN,
	SURVIVAL,
	PLANET_CHOOSING,
	PC,
	MULTIPLE_OPTIONS,
	MESSAGE,
	DUAL_OPTION,
	END,
	MENU,
	INTRO,
	TUTORIAL,
};

enum class ePcPage { //Mainly used in stage
	
	INVENTORY,
	CREW,
	PLANET
};


struct planetExplorationResults {
	bool wasDamaged=false;
	bool wasKilled=false;
	int damageReceived=0;
	int waterObtained=0;
	int foodObtained=0;
	int medsObtained=0;
};