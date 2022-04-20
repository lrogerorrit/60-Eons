#pragma once

#include "includes.h"
#include <vector>
#include "utils/commonItems.h"



/*
struct handItemSlot {
	eItemType itemType= eItemType::EMPTY;
	
	
	
	bool isEmpty() {
		return this->itemType == eItemType::EMPTY;
	};
	
};
*/




class spaceShipInventory {
	private:
		int foodCount = 0;
		int waterCount = 0;
		int medsCount = 0;
		int weaponCount = 0;
		
	public:
		spaceShipInventory() {};
		
		void addItem(eItemType itemType);
		void removeItem(eItemType itemType);
		
		int getItemCountOfType(eItemType itemType);
		std::vector<int> getItemCount() {
			std::vector<int> itemCounts;
			itemCounts.push_back(foodCount);
			itemCounts.push_back(waterCount);
			itemCounts.push_back(medsCount);
			itemCounts.push_back(weaponCount);
			return itemCounts;
		};
		
};


class handInventory {
private:
	int populatedSpots = 0;
	eItemType itemSlots[3];
public:

	handInventory() {
		for (int i = 0; i < 3; i++) {
			itemSlots[i] = eItemType::EMPTY;
		}
	}

	void addItem(eItemType itemType);
	void dumpItemsToShip(spaceShipInventory& shipInventory);
	std::vector<holdingIcon> getIconsToRender();
	//TODO: make function to get (with uiEnum) slot data;

	bool canAddItem() {return this->populatedSpots < 3;};
};

class inventoryHandler
{
public:
	
	spaceShipInventory shipInv;
	handInventory handInv;
	
	inventoryHandler() {};
	
};

