#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

#include <cmath>
#include <bass.h>

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

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;

	//set the clear color (the background color)
	glClearColor(0.623529, 0.752941, 0.796078, 1.0);
	//glClearColor(0.0, 0.0, 0.0, 1.0);

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0,1500,1500),Vector3(0,750,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	camera->setPerspective(70,window_width/(float)window_height,1,10000); //set the projection, we want to be perspective

	Game::getInstance()->world = new World();
	Game::getInstance()->world->setCamera(camera);
	Game::getInstance()->world->init();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Game::world->render();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	Game::world->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
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

