#pragma once

#include "image.h"
#include <String>
#include "utils/commonItems.h"



enum class eUiRender {
	NONE,
	COUNTDOWN,
	SURVIVAL,
};




struct icon {
	std::string name;
	holdingIcon typ= holdingIcon::EMPTY;
	Image img;
	
};

class iconStorage {
public:
	std::vector<icon> icons = {};

	iconStorage();
	~iconStorage() {
		icons.clear();
	};

	void loadIcons();

	Image& getIcon(std::string iconName);
	Image& getIconFromHoldingIcon(holdingIcon icon);
		
	
	std::vector<icon>& getIcons() {
		return icons;
	};
};



//TODO: set base class for all ui types;


class countdownUI {
	public:
		int time = 60;
		float startTime=0.0f;
		holdingIcon iconSlots[3];
		iconStorage iconStore;
		std::string promptMsg="";
		bool promptVisible=false;
		
		countdownUI() {
			for (int i = 0; i < 3; i++)
			{
				iconSlots[i] = holdingIcon::EMPTY;
			}
		}
		
		//countdownUI(iconStorage& storage);
	
		void assignIconStore(iconStorage& storage) {
			this->iconStore = storage;
		}

		void setStartTime(float time) {
			startTime = time;
		};
		
		
		void updateIconSlot(int slot, holdingIcon& newItem) {
			this->iconSlots[slot] = newItem;
		}

		void setIconSlotsFromVector(std::vector<holdingIcon> newIcons);
		
		void renderUI(Image &framebuffer, Image& font);
		
		void updateCountdown(float totalTime) {
			int calcTime= ((int)(60 - (totalTime - startTime)));
			time = calcTime >= 0 ? calcTime : 0;
		};
		int getCountdownTime() {
			return this->time;
		}
	
		void showPrompt(std::string msg) {
			this->promptMsg = msg;
			this->promptVisible = true;
		}
		void hidePrompt() {
			this->promptVisible = false;
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
public:
	
	eUiRender uiToRender = eUiRender::NONE;
	
	iconStorage icons;
	
	
	countdownUI countdownUIObj;
	//survivalUI survivalUIObj;
	
	uiHandler() {
		this->icons.loadIcons();
		countdownUIObj.assignIconStore(this->icons);
		std::cout << "icons id " << this<<std::endl;
	};
	
	eUiRender getUiToRender() {
		return uiToRender;
	};
	
	countdownUI getCountdownUI() {
		return countdownUIObj;
	};
	
	/*survivalUI getSurvivalUI() {
		return survivalUIObj;
	};*/

	void setUiToRender(eUiRender uiToRender) {
		this->uiToRender = uiToRender;
	};
	

	
	
};

