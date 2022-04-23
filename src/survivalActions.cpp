#include "survivalActions.h"

const int MAX_PLANET_OPTIONS = 4;

void survivalActions::advanceToNextDay()
{
	//go through players and update their resources
	for (character& plChar : charHandler.getCharacters()) {
		if (!plChar.isAlive) continue;
		plChar.updateResources();
	}
	this->totalDays++;
	if (this->atPlanet) {
		this->daysAtPlanet++;
		if (this->daysAtPlanet >= this->targetPlanet->maxDaysInPlanet) {
			this->atPlanet = false;
			this->isDepartingPlanet = true;
			this->daysAtPlanet = 0;
			
		}
	}
	else {
		if (this->traveledDays >= this->targetPlanet->travelDays ) {
			this->atPlanet = true;
			this->traveledDays = 0;
		}
		else {
			this->traveledDays++;
		}
	}
}

void survivalActions::choosePlanet(int planetPos)
{
	this->targetPlanet = &this->possibleTargetList[planetPos];
}

void survivalActions::generatePossibleTargets()
{
	this->possibleTargetList.clear();

	for (int i = 0; i < MAX_PLANET_OPTIONS; i++) {
		planetData plData;
		this->possibleTargetList.push_back(plData);
	}
	
}

void survivalActions::consumeItem(int plNum, eItemType type)
{
	charHandler.getCharacter(plNum).consumeItem((statusType)(int(type) - 1));
}

void survivalActions::explorePlanet(int plNum, planetData& pData, int gunNum)
{
}
