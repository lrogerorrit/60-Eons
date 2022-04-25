#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "stage.h"
#include <cmath>
#include <time.h>
#include <stdlib.h>


Game* Game::instance = NULL;


Image font;
Image minifont;
Image sprite;
Image testTileset;
Image testIcon;


Game::Game(int window_width, int window_height, SDL_Window* window)
{
	
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	tilePos = Vector2ub(7, 2);
	localTilePos = Vector2(0.0f, 0.0f);

	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	sprite.loadTGA("data/spritesheet.tga"); //example to load an sprite
	testTileset.loadTGA("data/tileset.tga");
	//testIcon.loadTGA("data/icons/guns.tga");
	this->assetMan = assetManager::instance;
	this->charHandler.makeCharacters(this->astronautNum);
	this->startMap.loadGameMap("data/finalMap.map");
	this->localChar = charHandler.getCharacter(0);
	cellSize = testTileset.width / 16;
	this->localChar.setPosition(tilePos*cellSize);
	
	this->uihandler.countdownUIObj.setStartTime(totalTime);
	
	assetMan->cacheImage("data/mini-font-black-4x6.tga");
	assetMan->cacheImage("data/bitmap-font-black.tga");
	assetMan->cacheImage("data/icons/advanceDay.tga");
	assetMan->cacheImage("data/icons/pc.tga");
	assetMan->cacheImage("data/icons/inventory.tga");
	assetMan->cacheImage("data/icons/close.tga");
	assetMan->cacheImage("data/pcLayout.tga");
	assetMan->cacheImage("data/pcTabText.tga");
	assetMan->cacheImage("data/playerCard.tga");
	assetMan->cacheImage("data/astronautBusts.tga");
	assetMan->cacheImage("data/deadOverlay.tga");
	assetMan->cacheImage("data/pcPlanets.tga");
	assetMan->cacheImage("data/endScreen.tga");
	assetMan->cacheImage("data/logo.tga");
	assetMan->cacheImage("data/starBackground.tga");
	assetMan->cacheImage("data/starBackground2.tga");
	assetMan->cacheImage("data/daysStay.tga");
	assetMan->cacheImage("data/pcDataLayout.tga");
	assetMan->cacheImage("data/planets0.tga");
	assetMan->cacheImage("data/planets1.tga");
	assetMan->cacheImage("data/shipBody.tga");
	assetMan->cacheImage("data/shipTail.tga");
	assetMan->cacheImage("data/planetCard.tga");
	assetMan->cacheImage("data/planetDataLayout.tga");
	assetMan->cacheImage("data/planetBackground.tga");
	assetMan->cacheImage("data/tutorial.tga");
	
	
	
	//this->stages.reserve(6);
	this->stages.push_back(new countdownStage(testTileset,sprite,font,this->localChar));
	this->stages.push_back(new survivalStage(font, minifont));
	this->stages.push_back(new planetChoosingStage(minifont,font));
	this->stages.push_back(new pcStage(font, minifont));
	this->stages.push_back(new multipleOptionsStage(font,minifont));
	this->stages.push_back(new messageStage(font, minifont));
	this->stages.push_back(new dualOptionStage(font, minifont));
	this->stages.push_back(new endStage(font, minifont));
	this->stages.push_back(new menuStage(font, minifont));
	this->stages.push_back(new introStage(font, minifont));
	this->stages.push_back(new postCountdownStage(font, minifont));
	
	

	enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/music/countdownMusic.wav",1,false);
	
	Synth::SamplePlayback* countdownMusic= synth.playSample("data/music/countdownMusic.wav", 1, false);
	Synth::SamplePlayback* menuMusic = synth.playSample("data/music/menuMusic.wav", 1, true);
	Synth::SamplePlayback* spaceMusic= synth.playSample("data/music/spaceMusic.wav", 1, true);
	countdownMusic->stop();
	spaceMusic->stop();

	
	

	assetMan->cacheAudio("countdownLoop", countdownMusic);
	assetMan->cacheAudio("menuMusic", menuMusic);
	assetMan->cacheAudio("spaceMusic", spaceMusic);

	//synth.osc1.amplitude = 0.5;
}

