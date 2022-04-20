#include "inventoryHandler.h"

void spaceShipInventory::addItem(eItemType itemType) {
	switch (itemType)
	{
	case eItemType::FOOD:
		this->foodCount++;
		break;

	case eItemType::WATER:
		this->waterCount++;
		break;

	case eItemType::MEDS:
		this->medsCount++;
		break;
	case eItemType::WEAPON:
		this->weaponCount++;
		break;
	}
}

void spaceShipInventory::removeItem(eItemType itemType) {
	switch (itemType)
	{
	case eItemType::FOOD:
		this->foodCount= max(this->foodCount-1,0);
		break;

	case eItemType::WATER:
		this->waterCount= max(this->waterCount - 1, 0);;
		break;

	case eItemType::MEDS:
		this->medsCount= max(this->medsCount - 1, 0);;
		break;
	case eItemType::WEAPON:
		this->weaponCount= max(this->weaponCount - 1, 0);;
		break;
	}
}

int spaceShipInventory::getItemCountOfType(eItemType itemType)
{
	switch (itemType)
	{
	case eItemType::FOOD:
		return this->foodCount;
		break;

	case eItemType::WATER:
		return this->waterCount;
		break;

	case eItemType::MEDS:
		return this->medsCount;
		break;
	case eItemType::WEAPON:
		return this->weaponCount;
		break;
	default:
		return -1;
		break;
	}
}

std::vector<holdingIcon> handInventory::getIconsToRender()
{
	std::vector<holdingIcon> toReturn;
	for (int i = 0; i < 3; i++)
	{
		holdingIcon icon= (holdingIcon&) itemSlots[i]; //can convert since int values are the same in both enums
		
		toReturn.push_back(icon);
	}
	return toReturn;
}

void handInventory::addItem(eItemType itemType) {
	std::cout << "hi\n";
	if (this->canAddItem()) {
		this->itemSlots[this->populatedSpots] = itemType;
		this->populatedSpots++;
		
	}
}

void handInventory::dumpItemsToShip(spaceShipInventory& shipInventory) {
	for (int i = 0; i < 3; i++) {
		if (itemSlots[i] != eItemType::EMPTY) {
			shipInventory.addItem(itemSlots[i]);
			itemSlots[i] = eItemType::EMPTY;
		}
	}
	this->populatedSpots = 0;
}