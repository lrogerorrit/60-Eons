#pragma once

#define randomInt() (rand()%10)
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