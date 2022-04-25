#pragma once
//#include "../game.h"
#include "utils/commonItems.h"
#include "includes.h"
#include "image.h"
#include "utils/mapHandler.h"
#include "characterHandler.h"
#include "utils/assetManager.h"
#include <sstream>



class Game;





class stage
{
protected:
	Image& font;
	Game* gameInstance;
	assetManager* assetManagerInstance;
	
	
public:
	
	stage(Image& font);
	
	virtual void render(Image& framebuffer) {};
	virtual void update(double seconds_elapsed) {};
	virtual void initStage() {};
	void displayMessage(std::string msg, int fallbackStage);
	void displayDualOption(std::string msg, std::string op1, std::string op2, int fallbackStage);
	void displayMultipleOption(std::vector<std::string>& options, int fallbackStage);
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
	countdownStage(Image& tileset, Image& sprite, Image& font, character& localChar) : stage(font), tileset(tileset), sprite(sprite), localChar(localChar) {
		this->cellSize = tileset.width / 16;
	};

	stageType type = stageType::COUNTDOWN;

	void render(Image& framebuffer, float dx, float dy);
	void update(double seconds_elapsed);
	void initStage() {};
	
};

class survivalActions;

enum class eNextCycleGetInfo {
	NONE,
	PLANETCHOICE,
};

class survivalStage :public stage
{
private:
	Image& smallFont;
	survivalActions* survivalActionHandler=nullptr;
	eNextCycleGetInfo infoToFech= eNextCycleGetInfo::NONE;
	void renderSpaceShip(Image& framebuffer);
	void renderInfo(Image& framebuffer);
	void renderUI(Image& framebuffer);
	void renderBackground(Image& framebuffer);
	
	std::string iconTip = "";
	bool tipVisible = false;

	void choosePlanet();
	void advanceToNextDay();
	void openPc(ePcPage page);
public:

	stageType type = stageType::SURVIVAL;
	survivalStage(Image& font, Image& smallFont);




	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage();
	survivalActions& getSurvivalActions();
	int getTotalDays();
	void resetSurvivalActions();
};

struct planetData;




class planetChoosingStage :public stage {
private:
	Image& secondFont;
	int selectedOption=0;
	int fallbackStage = 0;
	
	std::vector< planetData*> planetsData;
	void renderPlanetCard(Image& framebuffer, planetData* planData, int pos);
public:
	stageType type = stageType::PLANET_CHOOSING;
	
	planetChoosingStage(Image& font, Image& secondFont);
	
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage(std::vector< planetData>& newData, int fallbackStage);
	int getSelectedOption() {
		return this->selectedOption;
	};
};

enum class eNextCycleGetInfoPC {
	NONE,
	CREW_OPTIONS,
	GUN_CONFIRM,
};

class pcStage :public stage {
private:
	Image& smallFont;
	
	int fallbackStage = 0;
	ePcPage activePage= ePcPage::INVENTORY;
	bool atPlanet = false;
	bool tipVisible = false;
	int selectedCard = -1;
	std::string topTip = "";
	eNextCycleGetInfoPC infoToFech = eNextCycleGetInfoPC::NONE;
	survivalActions* survivalActionHandler=nullptr;
	
	void openPage(ePcPage page);

	//render functions
	void renderInventoryPage(Image& framebuffer);

	void renderCrewCard(Image& framebuffer, int crewNum);

	
	
	void renderCrewPage(Image& framebuffer);
	void renderPlanetPage(Image& framebuffer);

	void goExplorePlanet(int plNum, bool hasGun);

	

	void openDualOptions(std::string msg, std::string op1, std::string op2, eNextCycleGetInfoPC flagToSet);

	
	//update functions
	void updateInventoryPage();

	
	void openCrewOptions(int crewNum);
	
	void updateCrewPage();
	void updatePlanetPage();
	
	
public:

	stageType type = stageType::PC;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage(int fallbackStage, ePcPage pcPage, bool atPlanet);
	pcStage(Image& font, Image & smallFont);
	
};



class multipleOptionsStage : public stage {
private:
	std::vector<std::string> options;
	int selectedOption = 0;
	int fallbackStage = 0;
	Image& smallFont;
	void renderOption(Image& framebuffer, std::string& name, int num);
	
public:
	stageType type = stageType::MULTIPLE_OPTIONS;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage(int fallbackStage,std::vector<std::string>&options);
	int getSelectedOption() {
		return this->selectedOption;
	};
	multipleOptionsStage(Image& font, Image& smallFont);
};


class messageStage :public stage {
private:
	int fallbackStage = 0;
	Image& smallFont;
	std::string message;
	
public:
	stageType type = stageType::MESSAGE;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage(int fallbackStage, std::string& message);
	messageStage(Image& font, Image& smallFont);

};

class dualOptionStage :public stage {
private:
	int fallbackStage = 0;
	Image& smallFont;
	std::string message;
	std::string option1;
	std::string option2;
	int selectedOption = 0;
	void renderOption(Image& framebuffer, int num);
public:
	stageType type = stageType::DUAL_OPTION;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage(int fallbackStage, std::string& message, std::string& option1, std::string& option2);
	dualOptionStage(Image& font, Image& smallFont);
	int getSelectedOption() {
		return this->selectedOption;
	};

};


class endStage : public stage {
private:
	Image& smallFont;
	int fallbackStage = 0;

public:
	stageType type = stageType::END;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage(int fallbackStage);
	endStage(Image& font, Image& smallFont);


};

class menuStage : public stage {
	private:
	Image& smallFont;
	bool showText = true;
	float updCount = 0;
public:
	stageType type = stageType::MENU;
	void renderBackground(Image& framebuffer);
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage();
	menuStage(Image& font, Image& smallFont);

};

class introStage : public stage {
private:
	Image& smallFont;
	int fallbackStage = 0;
	int currentFrame = 0;
	int totalFrames = 0;
	std::vector<std::string> text;
	
public:
	stageType type = stageType::INTRO;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage();
	introStage(Image& font, Image& smallFont);


		
};

class postCountdownStage :public stage {
	private:
	Image& smallFont;
	int fallbackStage = 0;
	int currentFrame = 0;
	int totalFrames = 0;
	std::vector<std::string> text;
public:
	stageType type = stageType::POST_COUNTDOWN;
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void initStage();
	postCountdownStage(Image& font, Image& smallFont);

};