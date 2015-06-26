#ifndef MENU_HUD_H
#define MENU_HUD_H

#include "../../framework.h"
#include "../../includes.h"

#include "hud.h"
#include "../../mesh.h"
#include "../../camera.h"
#include "../../shader.h"
#include "../../controller/XBOX360.h"

class MenuHUD : public HUD
{
public:
	Camera* camera;
	
	SDL_Joystick* joystick;
	const Uint8* keystate;
	
	int current_index, options_increment, x_offset, y_offset;
	float index_change_cd, last_index_change;
	std::vector<std::string> options;

	Mesh* background;
	Mesh* pointer;
	Mesh* logo;

	MenuHUD(Camera* cam);
	~MenuHUD();

	void init();
	void render();
	void update(double delta);

	void optionIndexChange();
	void enter();
};


#endif