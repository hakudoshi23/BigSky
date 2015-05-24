/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "world/world.h"

class Game
{
public:
	static Game* getInstance();
	World* world;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
    
    float time;

	//mouse state
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	Camera* camera; //our global camera

	Game();
	void init( void );
	void render( void );
	void update( double dt );

	void setWindow(SDL_Window* window);

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
    void onResize( SDL_Event e );
    
	void setWindowSize(int width, int height);
};


#endif 