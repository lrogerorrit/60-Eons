#pragma once

#include "../includes.h"
#include <cassert>

//make an enum of itemtypes
typedef unsigned char uint8;

enum eDataType {
	WALL=3,
	GUNS=4,
	WALKABLE=5,
	FOOD=6,
	WATER=7,
	MEDS=8,
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
	
	bool canEnter() {
		return dataType != eDataType::WALL;
	};
	
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
		
		sCell& getCell(int x, int y) {
			return data[y*width+x];
		};
		void setCell(int x, int y, sCell cell) {
			data[y*width+x] = cell;
		};
		eDataType& getCellDataType(int x, int y) {
			return data[y*width+x].dataType;
		};
		bool isCellItemType(int x, int y) {
			eDataType& cellData = this->getCellDataType(x, y);
			return cellData== eDataType::GUNS || cellData>=6 && cellData<9; 
		}
		bool isCellExitType(int x, int y) {
			return this->getCellDataType(x, y) == eDataType::EXIT;
		}
		
		void setCellType(int x, int y, int type);
		void setDataType(int x, int y, int dataType);
		//void printMap();
		
		
		

};

