#include "mapHandler.h"

struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};
//example of parser of .map from rogued editor

//make gameMap constructor

gameMap::gameMap() {
	this->width = this->height = 0;
	this->data = NULL;
}

gameMap::gameMap(int w, int h,int layers=1) {
	this->width = w;
	this->height = h;
	this->data = new sCell[w * h];
}

void gameMap::setCellType(int x, int y, int type) {
	this->data[x + y * width].type = type;
}

void gameMap::setDataType(int x, int y, int dataType) {
	this->data[x + y * width].dataType = (eDataType) dataType;
}

gameMap* gameMap::loadGameMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return NULL;

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);
	assert(header.bytes == 1); //always control bad cases!!


	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w * header.h*header.bytes];
	fread(cells, header.bytes, header.w * header.h, file);
	fclose(file); //always close open files
	//create the map where we will store it
	std::cout << header.w <<" "<< header.h << "\n";
	gameMap* map = this;//new gameMap(header.w, header.h);
	this->height = header.h;
	this->width = header.w;
	delete[] this->data;
	this->data=new sCell[header.w * header.h];

	for (int x = 0; x < map->width; x++)
		for (int y = 0; y < map->height; y++) {
			std::cout << x << " " << y << "\n";
			map->setCellType(x, y, (int) cells[(x*header.bytes) + y * map->width]);
			map->setDataType(x, y, (int)cells[(x * header.bytes) + y * map->width+1]);
		}

	delete[] cells; //always free any memory allocated!

	return map;
}
