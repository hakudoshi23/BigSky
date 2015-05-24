#ifndef CONTROLLER_H
#define CONTROLLER_H
  
#include "../framework.h"
#include "../camera.h"
#include "../includes.h"

class Controller
{
public:
	//keyboard state
	const Uint8* keystate;
	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	int entity_id;
	Camera* camera;

	Controller();
	~Controller();

	void setEntity(int entity_id);
	void setCamera(Camera* cam);
	
	void init();
	void update(double delta);
};

#endif