#pragma once

#include "image.h"
#include <String>


enum eUiRender {
	NONE,
	COUNTDOWN,
	SURVIVAL,
};

enum holdingIcon {
	EMPTY,
	FOOD,
	WATER,
	MEDS,
	GUN,
	NA,

};


struct icon {
	std::string name;
	holdingIcon typ=NA;
	Image img;
};

class iconStorage {
public:
	std::vector<icon> icons = {};

	iconStorage();
	~iconStorage() {
		icons.clear();
	};

	Image& getIcon(std::string iconName);
	Image& getIconFromHoldingIcon(holdingIcon& icon);
		
	
	std::vector<icon>& getIcons() {
		return icons;
	};
};






class countdownUI {
	public:
		int time = 60;
		float startTime=0.0f;
		holdingIcon iconSlots[3];
		iconStorage iconStore;

		

		countdownUI(iconStorage& storage);
	
		

		void setStartTime(float time) {
			startTime = time;
		};
		void updateIconSlot(int slot, holdingIcon newItem);
		
		void renderUI(Image &framebuffer, Image& font);
		
		void updateCountdown(float totalTime) {
			time = (int)(60 - (totalTime - startTime));
		};
		int getCountdownTime() {
			return this->time;
		}
	
	
};



class survivalUI {
public: 
	survivalUI();
	
	void updateUI();
	
	void renderUI(Image& framebuffer);
};


class uiHandler
{
	
	eUiRender uiToRender = NONE;
	countdownUI countdownUIObj;
	survivalUI survivalUIObj;
	
	uiHandler();
	
	eUiRender getUiToRender() {
		return uiToRender;
	};
	
	countdownUI getCountdownUI() {
		return countdownUIObj;
	};
	
	survivalUI getSurvivalUI() {
		return survivalUIObj;
	};

	void setUiToRender(eUiRender uiToRender) {
		this->uiToRender = uiToRender;
	};
	

	
	
};

