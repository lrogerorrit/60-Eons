#include "stage.h"
#include "game.h"
#include "input.h"
#include "survivalActions.h"







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
	this->renderMap(framebuffer, charPos.x, charPos.y);
	framebuffer.drawImage(sprite, (framebuffer.width / 2) - 9, (framebuffer.height / 2) - 13, localChar.isMoving ? ((int)std::round(gameInstance->totalTime * localChar.getSpeed() * .4) % 4) * 18 : 0, 27 * localChar.getDirection(), 18, 27);			//draws a scaled image
	//gameInstance->renderDebugGrid(framebuffer);

	gameInstance->uihandler.countdownUIObj.renderUI(framebuffer, font);
	//gameInstance->showFramebuffer(&framebuffer);
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
		if (gameInstance->startMap.isCellItemType(tilePos.x, tilePos.y)) {
			gameInstance->uihandler.countdownUIObj.showPrompt("Space - Pick Item");
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
				gameInstance->invHandler.handInv.addItem(gameInstance->startMap.getCellItemType(tilePos.x, tilePos.y));
			}

		}
		else if (gameInstance->startMap.isCellExitType(tilePos.x, tilePos.y)) {
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


survivalStage::survivalStage(Image& font, Image& smallFont) : stage(font), smallFont(smallFont) {
	this->survivalActionHandler= new survivalActions();
}

void survivalStage::choosePlanet() {
	this->infoToFech = eNextCycleGetInfo::PLANETCHOICE;
	this->gameInstance->setActiveStage(stageType::PLANET_CHOOSING);
	planetChoosingStage* st = (planetChoosingStage*)this->gameInstance->getStageOfType(stageType::PLANET_CHOOSING);
	this->survivalActionHandler->generatePossibleTargets();
	this->survivalActionHandler->setIsDepartingPlanet(false);
	st->initStage(this->survivalActionHandler->getGeneratedPlanetData() , (int)this->type);
}

void survivalStage::advanceToNextDay() {
	this->survivalActionHandler->advanceToNextDay();
	if (this->survivalActionHandler->getIsDepartingPlanet())
		this->choosePlanet();
}

void survivalStage::update(double seconds_elapsed) {
	//fetch info from planet choice stage
	if (this->infoToFech == eNextCycleGetInfo::PLANETCHOICE) {
		planetChoosingStage* st = (planetChoosingStage*) this->gameInstance->getStageOfType(stageType::PLANET_CHOOSING);
		int chosenPlanetPos = st->getSelectedOption();
		this->survivalActionHandler->choosePlanet(chosenPlanetPos);
		std::cout << "Pl chose " << this->survivalActionHandler->getTargetPlanetData()->name << std::endl;
		
	}
	this->infoToFech= eNextCycleGetInfo::NONE;

	Vector2 mousePos(this->gameInstance->mapMousePosition());
	//print to console mousePos;
	//std::cout << mousePos.x << " " << mousePos.y << std::endl;
	if (mousePos.y > 87 && mousePos.y < 114) {
		if (mousePos.x > 17 && mousePos.x < 47) {
			this->iconTip = "Open Inventory";
			this->tipVisible = true;
		}
		else if (mousePos.x > 70 && mousePos.x < 90) {
			this->iconTip = "Open PC";
			this->tipVisible = true;
		}
		else if (mousePos.x > 115 && mousePos.x < 140) {
			this->iconTip = "Advance to next day";
			this->tipVisible = true;
			if (Input::wasMousePressed(0)) {
				this->advanceToNextDay();
			}
		}
		else
			this->tipVisible = false;
	}
	else
		this->tipVisible = false;

	

	//DEBUG KEYS BELOW
	if (!gameInstance->debugMode) return;
	if (Input::wasKeyPressed(SDL_SCANCODE_F1)) {
		this->choosePlanet();
	}
	else if (Input::wasKeyPressed(SDL_SCANCODE_F2))
		this->advanceToNextDay();
}


void survivalStage::renderBackground(Image& framebuffer) {
	
	Image& stars = assetManagerInstance->getImage("data/starBackground.tga");
	Image& starsB = assetManagerInstance->getImage("data/starBackground2.tga");
	int displacement = survivalActionHandler->isAtPlanet()?0: fmod((gameInstance->totalTime*20),framebuffer.width);
	int displacement2 = survivalActionHandler->isAtPlanet() ? 0 : fmod((gameInstance->totalTime*18), framebuffer.width);
	framebuffer.drawImage(starsB, -displacement2, 0);
	framebuffer.drawImage(starsB, framebuffer.width - displacement2, 0);
	framebuffer.drawImage(stars,-displacement, 0);
	framebuffer.drawImage(stars, framebuffer.width-displacement, 0);
	
}

void survivalStage::renderSpaceShip(Image& framebuffer) {
	float shipYdispl = sin(gameInstance->totalTime*1.46465)*5;
	float shipXdispl = abs(cos(gameInstance->totalTime)*5);
	Image& shipBody = assetManagerInstance->getImage("data/shipBody.tga");
	Image& shipTail = assetManagerInstance->getImage("data/shipTail.tga");
	int posX = ((framebuffer.width / 2) - shipBody.width / 2)+shipXdispl;
	int posY = ((framebuffer.height/ 2) - shipBody.height/2) + shipYdispl;
	int tim= (int)std::round(gameInstance->totalTime*10);
	int extraDispl = (tim % 2);
	
	framebuffer.drawImage(shipTail, posX+extraDispl, posY);
	framebuffer.drawImage(shipBody,posX,posY);
}

void survivalStage::renderInfo(Image& framebuffer) {
	std::string remainingText = this->survivalActionHandler->isAtPlanet()?"Days left at planet: ":"Days to planet: ";
	framebuffer.drawText("Day " + std::to_string(this->survivalActionHandler->getTotalDays()),2,2,this->font);
	framebuffer.drawText(remainingText + std::to_string(this->survivalActionHandler->getRemainingDays()), 2, 12, this->smallFont,4,6);
	if (this->tipVisible)
		framebuffer.drawText(this->iconTip, framebuffer.width - (4 * this->iconTip.length()+2),10, this->smallFont, 4, 6);
}

void survivalStage::renderUI(Image& framebuffer) {
	Image& inventoryIcon = assetManagerInstance->getImage("data/icons/inventory.tga");
	Image& pcIcon = assetManagerInstance->getImage("data/icons/pc.tga");
	Image& advanceDayIcon= assetManagerInstance->getImage("data/icons/advanceDay.tga");
	
	framebuffer.drawImage(inventoryIcon, framebuffer.width*.2 - inventoryIcon.width / 2, (framebuffer.height * .85) - inventoryIcon.height / 2);
	framebuffer.drawImage(advanceDayIcon, framebuffer.width * .8 - advanceDayIcon.width / 2, (framebuffer.height * .85) - advanceDayIcon.height / 2);
	framebuffer.drawImage(pcIcon, framebuffer.width *.5 - pcIcon.width / 2, (framebuffer.height * .85) - pcIcon.height / 2);
	
}

void survivalStage::render(Image& framebuffer) {
	framebuffer.fill(gameInstance->bgcolor);
	renderBackground(framebuffer);
	renderSpaceShip(framebuffer);
	renderUI(framebuffer);
	renderInfo(framebuffer);
	//gameInstance->showFramebuffer(&framebuffer);
}


/*==============================================planetSelection===================================================*/

planetChoosingStage::planetChoosingStage(Image& font, Image& secondFont):stage(font), secondFont(secondFont)
{
	//TODO Remove since this is used whilst testing
	this->planetsData.reserve(MAX_PLANET_OPTIONS);
	
}

void planetChoosingStage::renderPlanetCard(Image& framebuffer, planetData* planData, int pos) {
	std::string iconName = "data/planets" + std::to_string((int)planData->type);
	iconName = iconName + ".tga";
	Image& planetCard = assetManagerInstance->getImage("data/planetCard.tga");
	Image& stayTime = assetManagerInstance->getImage("data/daysStay.tga");
	Image& planetIcon = assetManagerInstance->getImage(iconName.c_str());
	Vector2 cardPos(framebuffer.width * .1, framebuffer.height * .025 + ((pos + 1) * framebuffer.height / 4));
	framebuffer.drawRectangle(cardPos.x, cardPos.y, framebuffer.width * .8, .8 * (framebuffer.height / 4), pos == this->selectedOption ? Color::GREEN : Color::WHITE);
	framebuffer.drawImage(planetCard, cardPos.x, cardPos.y);

	

	if (planetIcon.width == 88 && planetIcon.height == 22)
		framebuffer.drawImage(planetIcon, cardPos.x + 3, cardPos.y + 1, 22 * planData->iconNum, 0, 22, 22);

	if (planData->stats.foodLevel > 0)
		for (int i = 0; i < planData->stats.foodLevel; i++)
			framebuffer.drawRectangle(cardPos.x + 36 + (4 * i), cardPos.y + 11, 3, 3, Color::YELLOW);
	if (planData->stats.waterLevel > 0)
		for (int i = 0; i < planData->stats.waterLevel; i++)
			framebuffer.drawRectangle(cardPos.x + 36 + (4 * i), cardPos.y + 18, 3, 3, Color::CYAN);
	if (planData->stats.medLevel > 0)
		for (int i = 0; i < planData->stats.medLevel; i++)
			framebuffer.drawRectangle(cardPos.x + 60 + (4 * i), cardPos.y + 11, 3, 3, Color::RED);

	for (int i = 0; i <= (int)planData->stats.violenceLevel; i++)
		framebuffer.drawRectangle(cardPos.x + 60 + (4 * i), cardPos.y + 18, 3, 3, Color::RED);
	framebuffer.drawText(std::to_string(planData->travelDays) + " days", cardPos.x + 95, cardPos.y + 10, this->font, 4, 6);
	if (stayTime.width == 15 && stayTime.height == 15)
		framebuffer.drawImage(stayTime, cardPos.x + 111, cardPos.y + 17, 0, 5 * (planData->maxDaysInPlanet - 1), 15, 5);
	framebuffer.drawText(planData->name, cardPos.x + 27, cardPos.y + 2, this->font, 4, 6);
}

void planetChoosingStage::render(Image& framebuffer)
{
	framebuffer.fill(gameInstance->bgcolor);
	
	
	framebuffer.drawText("F - Food", 5, 12, this->font,4,6);
	framebuffer.drawText("D - Drink", 5, 20, this->font,4,6);
	framebuffer.drawText("M - Meds", 45, 12, this->font, 4, 6);
	framebuffer.drawText("V - Violence", 45, 20, this->font, 4, 6);
	framebuffer.drawText("SPACE : Select", 100, 12, this->font, 4, 6);
	framebuffer.drawText("ARROWS: Change", 100, 20, this->font, 4, 6);

	framebuffer.drawText("Select Destination:", 16 , 1,this->secondFont);
	for (int i = 0; i < MAX_PLANET_OPTIONS; i++) {
		this->renderPlanetCard(framebuffer,  this->planetsData[i], i);
	}
}

void planetChoosingStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) //if key down
		this->selectedOption = min(MAX_PLANET_OPTIONS - 1, this->selectedOption + 1);
	else if (Input::wasKeyPressed(SDL_SCANCODE_UP)) //if key down
		this->selectedOption = max(0, this->selectedOption - 1);
	else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE))
		this->gameInstance->setActiveStage(this->fallbackStage);
}

