#include "stage.h"
#include "game.h"
#include "input.h"
#include "survivalActions.h"







stage::stage(Image& font):font(font)
{
	this->gameInstance = Game::instance;
	this->assetManagerInstance = assetManager::instance;
	
}

void stage::displayMessage(std::string msg, int fallbackStage)
{
	messageStage* st = (messageStage*)gameInstance->getStageOfType(stageType::MESSAGE);
	st->initStage(fallbackStage, msg);
	this->gameInstance->setActiveStage(stageType::MESSAGE);
}

void stage::displayDualOption(std::string msg, std::string op1, std::string op2, int fallbackStage)
{
	dualOptionStage* st = (dualOptionStage*)gameInstance->getStageOfType(stageType::DUAL_OPTION);
	st->initStage(fallbackStage, msg,op1,op2);
	this->gameInstance->setActiveStage(stageType::DUAL_OPTION);
}

void stage::displayMultipleOption(std::vector<std::string>& options, int fallbackStage)
{
	multipleOptionsStage* st = (multipleOptionsStage*)gameInstance->getStageOfType(stageType::MULTIPLE_OPTIONS);
	st->initStage(fallbackStage,options);
	this->gameInstance->setActiveStage(stageType::MULTIPLE_OPTIONS);
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

void survivalStage::openPc(ePcPage page) {
	pcStage* st = (pcStage*)this->gameInstance->getStageOfType(stageType::PC);
	st->initStage((int)this->type, page, this->survivalActionHandler->isAtPlanet());
	this->gameInstance->setActiveStage(stageType::PC);
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
			if (Input::wasMousePressed(0)) {
				this->openPc(ePcPage::INVENTORY);
			}
		}
		else if (mousePos.x > 70 && mousePos.x < 90) {
			this->iconTip = "Open PC";
			this->tipVisible = true;
			if (Input::wasMousePressed(0)) {
				this->openPc(ePcPage::CREW);
			}
			
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

void survivalStage::initStage()
{
	this->choosePlanet();
}

survivalActions& survivalStage::getSurvivalActions()
	{
		return *this->survivalActionHandler;
	}
int survivalStage::getTotalDays()
{
	return this->survivalActionHandler->getTotalDays();
}
void survivalStage::resetSurvivalActions()
{
	this->survivalActionHandler->reset();
}
;




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
		framebuffer.drawText(this->iconTip, framebuffer.width - (4 * this->iconTip.length()+2),2, this->smallFont, 4, 6);
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


/*==============================================pc===================================================*/


void pcStage::openPage(ePcPage page)
{
	this->activePage = page;
}
void pcStage::render(Image& framebuffer)
{
	framebuffer.fill(gameInstance->bgcolor);
	switch (this->activePage) {
	case ePcPage::INVENTORY:
		this->renderInventoryPage(framebuffer);
		break;
	
	case ePcPage::CREW:
		this->renderCrewPage(framebuffer);
		break;
	case ePcPage::PLANET:
		this->renderPlanetPage(framebuffer);
		break;
	}
	Image& tabText = this->assetManagerInstance->getImage("data/pcTabText.tga");
	for (int i=0; i < MAX_PC_MENU; i++) 
		framebuffer.drawRectangle(3 + (27 * i), 9, 27, 10, (ePcPage)i == this->activePage?Color::GREEN:Color::WHITE);
	
	framebuffer.drawImage(this->assetManagerInstance->getImage("data/icons/close.tga"),framebuffer.width-15,2);
	framebuffer.drawImage(this->assetManagerInstance->getImage("data/pcLayout.tga"), 0, 0);
	for (int i = 0; i < MAX_PC_MENU; i++)
		framebuffer.drawImage(tabText,4+(27*i),10,(i*25), ((ePcPage)i == this->activePage)?8:0,25,8);
	
	if (this->tipVisible)
		framebuffer.drawText(this->topTip, 2, 2, this->smallFont, 4, 6);
	
}
void pcStage::renderInventoryPage(Image& framebuffer)
{
	Image& foodIcon = this->gameInstance->uihandler.icons.getIconFromHoldingIcon(holdingIcon::FOOD);
	Image& waterIcon = this->gameInstance->uihandler.icons.getIconFromHoldingIcon(holdingIcon::WATER);
	Image& medsIcon = this->gameInstance->uihandler.icons.getIconFromHoldingIcon(holdingIcon::MEDS);
	Image& gunIcon = this->gameInstance->uihandler.icons.getIconFromHoldingIcon(holdingIcon::GUN);
	Vector2 displ(5, 23);
	framebuffer.drawImage(foodIcon, displ.x + 2, displ.y + 2);
	framebuffer.drawImage(waterIcon, displ.x + 2, framebuffer.height-40);
	framebuffer.drawImage(medsIcon, displ.x + 77, displ.y + 2);	
	framebuffer.drawImage(gunIcon, displ.x + 77, framebuffer.height - 40);
	framebuffer.drawText(std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::FOOD)),displ.x+foodIcon.width+10, -4+displ.y + 2+foodIcon.height/2,this->font);
	framebuffer.drawText(std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::WATER)), displ.x + foodIcon.width + 10, -4 + framebuffer.height - 40 + foodIcon.height / 2, this->font);
	framebuffer.drawText(std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::MEDS)), displ.x + foodIcon.width + 85, -4 + displ.y + 2 + foodIcon.height / 2, this->font);
	framebuffer.drawText(std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::WEAPON)), displ.x + foodIcon.width + 85, -4 + framebuffer.height - 40 + foodIcon.height / 2, this->font);
	
}

