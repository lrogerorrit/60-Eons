#include "uiHandler.h"


std::vector<std::string> iconNames{ "drink","emptySlot","food","guns","meds"};

countdownUI::countdownUI(iconStorage& storage)
{
	this->iconStore = storage;	
};


void countdownUI::renderUI(Image& framebuffer, Image& font) {
	framebuffer.drawText( std::to_string(this->time), framebuffer.width * .8, framebuffer.height * .05, font );
	
	for (int i = 0; i < 3; i++) {
		Image& img = iconStore.getIconFromHoldingIcon(this->iconSlots[i]);
		framebuffer.drawImage(img, (i == 0 ? (framebuffer.width / 8) : (i == 1 ? (framebuffer.width / 2) : (7 * framebuffer.width / 8))) - (img.width / 2), framebuffer.height - (img.height * 1.2));
	}
}

iconStorage::iconStorage() {
	
	for (int i = 0; i < iconNames.size(); i++) { //loads icons;
		//load the icon
		icon iconData;
		iconData.img= Image();
		
		std::string path = "data/icons/" + iconNames[i] + ".tga";

		
		iconData.img.loadTGA(path.c_str());
		iconData.name = iconNames[i];
		//add it to the map
		icons.push_back(iconData);
	}
}

Image& iconStorage::getIcon(std::string iconName) {
	for (int i = 0; i < icons.size(); i++) {
		if (icons[i].name == iconName) {
			return icons[i].img;
		}
	}
	
}

Image& iconStorage::getIconFromHoldingIcon(holdingIcon& icon) {
	//if (icon == NA) return; //TODO: change so only returns if not na
	for (int i = 0; i < icons.size(); i++) {
		if (icons[i].typ == icon) {
			return icons[i].img;
		}
	}
}