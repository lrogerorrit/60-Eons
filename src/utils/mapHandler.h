#pragma once

#include "../includes.h"
#include <cassert>

//make an enum of itemtypes
typedef unsigned char uint8;

enum eCellType:uint8 {
	EMPTY,
	START,
	WALL,
	DOOR,
	CHEST
};

enum eItemType:uint8 {
	NOTHING,
	SWORD,
	POTION
};




struct sCell {
	eCellType type;
	eItemType item;
	
};


class gameMap{
	public:
		int width;
		int height;
		sCell* data;
		gameMap* loadGameMap(const char* filename);
		gameMap();
		gameMap(int width, int height);
		~gameMap();
		sCell getCell(int x, int y);
		void setCell(int x, int y, sCell cell);
		void setCellType(int x, int y, eCellType type);
		void printMap();
		
		
		

};

