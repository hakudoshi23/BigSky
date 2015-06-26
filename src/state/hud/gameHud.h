#ifndef GAME_HUD_H
#define GAME_HUD_H

#include "../../framework.h"

#include "hud.h"
#include "../../mesh.h"
#include "../../camera.h"
#include "../../shader.h"

class GameHUD : public HUD
{
public:
	Camera* camera;

	Mesh* health;
	Mesh* shield;
	Mesh* crosshair;

	Mesh* pointers;
	int* score;

	GameHUD(int* score);
	~GameHUD();

	void init();
	void render();
	void update(double delta);
};


#endif