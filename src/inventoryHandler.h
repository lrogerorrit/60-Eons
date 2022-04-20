#pragma once


enum class eItemType
{
	EMPTY,
	FOOD,
	WATER,
	MEDS,
	WEAPON,
};

struct handItemSlot {
	eItemType itemType= eItemType::EMPTY;
	
	
	
	bool isEmpty() {
		return this->itemType == eItemType::EMPTY;
	};
	
};

class inventoryHandler
{
public:
	inventoryHandler();
	
};

