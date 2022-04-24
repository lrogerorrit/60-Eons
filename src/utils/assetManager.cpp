#include "assetManager.h"

assetManager* assetManager::instance = NULL;


assetManager::assetManager() {
	instance = this;
}

Image& assetManager::getImage(const char* name)
{
	auto it = this->imageCache.find(name);
	
	if (it != this->imageCache.end())
		return (it->second);
	Image img;
	img.loadTGA(name);
	this->imageCache[name] = img;
	return img;
}

void assetManager::cacheImage(const char* path)
{
	if (this->imageCache.find(path) == this->imageCache.end()) {
		Image img;
		img.loadTGA(path);
		this->imageCache[path] = img;
	}
	
}
