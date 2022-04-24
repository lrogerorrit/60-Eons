#pragma once

#include "../framework.h"
#include "../includes.h"
#include <map>
#include <string>
#include "../image.h"

//TODO: add file caching

class assetManager
{
private:
	std::map<std::string, Image> imageCache;


	public:
		static assetManager* instance;

		assetManager();
		
		//Will try to get image, if not found, will load it and cache it.
		Image& getImage(const char *name);
		//precache image on call. Will ignore if already cached
		void cacheImage(const char *path);
		
		
};