void pcStage::renderCrewCard(Image& framebuffer,int crewNum) {
	Vector2 displ(5+(74*(crewNum%2)+2), 23+(46*trunc(crewNum/2.0f))+1);
	character& crew = this->gameInstance->charHandler.getCharacter(crewNum);
	framebuffer.drawRectangle(displ.x, displ.y, 72, 45, (crewNum == this->selectedCard &&crew.isAlive) ? Color::GREEN : Color::WHITE);
	framebuffer.drawImage(this->assetManagerInstance->getImage("data/playerCard.tga"), displ.x, displ.y);

	framebuffer.drawText(crew.name, displ.x + 27, displ.y + 3, this->font);

	framebuffer.drawImage(this->assetManagerInstance->getImage("data/astronautBusts.tga"), displ.x + 3, displ.y + 3, (22 * crewNum), 0, 22, 22);

	if (crew.isAlive) {
		for (int i = 0; i < crew.status.foodStat; i++)
			framebuffer.drawRectangle(displ.x + 13 + (4 * i), displ.y + 29, 3, 3, Color::YELLOW);
		for (int i = 0; i < crew.status.waterStat; i++)
			framebuffer.drawRectangle(displ.x + 13 + (4 * i), displ.y + 36, 3, 3, Color::CYAN);
		for (int i = 0; i < crew.status.healthStat; i++)
			framebuffer.drawRectangle(displ.x + 48 + (4 * i), displ.y + 29, 3, 3, Color::RED);
	}
	else {
		framebuffer.drawImage(this->assetManagerInstance->getImage("data/deadOverlay.tga"), displ.x, displ.y);
	}
}

