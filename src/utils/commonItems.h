#pragma once

#define randomInt() (rand()%10)
#define randomIntRange(min,max) (min + ( rand() % ( max - min + 1 ) ))

const int MAX_PLANET_OPTIONS = 3;

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
	MENU,
	TUTORIAL,
	END,
	MESSAGE,
};