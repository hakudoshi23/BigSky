#ifndef LOADING_HUD_H
#define LOADING_HUD_H

#include "../../framework.h"
#include "../../includes.h"

#include "hud.h"
#include "../../mesh.h"
#include "../../camera.h"
#include "../../shader.h"
#include "../../controller/XBOX360.h"

class LoadingHUD : public HUD
{
public:
	Camera* camera;
	
	SDL_Joystick* joystick;
	const Uint8* keystate;

	bool* ready;

	Mesh* progress;
	Mesh* mission;
	Mesh* logo;

	LoadingHUD(bool* ready);
	~LoadingHUD();

	void init();
	void render();
	void update(double delta);
};


#endif