void pcStage::renderCrewPage(Image& framebuffer)
{
	Vector2 displ(5, 23);
	
	for (int i= 0; i < this->gameInstance->charHandler.numCharacters; i++) {
		this->renderCrewCard(framebuffer, i);
	}
	
}
void pcStage::renderPlanetPage(Image& framebuffer)
{
	Vector2 displ(5, 23);
	framebuffer.drawImage(this->assetManagerInstance->getImage("data/planetDataLayout.tga"), displ.x, displ.y);
	planetData* plData = this->survivalActionHandler->getTargetPlanetData();
	Image& planetIcon = this->assetManagerInstance->getImage("data/pcPlanets.tga");
	
	framebuffer.drawImage(planetIcon, displ.x + 4, displ.y + 4,(int) this->survivalActionHandler->getTargetPlanetData()->type * 32, 0, 32, 32);
	
	for (int i = 0; i < plData->stats.foodLevel; i++)
		framebuffer.drawRectangle(displ.x + 45 + (7 * i), displ.y + 40, 5, 4, Color::YELLOW);
	for (int i = 0; i < plData->stats.waterLevel; i++)
		framebuffer.drawRectangle(displ.x + 45 + (7 * i), displ.y + 50, 5, 4, Color::CYAN);
	for (int i = 0; i < plData->stats.medLevel; i++)
		framebuffer.drawRectangle(displ.x + 45 + (7 * i), displ.y + 60, 5, 4, Color::RED);
	for (int i = 0; i < (int) plData->stats.violenceLevel+1; i++)
		framebuffer.drawRectangle(displ.x + 45 + (7 * i), displ.y + 70, 5, 4, Color::RED);
	framebuffer.drawText(this->survivalActionHandler->getTargetPlanetData()->name, displ.x + 38, displ.y + 4, this->font);
	
}


void pcStage::goExplorePlanet(int plNum,bool hasGun=false) {
	
	planetExplorationResults& results = this->survivalActionHandler->explorePlanet(plNum, *this->survivalActionHandler->getTargetPlanetData(), hasGun);
	if (results.wasKilled)
		this->displayMessage("During the exploration\nthe astronaut got\n ambushed and was\n killed!\nYou got no resources.", (int)this->type);
	else {
		std::string msg = "";
		if(results.wasDamaged)
			msg+="You were ambushed and\nlost " + std::to_string(results.damageReceived) + " health.\n";
		if (results.foodObtained > 0 || results.medsObtained > 0 || results.waterObtained > 0) {
			msg += "You found:\n";
			int count = 0;
			if (results.foodObtained > 0) {
				count++;
				msg += std::to_string(results.foodObtained) + " food, ";
			}
			if (results.medsObtained > 0) {
				msg += std::to_string(results.medsObtained) + " meds, ";
				count++;
			}
			if (count == 2) msg += "\n";
			if (results.waterObtained > 0)
				msg += std::to_string(results.waterObtained) + " water.";
		}
		else
			msg += "You got no resources!";
		this->displayMessage(msg, (int)this->type);
	}
		
};

void pcStage::openDualOptions(std::string msg, std::string op1, std::string op2, eNextCycleGetInfoPC flagToSet) {
	this->infoToFech = flagToSet;
	this->displayDualOption(msg, op1, op2, (int)this->type);
}

