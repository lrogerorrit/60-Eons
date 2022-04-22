#pragma once
//#include "../game.h"
#include "utils/commonItems.h"
#include "includes.h"
#include "image.h"
#include "utils/mapHandler.h"


class Game;


enum class stageType {
	MENU,
	TUTORIAL,
	COUNTDOWN,
	SURVIVAL,
	END
};


class stage
{
protected:
	Game* gameInstance;
	Image& font;
	
public:
	stage(Image& font);
	virtual void render(Image& framebuffer) {};
	virtual void update(double seconds_elapsed) {};
	virtual void initStage() {};
};




class countdownStage :public stage
{
private:
	int cellSize;
	Vector2ub tilePos = Vector2ub(2, 2);
	Vector2 localTilePos = Vector2(0.0f, 0.0f);

	const int RENDER_X_CELLS = 3;
	const int RENDER_Y_CELLS = 3;
	const float x_collisionDist = 8.0f;
	const float y_collisionDist = 5.0f;
	Image& tileset;
	Image& sprite;
	character& localChar;
	
	
	void updateTilePosition();
	Vector2ub getTilePosition() { return tilePos; };
	Vector2ub getTileAtPos(float x, float y);
	Vector2ub getTileAtPos(Vector2 pos);

	sCell& getCellAtPos(Vector2 pos);

	sCell& getCellAtPos(float x, float y);
	void renderMap(Image& framebuffer, float dx, float dy);
public:
	
	countdownStage(Image& tileset, Image& sprite, Image& font, character& localChar) :stage(font) {
		this->tileset = tileset;
		this->sprite = sprite;
		this->localChar = localChar;
	}
	
	stageType type = stageType::COUNTDOWN;

	void render(Image& framebuffer, float dx, float dy);
	void update(double seconds_elapsed);
	void initStage(int cellSize);
	


};

