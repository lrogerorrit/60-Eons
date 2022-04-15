#pragma once

enum eUiRender {
	NONE,
	COUNTDOWN,
	SURVIVAL,
};




class countdownUI {
	public:
		countdownUI();
		
		
		void updateUI();
	
	
};

class survivalUI {
public: countdown
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