void planetChoosingStage::initStage(std::vector<planetData>& newData, int fallbackStage)
{
	
	//copy contents of newData into planetsData
	if (this->planetsData.size() != MAX_PLANET_OPTIONS)
		for (int i = 0; i < MAX_PLANET_OPTIONS; i++) 
			this->planetsData.emplace_back(& newData[i]);
	else
		for (int i = 0; i < MAX_PLANET_OPTIONS; i++) 
			this->planetsData[i] = &newData[i];
		
	
	
	this->selectedOption = 0;
	this->fallbackStage = fallbackStage;
	
}


void pcStage::render(Image& framebuffer)
{
	switch (this->activePage) {
	case ePcPage::INVENTORY:
		this->renderInventoryPage(framebuffer);
		break;
	case ePcPage::CONSUME:
		this->renderConsumePage(framebuffer);
		break;
	case ePcPage::INFO:
		this->renderInfoPage(framebuffer);
	case ePcPage::PLANET:
		if (this->atPlanet) {
			this->renderPlanetPlanetPage(framebuffer);
		}
		else
			this->renderPlanetSpacePage(framebuffer);
		break;
	}

	framebuffer.drawImage(this->assetManagerInstance->getImage("data/icons/close.tga"),framebuffer.width*.8,5);
}

void pcStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) //if key down
		this->activePage = (ePcPage) (max(0, (int) this->activePage- 1));  
	else if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
		this->activePage = (ePcPage)min(MAX_PC_MENU - 1, (int) this->activePage + 1);


	switch (this->activePage) {
	case ePcPage::INVENTORY:
		this->updateInventoryPage();
		break;
	case ePcPage::CONSUME:
		this->updateConsumePage();
		break;
	case ePcPage::INFO:
		this->updateInfoPage();
	case ePcPage::PLANET:
		if (this->atPlanet) {
			this->updatePlanetPlanetPage();
		}
		else
			this->updatePlanetSpacePage();
		break;
	}
}

void pcStage::openPage(ePcPage page)
{
	this->activePage = page;
}
void pcStage::renderInventoryPage(Image& framebuffer)
{
}
void pcStage::renderConsumePage(Image& framebuffer)
{
}
void pcStage::renderInfoPage(Image& framebuffer)
{
}
void pcStage::renderPlanetSpacePage(Image& framebuffer)
{
}
void pcStage::renderPlanetPlanetPage(Image& framebuffer)
{
}
pcStage::pcStage(Image& font, Image& secondFont): stage(font), secondFont(secondFont)
{
	
}

void pcStage::updateInventoryPage()
{
	
}

void pcStage::updateConsumePage()
{
}

void pcStage::updateInfoPage()
{
}

void pcStage::updatePlanetSpacePage()
{
}

void pcStage::updatePlanetPlanetPage()
{
}

void pcStage::initStage(int fallbackStage, ePcPage pcPage, bool atPlanet)
{
	this->fallbackStage = fallbackStage;
	this->atPlanet = atPlanet;
	this->openPage(pcPage);
}