void pcStage::update(double seconds_elapsed)
{
	bool dontResetFlag = false;
	switch (infoToFech) {
	case eNextCycleGetInfoPC::CREW_OPTIONS: {
		multipleOptionsStage* st = (multipleOptionsStage*)gameInstance->getStageOfType(stageType::MULTIPLE_OPTIONS);
		switch (st->getSelectedOption()) {
		case 1: //Food
			if (this->gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::FOOD) > 0) {
				if (this->gameInstance->charHandler.getCharacter(this->selectedCard).status.foodStat < MAX_STAT_LEVEL)
					this->survivalActionHandler->consumeItem(this->selectedCard, eItemType::FOOD);
				else
					this->displayMessage("Cant give food\nAstronaut is at max\nfood level!", (int)this->type);
			}
			else
				this->displayMessage("Cant give food\nNo food left!", (int)this->type);
			break;
		case 2://water
			if (this->gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::WATER) > 0) {
				if (this->gameInstance->charHandler.getCharacter(this->selectedCard).status.waterStat < MAX_STAT_LEVEL)
					this->survivalActionHandler->consumeItem(this->selectedCard, eItemType::WATER);
				else
					this->displayMessage("Cant give water\nAstronaut is at max\nwater level!", (int)this->type);
			}
			else
				this->displayMessage("Cant give water\nNo water left!", (int)this->type);
			break;
		case 3://meds
			if (this->gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::MEDS) > 0) {
				if (this->gameInstance->charHandler.getCharacter(this->selectedCard).status.healthStat < MAX_STAT_LEVEL)
					this->survivalActionHandler->consumeItem(this->selectedCard, eItemType::MEDS);
				else
					this->displayMessage("Cant heal\nAstronaut is at max\nhealth level!", (int)this->type);
			}
			else
				this->displayMessage("Cant heal\nNo meds left!", (int)this->type);
			break;
		case 4://explore planet
			if (this->survivalActionHandler->hasVisitedPlanetOnDay()) {
				this->displayMessage("Please wait until\ntomorrow to visit\nplanet again!",(int)this->type);
			}
			else {
				if (this->gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::WEAPON) > 0) {
					dontResetFlag = true;
					this->openDualOptions("Do you want to take\na gun to explore?", "Yes", "No", eNextCycleGetInfoPC::GUN_CONFIRM);

				}
				else
					goExplorePlanet(this->selectedCard);
			}
			break;

		}
		break;
	}
	case eNextCycleGetInfoPC::GUN_CONFIRM: {
		dualOptionStage* st = (dualOptionStage*)gameInstance->getStageOfType(stageType::DUAL_OPTION);
		goExplorePlanet(this->selectedCard, st->getSelectedOption() == 0);

		break;
		}
	}
	if(!dontResetFlag)
		this->infoToFech = eNextCycleGetInfoPC::NONE;
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) //if key down
		this->activePage = (ePcPage) (max(0, (int) this->activePage- 1));  
	else if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
		this->activePage = (ePcPage)min(MAX_PC_MENU - 1, (int) this->activePage + 1);

	this->tipVisible = false;
	switch (this->activePage) {
	case ePcPage::INVENTORY:
		this->updateInventoryPage();
		break;
	case ePcPage::CREW:
		this->updateCrewPage();
		break;
	case ePcPage::PLANET:
		if (this->atPlanet)
			this->updatePlanetPage();
		break;
	}

	Vector2 mousePos = this->gameInstance->mapMousePosition();
	if (mousePos.x > 145 && mousePos.x < 157 && mousePos.y>2 && mousePos.y < 14) {
		this->tipVisible = true;
		this->topTip = "Close PC";
		if (Input::wasMousePressed(0)) 
			this->gameInstance->setActiveStage(stageType::SURVIVAL);
		
	}
	
}

void pcStage::updateInventoryPage()
{
	
}

void pcStage::openCrewOptions(int crewNum) {
	this->infoToFech = eNextCycleGetInfoPC::CREW_OPTIONS;
	std::cout << "Opening Options for crew " << crewNum << std::endl;

	if (this->gameInstance->charHandler.getCharacter(crewNum).isAlive) {

		std::vector<std::string> optionsToSend;
		std::string foodNum = std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::FOOD)) + ")";
		std::string drinkNum = std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::WATER)) + ")";
		std::string medNum = std::to_string(gameInstance->invHandler.shipInv.getItemCountOfType(eItemType::MEDS)) + ")";

		optionsToSend.push_back("Close");
		optionsToSend.push_back("Give food (" + foodNum);
		optionsToSend.push_back("Give water (" + drinkNum);
		optionsToSend.push_back("Heal (" + medNum);
		if (this->atPlanet)
			optionsToSend.push_back("Explore Planet");
		this->displayMultipleOption(optionsToSend, (int)this->type);
		
	}

}

void pcStage::updateCrewPage()
{
	this->tipVisible = true;
	this->topTip = "Click on a Member to open options!";
	//Vector2 displ(5 + (74 * (crewNum % 2) + 2), 23 + (46 * trunc(crewNum / 2.0f)) + 1);
	this->selectedCard = -1;
	Vector2 mousePos = this->gameInstance->mapMousePosition();
	if (mousePos.x >7 && mousePos.x< 79&& mousePos.y>24 && mousePos.y <69 )
		this->selectedCard = 0;
	else if (mousePos.x >7 && mousePos.x<79 && mousePos.y>70 && mousePos.y <115)
		this->selectedCard = 2;
	else if (mousePos.x >81 && mousePos.x<153 && mousePos.y>24 && mousePos.y <69 )
		this->selectedCard = 1;
	else if (mousePos.x >81 && mousePos.x<153 && mousePos.y>70 && mousePos.y <115 )
		this->selectedCard = 3;
	

	if (Input::wasMousePressed(0) && this->selectedCard!=-1) {
		this->openCrewOptions(this->selectedCard);
	}

}

