#ifndef LOSE_HUD_H
#define LOSE_HUD_H

#include "../../framework.h"
#include "../../includes.h"

#include "hud.h"
#include "../../mesh.h"
#include "../../camera.h"
#include "../../shader.h"
#include "../../controller/XBOX360.h"

class LoseHUD : public HUD
{
public:
	Camera* camera;
	
	SDL_Joystick* joystick;
	const Uint8* keystate;
	
	Mesh* title;
	Mesh* text;
	int last_time;

	LoseHUD();
	~LoseHUD();

	void init();
	void render();
	void update(double delta);
};


#endif