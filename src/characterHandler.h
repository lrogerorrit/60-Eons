#pragma once
#include "includes.h"
#include "framework.h"

enum class statusType {
	FOOD,
	WATER,
	HEALTH
};

enum CHAR_COLOR:int {
    WHITE,
    RED,
    BLUE,
    GREEN,
};

struct characterStatus {
    int foodStat = 5;
    int waterStat = 5;
	int healthStat = 5;
};


const std::string names[] = {
	"Alex",
	"Roger",
	"Cecil",
	"Denis",
	"Eddie",
	"Fred",
	"Gus",
	"Hans",
	"Ike",
	"Jens",
	"Kurt",
	"Lars",
	"Morty",
	"Nils",
	"Ole",
};

enum facingDirection :int
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
};
	

class character {

private:
		int daysWithoutEating = 0;
		int daysWithoutDrinking = 0;
		int daysWithoutHealing = 0;
		int randChance = 7;
	
    public:
		
		
        CHAR_COLOR characterColor= WHITE;
        Vector2 position;
        characterStatus status;
		bool isPlayerChar;
        bool isAlive = true;
		bool isMoving = true;
		
		float movementSpeed = 50.0f;
		facingDirection dir = DOWN;
		
		
		std::string name = names[rand() % 15];
		character(bool isPlayerChar = false, CHAR_COLOR characterColor = WHITE, Vector2 position = Vector2(0, 0)){
			this->isPlayerChar = isPlayerChar;
			this->characterColor = characterColor;
			this->position = position;
		};
		

        characterStatus getStatus() { return status; };
		characterStatus& getStatusRef() { return status; };
        bool getIsAlive() { return isAlive; };
        Vector2 getPosition() { return position; };
		Vector2& getPositionRef() { return position; };
        CHAR_COLOR getCharacterColor() { return characterColor; };
		void setCharacterColor(CHAR_COLOR color) { characterColor = color; };
		void setPosition(Vector2 pos) { position = pos; };
		
		void setStatus(characterStatus stat) { status = stat; };
		void setIsAlive(bool alive) { isAlive = alive; };
		float getSpeed() {return movementSpeed;	};
		facingDirection getDirection() {
			return dir;
		};
		
		void updateResources();
        void consumeItem(statusType type);

        
        

};


//TODO: Make constructor to load characters from savefile

class characterHandler {

	

public:
		std::vector<character> characters;
		int numCharacters=0;
		
		
		characterHandler() {
			srand(time(NULL));
		};
		
		
		

		void makeCharacters(const int numCharacters);

		character& getCharacter(int index)
		{
			return characters[index];
		};
		std::vector<character>& getCharacters() {
			return characters;
		};
		
		int getAliveCharacterNum() {
			int num = 0;
			for (int i = 0; i < characters.size(); i++) {
				if (characters[i].getIsAlive()) {
					num++;
				}
			}
			return num;
		};
		
};