void pcStage::updatePlanetPage()
{
}



void pcStage::initStage(int fallbackStage, ePcPage pcPage, bool atPlanet)
{
	this->fallbackStage = fallbackStage;
	this->atPlanet = atPlanet;
	this->openPage(pcPage);
	
}
pcStage::pcStage(Image& font, Image& smallFont) : stage(font), smallFont(smallFont) {
	survivalStage* st = (survivalStage*)this->gameInstance->getStageOfType(stageType::SURVIVAL);
	this->survivalActionHandler=&st->getSurvivalActions();
}

/*==============================================Multiple Options===================================================*/

void multipleOptionsStage::renderOption(Image& framebuffer, std::string& name, int num) {
	Vector2 displ(16, 23 + (14 * num));
	framebuffer.drawRectangle(displ.x, displ.y, 128, 13, this->selectedOption == num ? Color::GREEN : Color::WHITE);
	framebuffer.drawRectangle(displ.x + 1, displ.y+1, 126, 11,gameInstance->bgcolor);
	framebuffer.drawText(name, framebuffer.width / 2 - (std::round((name.length()-.1) / 2) * 7)-3, displ.y + 2, this->font);
}

void multipleOptionsStage::render(Image& framebuffer)
{
	framebuffer.fill(gameInstance->bgcolor);
	framebuffer.drawRectangle(13, 20, 134, 90, Color::WHITE);
	framebuffer.drawRectangle(15, 22, 130, 86, gameInstance->bgcolor); 

	
	framebuffer.drawText("SPACE : Select", 5, 12, this->smallFont, 4, 6);
	framebuffer.drawText("ARROWS: Change", framebuffer.width-61, 12, this->smallFont, 4, 6);

	framebuffer.drawText("Choose Option:",34, 1, this->font);

	for (int i = 0; i < this->options.size(); i++) {
		this->renderOption(framebuffer, this->options[i], i);
	}
	
}

void multipleOptionsStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)|| Input::gamepads[0].didDirectionChanged(PAD_UP)) //if key down
		this->selectedOption = max(0, this->selectedOption - 1);
	else if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)|| Input::gamepads[0].didDirectionChanged(PAD_DOWN)) //if key down
		this->selectedOption = min(this->options.size() - 1, this->selectedOption + 1);
	else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)|| Input::gamepads[0].wasButtonPressed(A_BUTTON))
		this->gameInstance->setActiveStage(this->fallbackStage);

}

void multipleOptionsStage::initStage(int fallbackStage, std::vector<std::string>& options)
{
	this->selectedOption = 0;
	this->fallbackStage = fallbackStage;

	this->options.clear();
	for (std::string& op : options)
		this->options.push_back(op);
		

}


multipleOptionsStage::multipleOptionsStage(Image& font,Image& smallFont) :stage(font),smallFont(smallFont)
{
}

/*==============================================Message===================================================*/

void messageStage::render(Image& framebuffer)
{
	framebuffer.fill(gameInstance->bgcolor);
	std::istringstream iss(this->message);
	int yDispl = 20;
	for (std::string line; std::getline(iss, line);) {
		framebuffer.drawText(line, framebuffer.width / 2 - (std::round((line.length()-1) / 2) * 7) - 7, yDispl, this->font);
		yDispl += 12;
	}
	framebuffer.drawText("SPACE to continue", 19, framebuffer.height - 12, this->font);
	
}

void messageStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)|| Input::gamepads[0].wasButtonPressed(A_BUTTON))
		this->gameInstance->setActiveStage(this->fallbackStage);
}

void messageStage::initStage(int fallbackStage, std::string& message)
{
	this->fallbackStage = fallbackStage;
	this->message = message;
	
}

