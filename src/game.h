/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "world/world.h"
#include "state/gameState.h"

class Game
{
public:
	static void swapState(GameState* state);
	static Game* getInstance();
	static GameState* state;
	World* world;

	SDL_Window* window;
	int window_width;
	int window_height;
    
    float time;

	bool mouse_locked;
	
	Camera* camera;

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