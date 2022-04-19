#include "uiHandler.h"


std::vector<std::string> iconNames{ "drink","emptySlot","food","guns","meds"};
std::vector<holdingIcon> iconTypes {holdingIcon::WATER,holdingIcon::EMPTY,holdingIcon::FOOD,holdingIcon::GUN,holdingIcon::MEDS}; //Would use a switch, but cant do switch of types :(

/*countdownUI::countdownUI(iconStorage& storage)
{
	
	this->iconStore = storage;

	//set all values of iconslots to empty
	for (int i = 0; i < 3; i++)
	{
		iconSlots[i] = holdingIcon::WATER;
	}

	
};*/


void countdownUI::renderUI(Image& framebuffer, Image& font) {
	framebuffer.drawText( std::to_string(this->time), framebuffer.width * .8, framebuffer.height * .05, font );
	//std::cout << "icons id second check " << this << std::endl;
	for (int i = 0; i < 3; i++) {
		Image& img = iconStore.getIconFromHoldingIcon(this->iconSlots[i]);
		framebuffer.drawImage(img, (i == 0 ? (framebuffer.width / 8) : (i == 1 ? (framebuffer.width / 2) : (7 * framebuffer.width / 8))) - (img.width / 2), framebuffer.height - (img.height * 1.2));
	}
}




iconStorage::iconStorage() {
	
	
}

void iconStorage::loadIcons() {
	std::cout << "loading icons\n";
	for (int i = 0; i < iconNames.size(); i++) { //loads icons;
		//load the icon

		//make new icon obj named iconDat;
		
		
		icon iconData;
		iconData.typ = iconTypes[i];
		//iconData.img = Image();
		iconData.name = iconNames[i];
		std::string path = "data/icons/" + iconData.name + ".tga";


		iconData.img.loadTGA(path.c_str());


		//add it to the map
		this->icons.push_back(iconData);


	}	
}

Image& iconStorage::getIcon(std::string iconName) {
	for (int i = 0; i < icons.size(); i++) {
		if (icons[i].name == iconName) {
			return icons[i].img;
		}
	}
	
}

Image& iconStorage::getIconFromHoldingIcon(holdingIcon icon) {
	//if (icon == NA) return; //TODO: change so only returns if not na
	for (int i = 0; i < icons.size(); i++) {
		if (icons[i].typ == icon) {
			return icons[i].img;
		}
	}
}


