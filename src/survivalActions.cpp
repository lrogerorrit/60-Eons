#include "game.h"
#include "survivalActions.h"






void survivalActions::advanceToNextDay()
{

	this->hasVisitedPlanet = false;
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
	gameInstance->invHandler.shipInv.removeItem(type);
}

planetExplorationResults& survivalActions::explorePlanet(int plNum, planetData& pData, bool hasGun)
{
	this->hasVisitedPlanet=true;
	character& chosenChar = this->charHandler.getCharacter(plNum);
	assert(chosenChar.isAlive);
	planetStats planetInfo = this->targetPlanet->stats;
	int minHealthDamage, maxHealthDamage, damageProb, gunEfectivenes;
	planetExplorationResults results;
	switch (planetInfo.violenceLevel) {
	default:
	case eViolenceLevel::LOW:
		minHealthDamage = 1;
		maxHealthDamage = 1;
		damageProb = 3;
		gunEfectivenes = 10;
		break;
	case eViolenceLevel::MID:
		minHealthDamage = 1;
		maxHealthDamage = 3;
		damageProb = 5;
		gunEfectivenes = 7;
		break;
	case eViolenceLevel::HIGH:
		minHealthDamage = 2;
		maxHealthDamage = 4;
		damageProb = 7;
		gunEfectivenes = 5;
		break;


	}
	if (randomInt() < damageProb)
	{
		if ((!hasGun) || (hasGun && (randomInt() >= gunEfectivenes))) {
			int damage = minHealthDamage + (rand() % (maxHealthDamage - minHealthDamage));
			chosenChar.status.healthStat -= damage;
			results.wasDamaged = true;
			results.damageReceived = damage;
			if (chosenChar.status.healthStat <= 0) {
				results.wasKilled = true;
				chosenChar.status.healthStat = 0;
				chosenChar.isAlive = false;
				//TODO: Show message that player died
			}
		}
	}
	
	if (chosenChar.isAlive) {
		if (planetInfo.foodLevel > 0) {
			results.foodObtained = planetInfo.foodLevel;
			for (int i = 0; i < planetInfo.foodLevel; i++) {
				this->invHandler.shipInv.addItem(eItemType::FOOD);
			}
		}
		if (planetInfo.waterLevel > 0) {
			results.waterObtained= planetInfo.waterLevel;
			for (int i = 0; i < planetInfo.waterLevel; i++) {
				this->invHandler.shipInv.addItem(eItemType::WATER);
			}
		}
		if (planetInfo.medLevel > 0) {
			results.medsObtained = planetInfo.medLevel;
			for (int i = 0; i < planetInfo.medLevel; i++) {
				this->invHandler.shipInv.addItem(eItemType::MEDS);
			}
		}
	}
	return results;
}
