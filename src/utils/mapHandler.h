#pragma once

#include "../includes.h"
#include <cassert>

//make an enum of itemtypes
typedef unsigned char uint8;

enum eDataType {
	WALKABLE=3,
	FOOD=6,
	WATER=7,
	WALL=3,
	MEDS=8,
	GUNS=4,
	EXIT=9,

};



enum eItemType:uint8 {
	NOTHING,
	SWORD,
	POTION
};




struct sCell {
	int type;
	eDataType dataType;
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
		gameMap(int w, int h, int layers = 1);
		
		sCell getCell(int x, int y) {
			return data[y*width+x];
		};
		void setCell(int x, int y, sCell cell) {
			data[y*width+x] = cell;
		};
		
		void setCellType(int x, int y, int type);
		void setDataType(int x, int y, int dataType);
		//void printMap();
		
		
		

};

