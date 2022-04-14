/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "characterHandler.h"
#include "utils/mapHandler.h"
#include <cmath>

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	float totalTime = 0.0f;
	int fps;
	bool must_exit;

	//audio
	Synth synth;

	//custom stuff
	const int RENDER_X_CELLS = 3;
	const int RENDER_Y_CELLS = 3;


	const int astronautNum = 1;
	character localChar;

	characterHandler charHandler;
	gameMap startMap;
	Vector2ub tilePos;
	int cellSize;

	bool is_countdownLevel=false;

	// make a list of ints of size astronautNum
	

	//ctor
	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	void showFramebuffer(Image* img);

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	//audio stuff
	void enableAudio(); //opens audio channel to play sound
	void onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec &audio_spec); //called constantly to fill the audio buffer

	//custom functions
	inline bool getIsCountdownLevel() { return is_countdownLevel; }
	inline void setIsCountdownLevel(bool value) { is_countdownLevel = value; }
	void setLocalChar(character& plChar) {
		this->localChar = plChar;
	};
	
	void renderMapTest(Image& framebuffer, float dx, float dy);
	void updateTilePosition();	
	Vector2ub getTilePosition() { return tilePos; };
	
	

	
	
	
};


#endif 