#include "stage.h"
#include "game.h"
#include "input.h"
//#include "survivalActions.h"







stage::stage(Image& font):font(font)
{
	this->gameInstance = Game::instance;
	this->assetManagerInstance = assetManager::instance;
}


/*==============================================countdown===================================================*/


void countdownStage::updateTilePosition() {
	Vector2 pos = localChar.getPosition();
	tilePos = Vector2ub(pos.x / cellSize, (pos.y + gameInstance->y_displ) / cellSize);
	//calculate position in local tile
	localTilePos = Vector2((((pos.x + gameInstance->x_displ)) - tilePos.x * cellSize) / cellSize, ((pos.y + gameInstance->y_displ) - tilePos.y * cellSize) / cellSize);
}

Vector2ub countdownStage::getTileAtPos(float x, float y) {
	return Vector2ub(x / cellSize, (y + gameInstance->y_displ) / cellSize);
}

Vector2ub countdownStage::getTileAtPos(Vector2 pos) {
	return Vector2ub(pos.x / cellSize, (pos.y + gameInstance->y_displ) / cellSize);
}

sCell& countdownStage::getCellAtPos(Vector2 pos) {
	Vector2ub tile = getTileAtPos(pos);
	return gameInstance->startMap.getCell(tile.x, tile.y);
}

sCell& countdownStage::getCellAtPos(float x, float y) {
	Vector2ub tile = getTileAtPos(x, y);
	return gameInstance->startMap.getCell(tile.x, tile.y);
}




void countdownStage::renderMap(Image& framebuffer, float dx, float dy) {
	int cs = cellSize;


	/*int cellX = (ogDx) / cs;
	int cellY = (ogDy+13) / cs;

	std::cout << cellX << ", " << cellY << std::endl;*/

	dx -= (framebuffer.width / 2);
	dy -= (framebuffer.height / 2);

	Vector2ub tilePos = getTilePosition();
	int startX = max(0, tilePos.x - RENDER_X_CELLS);
	int startY = max(0, tilePos.y - RENDER_Y_CELLS);
	int endX = min(tilePos.x + RENDER_X_CELLS, gameInstance->startMap.width - 1);
	int endY = min(tilePos.y + RENDER_Y_CELLS, gameInstance->startMap.height - 1);
	//for every cell
	for (int x = startX; x <= endX; ++x)
		for (int y = startY; y <= endY; ++y)
		{
			//get cell info
			sCell& cell = gameInstance->startMap.getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = x * cs; //place offset here if you want
			int screeny = y * cs;
			screenx -= dx;
			screeny -= dy;

			if (tilePos.x == x && tilePos.y == y) {
				framebuffer.drawRectangle(screenx, screeny, cs, cs, Color::BLUE); 	//pos in screen
				}
			else
				//draw region of tileset inside framebuffer
				framebuffer.drawImage(tileset, 		//image
					screenx, screeny, 	//pos in screen
					area); 		//area
		}

}








void countdownStage::render(Image& framebuffer, float dx, float dy) {
	Vector2 charPos = localChar.getPosition();
	framebuffer.fill(gameInstance->bgcolor);
	renderMap(framebuffer, charPos.x, charPos.y);
	framebuffer.drawImage(sprite, (framebuffer.width / 2) - 9, (framebuffer.height / 2) - 13, localChar.isMoving ? ((int)std::round(gameInstance->totalTime * localChar.getSpeed() * .4) % 4) * 18 : 0, 27 * localChar.getDirection(), 18, 27);			//draws a scaled image
	//gameInstance->renderDebugGrid(framebuffer);

	gameInstance->uihandler.countdownUIObj.renderUI(framebuffer, font);
	gameInstance->showFramebuffer(&framebuffer);
}

void countdownStage::update(double seconds_elapsed) {
	Vector2& charPos = localChar.getPositionRef();
	float speed = localChar.getSpeed();
	//std::cout <<charPos.x<< " "<<charPos.y<<"\n";
	localChar.isMoving = false;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) { //if key up
		sCell cell = getCellAtPos(charPos.x, charPos.y - max(y_collisionDist, (speed * seconds_elapsed)));

		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.y -= speed * seconds_elapsed;
		}
		localChar.dir = UP;

	}
	else if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		sCell cell = getCellAtPos(charPos.x, charPos.y + max(y_collisionDist, (speed * seconds_elapsed)));
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.y += speed * seconds_elapsed;
		}
		localChar.dir = DOWN;
	}
	else if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) { //if key up
		sCell cell = getCellAtPos(charPos.x - max(x_collisionDist, (speed * seconds_elapsed)), charPos.y);
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.x -= speed * seconds_elapsed;
		}
		localChar.dir = LEFT;
	}
	else if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		sCell cell = getCellAtPos(charPos.x + max(x_collisionDist, (speed * seconds_elapsed)), charPos.y);
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.x += speed * seconds_elapsed;
		}
		localChar.dir = RIGHT;
	}
	else {

		localChar.dir = DOWN;
	}
	updateTilePosition();
	if (gameInstance->localTilePos.y <= .4) { //if player is close enough
		if (gameInstance->startMap.isCellItemType(gameInstance->tilePos.x, gameInstance->tilePos.y)) {
			gameInstance->uihandler.countdownUIObj.showPrompt("Space - Pick Item");
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
				gameInstance->invHandler.handInv.addItem(gameInstance->startMap.getCellItemType(gameInstance->tilePos.x, gameInstance->tilePos.y));
			}

		}
		else if (gameInstance->startMap.isCellExitType(gameInstance->tilePos.x, gameInstance->tilePos.y)) {
			gameInstance->uihandler.countdownUIObj.showPrompt("Space - Save Item");
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
				gameInstance->invHandler.handInv.dumpItemsToShip(gameInstance->invHandler.shipInv);
			}
		}
		else {
			gameInstance->uihandler.countdownUIObj.hidePrompt();
		}
	}
	else {
		gameInstance->uihandler.countdownUIObj.hidePrompt();
	}
	gameInstance->uihandler.countdownUIObj.updateCountdown(gameInstance->totalTime);
	gameInstance->uihandler.countdownUIObj.setIconSlotsFromVector(gameInstance->invHandler.handInv.getIconsToRender());
}

/*==============================================survival===================================================*/



void survivalStage::update(double seconds_elapsed) {
	
}


void survivalStage::renderBackground(Image& framebuffer) {
	
}


void survivalStage::renderSpaceShip(Image& framebuffer) {
	float shipYdispl = sin(gameInstance->totalTime)*10;
	float shipXdispl = cos(gameInstance->totalTime)*10;
	Image& shipBody = assetManagerInstance->getImage("data/shipBody.tga");
	Image& shipTail = assetManagerInstance->getImage("data/shipTail.tga");
	int posX = ((framebuffer.width / 2) - shipBody.width / 2)+shipXdispl;
	int posY = ((framebuffer.height/ 2) - shipBody.height/2) + shipYdispl;
	
	framebuffer.drawImage(shipBody,posX,posY);
}

void survivalStage::render(Image& framebuffer) {
	framebuffer.fill(gameInstance->bgcolor);
	
	
	
	gameInstance->showFramebuffer(&framebuffer);
}