void Game::initGame()
{
	tilePos = Vector2ub(7, 2);
	localTilePos = Vector2(0.0f, 0.0f);
	this->localChar.setPosition(tilePos * cellSize);
	this->uihandler.countdownUIObj.setStartTime(totalTime);
	this->charHandler.makeCharacters(this->astronautNum);
	this->localChar = charHandler.getCharacter(0);
	this->localChar.setPosition(tilePos * cellSize);
	assetMan->getAudio("menuMusic")->stop();
	assetMan->getAudio("spaceMusic")->stop();
	Synth::SamplePlayback* countdownMusic = assetMan->getAudio("countdownLoop");
	countdownMusic->offset = 0;
	countdownMusic->in_use = true;
	this->deadCheckActive = true;
	survivalStage* st = (survivalStage*)this->getStageOfType(stageType::SURVIVAL);
	st->resetSurvivalActions();
	
	
}







//what to do when the image has to be draw
void Game::render()
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	//...
	
	switch (this->activeStage) {
	case stageType::COUNTDOWN: {
		Vector2 charPos = this->localChar.getPosition();
		countdownStage* cStage = (countdownStage*)getStage(stageType::COUNTDOWN);
		cStage->render(framebuffer, charPos.x, charPos.y);

		break;
	}
	default: {
		getActiveStage()->render(framebuffer);
		break;
	}
	}
	//getIsCountdownLevel() ? renderCountdown(framebuffer) : renderSurvival(framebuffer);
	
	showFramebuffer(&framebuffer);
	
	//some new useful functions
	/*Vector2 charPos = this->localChar.getPosition();
		framebuffer.fill( bgcolor );								//fills the image with one color
		//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
		//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
		//framebuffer.drawImage( sprite, 0, 0, 18, 27,18,27 );			//draws a scaled image
		//framebuffer.drawImage( sprite, 0, 0, Area(0,0,14,18) );	//draws only a part of an image
				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		//framebuffer.drawText( toString(time), 1, 10, minifont,4,6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
		renderMapTest(framebuffer, charPos.x, charPos.y);
		//std::cout << totalTime << "\n";
		framebuffer.drawImage(sprite, (framebuffer.width/2)-9,(framebuffer.height/2)-13, localChar.isMoving?((int)std::round(totalTime*localChar.getSpeed()*.4)%4) * 18:0, 27 * localChar.getDirection(), 18, 27);			//draws a scaled image
		renderDebugGrid(framebuffer);
	//send image to screen
	*/
}





void Game::update(double seconds_elapsed)
{
	
	
	//Add here your update method
	//...
	
	totalTime+= seconds_elapsed;
	if (this->deadCheckActive)
	if (this->charHandler.getAliveCharacterNum() == 0) {
		this->deadCheckActive = false;
		this->setActiveStage(stageType::END);
		endStage* st = (endStage*)this->getActiveStage();
		st->initStage((int)stageType::MENU);
	}

	switch (this->activeStage) {
	case stageType::COUNTDOWN: 
		this->stages[(int)stageType::COUNTDOWN]->update(seconds_elapsed);
		break;
	default: 
		getActiveStage()->update(seconds_elapsed);
		break;
	
	}

	
	
	//getIsCountdownLevel() ? updateCountdownLevel(seconds_elapsed) : updateSurvivalLevel(seconds_elapsed);
	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	{
		
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		//bgcolor.set(0, 255, 0);
	}
	
	
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}





Vector2ub Game::mapMousePosition() {
	Vector2 mouse_position = Input::mouse_position;
	Vector2ub toReturn;
	int clampedXSize = window_height * (4/3.0);
	int borderSize= (window_width - clampedXSize) / 2;

	int mx= clamp((int) max(0,mouse_position.x-borderSize),0,clampedXSize);
	//std::cout << window_width << "," << clampedXSize <<","<<window_height<<"," <<mx<<"," << mouse_position.x << "," << mouse_position.y << std::endl;
	toReturn.x = mapValue(mx, 0, clampedXSize, 0, 160);
	toReturn.y = mapValue(mouse_position.y, 0, window_height, 0, 120);
	return toReturn;
}