messageStage::messageStage(Image& font, Image& smallFont): stage(font), smallFont(smallFont)
{
}

/*==============================================Dual Option===================================================*/

void dualOptionStage::renderOption(Image& framebuffer, int num) {
	std::string opString = num == 0 ? this->option1 : this->option2;
	Vector2 displ(num == 0 ? 5 : 84,90);

	framebuffer.drawRectangle(displ.x, displ.y, 71, 15, this->selectedOption == num ? Color::GREEN : Color::WHITE);
	framebuffer.drawRectangle(displ.x + 1, displ.y + 1, 69, 13, gameInstance->bgcolor);
	framebuffer.drawText(opString, (displ.x+71) / 2 + (num==0?-1:1) * (std::round((opString.length() - 1) / 2) * 7) - 7, displ.y + 2, this->font);

}

void dualOptionStage::render(Image& framebuffer)
{
	framebuffer.fill(gameInstance->bgcolor);
	
	framebuffer.drawText("SPACE : Select", 5, 5, this->smallFont, 4, 6);
	framebuffer.drawText("ARROWS: Change", framebuffer.width - 61, 5, this->smallFont, 4, 6);
	std::istringstream iss(this->message);
	int yDispl = 20;
	for (std::string line; std::getline(iss, line);) {
		framebuffer.drawText(line, framebuffer.width / 2 - (std::round((line.length() - 1) / 2) * 7) - 7, yDispl, this->font);
		yDispl += 12;
	}
	this->renderOption(framebuffer, 0);
	this->renderOption(framebuffer, 1);
	
}

void dualOptionStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT) || Input::gamepads[0].didDirectionChanged(PAD_LEFT))
		this->selectedOption = 0;
	else if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)||Input::gamepads[0].didDirectionChanged(PAD_RIGHT))
		this->selectedOption = 1;
	else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)|| Input::gamepads[0].wasButtonPressed(A_BUTTON))
		this->gameInstance->setActiveStage(this->fallbackStage);
}

void dualOptionStage::initStage(int fallbackStage, std::string& message, std::string& option1, std::string& option2)
{
	this->fallbackStage = fallbackStage;
	this->message = message;
	this->option1 = option1;
	this->option2 = option2;
	this->selectedOption = 0;
}

dualOptionStage::dualOptionStage(Image& font, Image& smallFont):stage(font),smallFont(smallFont)
{
	
}

/*==============================================End Screen===================================================*/

void endStage::render(Image& framebuffer)
{
	framebuffer.drawImage(this->assetManagerInstance->getImage("data/endScreen.tga"), 0, 0);
	int survivedDays = ((survivalStage*)gameInstance->getStageOfType(stageType::SURVIVAL))->getTotalDays();
	
	framebuffer.drawText("Days survived: "+std::to_string(survivedDays),15,82,this->font);
	framebuffer.drawText("SPACE to continue",15,framebuffer.height-12,this->font);
}

void endStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)|| Input::gamepads[0].wasButtonPressed(A_BUTTON)) {
		this->gameInstance->setActiveStage(stageType::MENU);
		Input::update();
	}		
}

void endStage::initStage(int fallbackStage)
{
	this->fallbackStage = fallbackStage;
}

endStage::endStage(Image& font, Image& smallFont) :stage(font), smallFont(smallFont)
{

}

/*==============================================Menu Screen===================================================*/

void menuStage::renderBackground(Image& framebuffer) {

	Image& stars = assetManagerInstance->getImage("data/starBackground.tga");
	Image& starsB = assetManagerInstance->getImage("data/starBackground2.tga");
	int displacement =  fmod((gameInstance->totalTime * 20), framebuffer.width);
	int displacement2 =  fmod((gameInstance->totalTime * 18), framebuffer.width);
	
	framebuffer.drawImage(starsB, -displacement2, 0);
	framebuffer.drawImage(starsB, framebuffer.width - displacement2, 0);
	framebuffer.drawImage(stars, -displacement, 0);
	framebuffer.drawImage(stars, framebuffer.width - displacement, 0);

}

