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


	

class character {
    public:
		
        CHAR_COLOR characterColor= WHITE;
        Vector2 position;
        characterStatus status;
		bool isPlayerChar;
        bool isAlive = true;
		std::string name = names[rand() % 15];
		character(bool isPlayerChar = false, CHAR_COLOR characterColor = WHITE, Vector2 position = Vector2(0, 0)){
			this->isPlayerChar = isPlayerChar;
			this->characterColor = characterColor;
			this->position = position;
		};
		~character();

        inline characterStatus getStatus() { return status; };
        inline bool getIsAlive() { return isAlive; };
        inline Vector2 getPosition() { return position; };
        inline CHAR_COLOR getCharacterColor() { return characterColor; };
		inline void setCharacterColor(CHAR_COLOR color) { characterColor = color; };
		inline void setPosition(Vector2 pos) { position = pos; };
		inline void setStatus(characterStatus stat) { status = stat; };
		inline void setIsAlive(bool alive) { isAlive = alive; };
		

        

        
        

};

