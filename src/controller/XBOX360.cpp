#include "XBOX360.h"

#include "../includes.h"
#include "../utils.h"

XBOX360::XBOX360(){
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
}

XBOX360::~XBOX360(){
}

int XBOX360::getNumJoysticks(){
	return SDL_NumJoysticks();
}

SDL_Joystick* XBOX360::openJoystick(int num_joystick) {
	if(SDL_NumJoysticks() <= num_joystick)
		return NULL;
	return SDL_JoystickOpen(num_joystick);
}

JoystickState XBOX360::getJoystickState(SDL_Joystick* joystick) {
	JoystickState state;
	memset(&state, 0, sizeof(JoystickState));

	if ( joystick == NULL ) return state;

	state.num_axis = SDL_JoystickNumAxes((SDL_Joystick*) joystick);
	state.num_buttons = SDL_JoystickNumButtons( (SDL_Joystick*)joystick);

	if (state.num_axis > 8) state.num_axis = 8;
	if (state.num_buttons > 16) state.num_buttons = 16;

	/* Axis */
	for (int i = 0; i < state.num_axis; ++i)
		state.axis[i] = SDL_JoystickGetAxis((SDL_Joystick*) joystick, i) / 32768.0f; //range -32768 to 32768
	/* Buttons */
	for (int i = 0; i < state.num_buttons; ++i)
		state.button[i] = SDL_JoystickGetButton((SDL_Joystick*) joystick, i);
	state.hat = (HATState)(SDL_JoystickGetHat((SDL_Joystick*) joystick, 0) - SDL_HAT_CENTERED); //one hat is enough

	return state;
}

XBOX360* XBOX360::getInstance(){
	static XBOX360* instance = new XBOX360();
	return instance;
}