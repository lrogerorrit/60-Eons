#pragma once

#include "image.h"

enum eUiRender {
	NONE,
	COUNTDOWN,
	SURVIVAL,
};

class iconStorage {
public:
	std::vector<Image> icons;

	iconStorage();
	~iconStorage() {
		icons.clear();
	};

	Image& getIcon(String iconName);
	std::vector<Image>& getIcons() {
		return icons;
	};
};




class countdownUI {
	public:
		countdownUI();
		int time = 60;
		
		
		void updateUI();
		void renderUI(Image &framebuffer);
		
		void updateCountdown(float totalTime);
		
	
	
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

