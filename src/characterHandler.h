#pragma once
#include "includes.h"
#include "framework.h"



enum CHAR_COLOR:int {
    WHITE,
    RED,
    BLUE,
    GREEN,
};

struct characterStatus {
    float foodStat = 1;
    float waterStat = 1;
};


const std::string names[] = {
	"Alfred",
	"Bert",
	"Cecil",
	"Dennis",
	"Eddie",
	"Freddie",
	"Gustav",
	"Hans",
	"Ike",
	"Jens",
	"Kurt",
	"Lars",
	"Morten",
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
    public:
		
		
        CHAR_COLOR characterColor= WHITE;
        Vector2 position;
        characterStatus status;
		bool isPlayerChar;
        bool isAlive = true;
		float movementSpeed = 15.0f;
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
		
		
        

        
        

};


//TODO: Make constructor to load characters from savefile

class characterHandler {
	public:
		std::vector<character> characters;
		int numCharacters;
		
		characterHandler() {};
		
		
		

		void makeCharacters(const int numCharacters);

		character& getCharacter(int index)
		{
			return characters[index];
		};
		std::vector<character>& getCharacters() {
			return characters;
		};
		
		
};

