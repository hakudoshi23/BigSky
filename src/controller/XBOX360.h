#ifndef XBOX360_H
#define XBOX360_H
  
#include "../framework.h"
#include "../includes.h"

#include "controller.h"
#include "../camera.h"

enum XBOXpad {
	LEFT_ANALOG_X = 0,
	LEFT_ANALOG_Y = 1,
	RIGHT_ANALOG_X = 4,
	RIGHT_ANALOG_Y = 3,
	TRIGGERS = 2,

	A_BUTTON = 0,
	B_BUTTON = 1,
	X_BUTTON = 2,
	Y_BUTTON = 3,
	LB_BUTTON = 4,
	RB_BUTTON = 5,
	BACK_BUTTON = 6,
	START_BUTTON = 7,
	LEFT_ANALOG_BUTTON = 8,
	RIGHT_ANALOG_BUTTON = 9
};

enum HATState {
	HAT_CENTERED = 0x00,
	HAT_UP = 0x01,
	HAT_RIGHT = 0x02,
	HAT_DOWN = 0x04,
	HAT_LEFT = 0x08,
	HAT_RIGHTUP = (HAT_RIGHT|HAT_UP),
	HAT_RIGHTDOWN = (HAT_RIGHT|HAT_DOWN),
	HAT_LEFTUP = (HAT_LEFT|HAT_UP),
	HAT_LEFTDOWN = (HAT_LEFT|HAT_DOWN)
};

struct JoystickState {
	int num_axis;
	int num_buttons;
	float axis[8];
	char button[16];
	HATState hat;
};

class XBOX360
{
public:
	static XBOX360* getInstance();

	~XBOX360();

	int getNumJoysticks();
	SDL_Joystick* openJoystick(int num_joystick);
	JoystickState getJoystickState(SDL_Joystick* joystick);
private:
	XBOX360();
};

#endif