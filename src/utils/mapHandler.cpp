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
	width = height = 0;
	data = NULL;
}

gameMap::gameMap(int w, int h) {
	width = w;
	height = h;
	data = new sCell[w * h];
}

void gameMap::setCellType(int x, int y, eCellType type) {
	data[x + y * width].type = type;
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
	unsigned char* cells = new unsigned char[header.w * header.h];
	fread(cells, header.bytes, header.w * header.h, file);
	fclose(file); //always close open files
	//create the map where we will store it
	gameMap* map = new gameMap(header.w, header.h);

	for (int x = 0; x < map->width; x++)
		for (int y = 0; y < map->height; y++)
			map->setCellType(x, y, (eCellType) cells[x + y * map->width]);

	delete[] cells; //always free any memory allocated!

	return map;
}
