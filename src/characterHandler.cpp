#include "characterHandler.h"


//characterHandler constructor

//
void characterHandler::makeCharacters(const int numCharacters)
{
	for (int i = 0; i < numCharacters; i++)
	{
		character newCharacter = new character(i==0,(CHAR_COLOR)i);
		characters.push_back(newCharacter);
	}
}

