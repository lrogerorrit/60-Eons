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
		gameMap* map;
		gameMap* loadGameMap(const char* filename);
		
		gameMap();
		gameMap(int width, int height);
		
		sCell getCell(int x, int y) {
			return data[y*width+x];
		};
		void setCell(int x, int y, sCell cell) {
			data[y*width+x] = cell;
		};
		
		void setCellType(int x, int y, eCellType type);
		//void printMap();
		
		
		

};

