#include "characterHandler.h"
#include "utils/commonItems.h"


//characterHandler constructor

//
void characterHandler::makeCharacters(const int numCharacters)
{
	for (int i = 0; i < numCharacters; i++)
	{
		character newCharacter = new character(i==0,(CHAR_COLOR)i);
		characters.push_back(newCharacter);
	}
	this->numCharacters = numCharacters;
}

void character::updateResources()
{
	this->daysWithoutDrinking++;
	this->daysWithoutEating++;
	this->daysWithoutHealing++;
	
	if (!this->isAlive) return;
	
	if (this->daysWithoutDrinking > 2) {
		if (randomInt() >= randChance) {
			this->daysWithoutDrinking = 0;
			this->status.waterStat--;
		}
	}
	
	if (this->daysWithoutEating > 2) {
		if (randomInt() >= randChance) {
			this->daysWithoutEating = 0;
			this->status.foodStat--;
		}
		
	}

	if (this->daysWithoutHealing > 3) {
		if (randomInt() >= randChance) {
			this->daysWithoutHealing = 0;
			this->status.healthStat--;
		}
	}

	if (this->status.healthStat == 0||this->status.foodStat==0||this->status.waterStat==0)
		this->setIsAlive(false);
		//TODO: Set show message to character has died;
}

void character::consumeItem(statusType type)
{
	switch (type) {
		case statusType::FOOD:
			this->status.foodStat = min(this->status.foodStat + 1, 5);
			break;
		case statusType::WATER:
			this->status.waterStat= min(this->status.waterStat + 1, 5);
			break;
		case statusType::HEALTH:
			this->status.healthStat = min(this->status.healthStat + 1, 5);
			break;
	}
}