void menuStage::render(Image& framebuffer)
{
	framebuffer.fill(gameInstance->bgcolor);
	this->renderBackground(framebuffer);
	if (this->showText)
		framebuffer.drawText("Press SPACE to start", 9, framebuffer.height - 20, this->font);
		
	framebuffer.drawImage(this->assetManagerInstance->getImage("data/logo.tga"), framebuffer.width / 2 - 56, 10);
	
}

void menuStage::update(double seconds_elapsed)
{
	if (this->updCount > 1) {
		this->updCount = 0;
		this->showText = !this->showText;
	}
	this->updCount+= seconds_elapsed;
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		/*this->gameInstance->setActiveStage(stageType::INTRO);
		this->gameInstance->getStageOfType(stageType::INTRO)->initStage();*/
		this->gameInstance->setActiveStage(stageType::INTRO);
		this->gameInstance->getStageOfType(stageType::INTRO)->initStage();
	}
}



menuStage::menuStage(Image& font, Image& smallFont):stage(font),smallFont(smallFont)
{
	
}
/*==============================================Intro===================================================*/

void introStage::render(Image& framebuffer)
{
}

void introStage::update(double seconds_elapsed)
{
	if (currentFrame >= totalFrames) {
		this->gameInstance->setActiveStage(fallbackStage);
		this->gameInstance->initGame();
	}
	else {
		this->displayMessage(this->text[currentFrame], (int)this->type);
	}
	currentFrame++;
	
}

void introStage::initStage()
{
	
	this->fallbackStage = (int) stageType::COUNTDOWN;
	this->currentFrame = 0;
	this->totalFrames = text.size();
	std::cout << this->totalFrames<<std::endl;
}

introStage::introStage(Image& font, Image& smallFont):stage(font),smallFont(smallFont)
{
	
	std::vector<std::string> test={ "Alert!\nAlert!\nAlert!", "A Missile has\nbeen detected!", "Please read the\nfollowing guide and\n gather the maximum\n number of resources\npossible", "You'll have\n60 seconds to\ncollect them!" };
	this->text = test;
}

/*==============================================Post Countdown===================================================*/
void postCountdownStage::render(Image& framebuffer)
{
}

void postCountdownStage::update(double seconds_elapsed)
{
	if (currentFrame >= totalFrames) {
		this->gameInstance->setActiveStage(fallbackStage);
		this->gameInstance->getActiveStage()->initStage();
	}
	else {
		this->displayMessage(this->text[currentFrame], (int)this->type);
	}
	currentFrame++;
}

void postCountdownStage::initStage()
{
	this->fallbackStage = (int)stageType::SURVIVAL;
	this->currentFrame = 0;
	this->totalFrames = text.size();
	std::cout << this->totalFrames << std::endl;
	Synth::SamplePlayback* spaceMusic = this->assetManagerInstance->getAudio("spaceMusic");
	spaceMusic->offset = 0;
	spaceMusic->in_use = true;
}

postCountdownStage::postCountdownStage(Image& font, Image& smallFont): stage(font), smallFont(smallFont)
{
	std::vector<std::string> toDisplay = { "You managed to escape\non time!","As the commander of\nthis escape mission\nyour objective is\nto keep everyone\nalive","You will now have\nto choose your\ndestination!",
	"Our planetary database\nhas information about\n multiple planets!","We've collected info\nabout each one!\nResource levels,\nviolence levels and\ntravel distance",
	"Some planets are\nmore violent\nthan others.\nThe more violent\nthe more probable you\nare to get ambushed\nand injured or killed.","You can take a\ngun with you to\nreduce the chances\nof getting hurt!",
	"Your escape capsule\nis equipped with a\npc where you'll be\nable to do all\nthe decision taking!","Use it to monitor\nyour crew's status\ncheck the inventory\nand see info about\nyour next destination",
	"In the pc you'll be\nable to ration your\nsupplies and give\nthem to crew members\nwhen needed!"};
	this->text = toDisplay;
}
