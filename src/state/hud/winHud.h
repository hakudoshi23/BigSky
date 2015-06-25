#ifndef WIN_HUD_H
#define WIN_HUD_H

#include "../../framework.h"
#include "../../includes.h"

#include "hud.h"
#include "../../mesh.h"
#include "../../camera.h"
#include "../../shader.h"
#include "../../controller/XBOX360.h"

class WinHUD : public HUD
{
public:
	Camera* camera;
	
	SDL_Joystick* joystick;
	const Uint8* keystate;
	
	Mesh* title;
	Mesh* text;
	int last_time;

	WinHUD();
	~WinHUD();

	void init();
	void render();
	void update(double delta);
};


#endif