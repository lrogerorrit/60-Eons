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

Synth::SamplePlayback* assetManager::getAudio(const char* name)
{
	auto it = this->audioCache.find(name);

	if (it != this->audioCache.end())
		return (it->second);
	
}

void assetManager::cacheImage(const char* path)
{
	if (this->imageCache.find(path) == this->imageCache.end()) {
		Image img;
		img.loadTGA(path);
		this->imageCache[path] = img;
	}
	
}

void assetManager::cacheAudio(const char* name, Synth::SamplePlayback* sample)
{
	this->audioCache[name] = sample;
}
