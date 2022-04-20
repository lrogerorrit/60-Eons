#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"

#include <cmath>


Game* Game::instance = NULL;

Image font;
Image minifont;
Image sprite;
Image testTileset;
Image testIcon;
Color bgcolor(130, 80, 100);

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
	tilePos = Vector2ub(2, 2);
	localTilePos = Vector2(0.0f, 0.0f);

	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	sprite.loadTGA("data/spritesheet.tga"); //example to load an sprite
	testTileset.loadTGA("data/tileset.tga");
	//testIcon.loadTGA("data/icons/guns.tga");

	this->charHandler.makeCharacters(this->astronautNum);
	Game::startMap.loadGameMap("data/mymapV2.map");
	this->localChar = charHandler.getCharacter(0);
	cellSize = testTileset.width / 16;
	this->localChar.setPosition(tilePos*cellSize);

	this->uihandler.countdownUIObj.setStartTime(totalTime);
	

	//enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/music/countdownMusic.wav",1,false);
	//synth.osc1.amplitude = 0.5;
}


void Game::updateTilePosition() {
	Vector2 pos= localChar.getPosition();
	tilePos = Vector2ub(pos.x / cellSize, (pos.y + y_displ) / cellSize);
	//calculate position in local tile
	localTilePos = Vector2((((pos.x + x_displ)) - tilePos.x * cellSize) / cellSize, ((pos.y + y_displ) - tilePos.y * cellSize) / cellSize);
}



void Game::renderMapTest(Image& framebuffer,float dx, float dy) {
	int cs = testTileset.width / 16;
	
	
	/*int cellX = (ogDx) / cs;
	int cellY = (ogDy+13) / cs;

	std::cout << cellX << ", " << cellY << std::endl;*/
	
	dx -= (framebuffer.width / 2);
	dy -= (framebuffer.height / 2);

	Vector2ub tilePos= getTilePosition();
	int startX = max(0, tilePos.x - RENDER_X_CELLS);
	int startY = max(0, tilePos.y- RENDER_Y_CELLS);
	int endX = min(tilePos.x + RENDER_X_CELLS, Game::startMap.width-1);
	int endY = min(tilePos.y + RENDER_Y_CELLS, Game::startMap.height-1);
	//for every cell
	for (int x = startX;x<=endX; ++x)
		for (int y = startY;y<=endY; ++y)
		{
			//get cell info
			sCell& cell = startMap.getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = x * cs; //place offset here if you want
			int screeny = y * cs;
			screenx -= dx;
			screeny -= dy;
			//avoid rendering out of screen stuff
			
			//if (screenx < -cs || (screenx + cs) > framebuffer.width ||
			//	screeny < -cs || (screeny) > framebuffer.height) {
			//	//std::cout << screenx << ", " << screeny << ", " << -cs << std::endl;
			//	continue;
			//}

			if (tilePos.x == x && tilePos.y == y) {
				framebuffer.drawRectangle(screenx, screeny, cs, cs, Color::BLUE); 	//pos in screen
				//print localTilePos to console
				//std::cout << localTilePos.x << ", " << localTilePos.y << std::endl;
				
			}
			else
			//draw region of tileset inside framebuffer
			framebuffer.drawImage(testTileset, 		//image
				screenx, screeny, 	//pos in screen
				area); 		//area
		}

}



void renderDebugGrid(Image& framebuffer) {
	framebuffer.drawLine(framebuffer.width / 2, 0, framebuffer.width / 2, framebuffer.height, Color::RED);
	framebuffer.drawLine(0, framebuffer.height / 2, framebuffer.width, framebuffer.height/2, Color::RED);
}


void Game::renderCountdown(Image &framebuffer) {
	Vector2 charPos = this->localChar.getPosition();
	framebuffer.fill(bgcolor);
	renderMapTest(framebuffer, charPos.x, charPos.y);
	framebuffer.drawImage(sprite, (framebuffer.width / 2) - 9, (framebuffer.height / 2) - 13, localChar.isMoving ? ((int)std::round(totalTime * localChar.getSpeed() * .4) % 4) * 18 : 0, 27 * localChar.getDirection(), 18, 27);			//draws a scaled image
	renderDebugGrid(framebuffer);
	framebuffer.drawImage(testIcon, (framebuffer.width/8) - (testIcon.width / 2), framebuffer.height-(testIcon.height*1.2));
	framebuffer.drawImage(testIcon, (framebuffer.width / 2) - (testIcon.width / 2), framebuffer.height - (testIcon.height * 1.2));
	framebuffer.drawImage(testIcon, (7*framebuffer.width / 8) - (testIcon.width / 2), framebuffer.height - (testIcon.height * 1.2));
	this->uihandler.countdownUIObj.renderUI(framebuffer, font);
	showFramebuffer(&framebuffer);
}

void Game::renderSurvival(Image& framebuffer) {

}

//what to do when the image has to be draw
void Game::render()
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	//...

	getIsCountdownLevel() ? renderCountdown(framebuffer) : renderSurvival(framebuffer);
	
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
	showFramebuffer(&framebuffer);
	*/
}



void Game::updateCountdownLevel(double seconds_elapsed) {
	Vector2& charPos = this->localChar.getPositionRef();
	float speed = this->localChar.getSpeed();
	//std::cout <<charPos.x<< " "<<charPos.y<<"\n";
	localChar.isMoving = false;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) { //if key up
		sCell cell = getCellAtPos(charPos.x, charPos.y - max(y_collisionDist, (speed * seconds_elapsed)));
		
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.y -= speed * seconds_elapsed;
		}
			localChar.dir = UP;

	}
	else if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		sCell cell = getCellAtPos(charPos.x, charPos.y + max(y_collisionDist, (speed * seconds_elapsed)));
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.y += speed * seconds_elapsed;
		}
			localChar.dir = DOWN;
	}
	else if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) { //if key up
		sCell cell = getCellAtPos(charPos.x - max(x_collisionDist, (speed * seconds_elapsed)), charPos.y);
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.x -= speed * seconds_elapsed;
		}
			localChar.dir = LEFT;
	}
	else if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		sCell cell = getCellAtPos(charPos.x + max(x_collisionDist, (speed * seconds_elapsed)), charPos.y);
		if (cell.canEnter()) {
			localChar.isMoving = true;
			charPos.x += speed * seconds_elapsed;
		}
			localChar.dir = RIGHT;
	}
	else {

		localChar.dir = DOWN;
	}
	updateTilePosition();
	if (this->localTilePos.y <= .4) { //if player is close enough
		if (this->startMap.isCellItemType(this->tilePos.x, this->tilePos.y)) {
			this->uihandler.countdownUIObj.showPrompt("Space - Pick Item");
		}
		else if (this->startMap.isCellExitType(this->tilePos.x, this->tilePos.y)) {
			this->uihandler.countdownUIObj.showPrompt("Space - Save Item");
		}
		else {
			this->uihandler.countdownUIObj.hidePrompt();
		}
	}
	else {
		this->uihandler.countdownUIObj.hidePrompt();
	}
	this->uihandler.countdownUIObj.updateCountdown(totalTime);
}

void Game::updateSurvivalLevel(double seconds_elapsed) {

};

void Game::update(double seconds_elapsed)
{
	
	
	//Add here your update method
	//...
	
	totalTime+= seconds_elapsed;
	
	getIsCountdownLevel() ? updateCountdownLevel(seconds_elapsed) : updateSurvivalLevel(seconds_elapsed);
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
		bgcolor.set(0, 255, 0);
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
