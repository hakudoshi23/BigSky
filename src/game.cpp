#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

#include <cmath>
#include <bass.h>

#include "state/mainState.h"

GameState* Game::state = NULL;

Game::Game()
{
	mouse_locked = false;
	//Inicializamos BASS  (id_del_device, muestras por segundo, ...)
	BASS_Init(1, 44100, 0, 0, NULL);
	//El handler para un sample
	HSAMPLE hSample;
	//El handler para un canal
	HCHANNEL hSampleChannel;
	//Cargamos un sample (memoria, filename, offset, length, max, flags)
	hSample = BASS_SampleLoad(false, "data/sound/shot.wav",0,0,3,0); //use BASS_SAMPLE_LOOP in the last param to have a looped sound
	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample,false);
	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}

void Game::setWindow(SDL_Window* window)
{
	this->window = window;
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;
}

void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
	Game::swapState(new MainState());
}

void Game::render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Game::state->render();

	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	Game::state->update(seconds_elapsed);
}

void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0);
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE)
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

Game* Game::getInstance()
{
	static Game* instance = new Game();
	return instance;
}

void Game::swapState(GameState* state)
{
	if(Game::state != NULL) Game::state->destroy();
	Game::state = state;
	if(Game::state != NULL) Game::state->init();
}

