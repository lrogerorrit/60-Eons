#pragma once

#include "../framework.h"
#include "../includes.h"
#include <map>
#include <string>
#include "../image.h"
#include "../synth.h"

//TODO: add file caching

class assetManager
{
private:
	std::map<std::string, Image> imageCache;
	std::map < std::string, Synth::SamplePlayback*> audioCache;

	public:
		static assetManager* instance;

		assetManager();
		
		//Will try to get image, if not found, will load it and cache it.
		Image& getImage(const char *name);
		
		//Will try to get audio, if not found, will load it and cache it.
		Synth::SamplePlayback* getAudio(const char *name);
		//precache image on call. Will ignore if already cached
		void cacheImage(const char *path);
		//precache audio on call. Will ignore if already cached
		void cacheAudio(const char* name, Synth::SamplePlayback* sample);
		
		
};

