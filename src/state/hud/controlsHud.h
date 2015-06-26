#ifndef CONTROLS_HUD_H
#define CONTROLS_HUD_H

#include "../../framework.h"
#include "../../includes.h"

#include "hud.h"
#include "../../mesh.h"
#include "../../camera.h"
#include "../../shader.h"
#include "../../controller/XBOX360.h"

class ControlsHUD : public HUD
{
public:
	Camera* camera;
	
	SDL_Joystick* joystick;
	const Uint8* keystate;

	Mesh* background;
	int last_time;

	ControlsHUD(Camera* cam);
	~ControlsHUD();

	void init();
	void render();
	void update(double delta);
};


#endif