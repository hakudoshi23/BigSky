#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
  
#include "../framework.h"
#include "../includes.h"

#include "../camera.h"
#include "controller.h"
#include "XBOX360.h"

class PlayerController : public Controller
{
public:
	//keyboard state
	const Uint8* keystate;
	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	SDL_Joystick* joystick;

	int entity_id;

	bool debug;

	float lastShoot;

	PlayerController();
	~PlayerController();
	
	void init();
	void update(double delta);
};

#endif