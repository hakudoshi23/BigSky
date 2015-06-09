#include "controller.h"

#include "../includes.h"
#include "../utils.h"

#include "../entity/bulletManager.h"
#include "../game.h"

Controller::Controller()
{
	this->keystate = NULL;
	this->shootCD = 0.2;
	this->lastShoot = 0;
}

Controller::~Controller()
{
}

void Controller::init()
{
}

void Controller::update(double delta)
{
	int x, y;
	this->mouse_state = SDL_GetMouseState(&x,&y);
	this->mouse_delta.set( this->mouse_position.x - x, this->mouse_position.y - y );
	this->mouse_position.set(x,y);
	this->keystate = SDL_GetKeyboardState(NULL);

	Entity* player = Game::getInstance()->world->findByID(this->entity_id);
	if(player != NULL && camera != NULL){
		double speed = delta * 30;
		if (keystate[SDL_SCANCODE_LSHIFT]) speed *= 5;
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) player->model.rotateLocal(-0.01, Vector3(1,0,0));
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) player->model.rotateLocal(0.01, Vector3(1,0,0));
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) player->model.rotateLocal(0.03, Vector3(0,0,1));
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) player->model.rotateLocal(-0.03, Vector3(0,0,1));
		if (keystate[SDL_SCANCODE_Q]) player->model.rotateLocal(-0.005, Vector3(0,1,0));
		if (keystate[SDL_SCANCODE_E]) player->model.rotateLocal(0.005, Vector3(0,1,0));

		Vector3 f = player->model.rotateVector(Vector3(0, 0, 1));
		player->model.traslate(f.x * speed, f.y * speed, f.z * speed);

		Vector3 playerPos = Vector3(player->model.m[12],player->model.m[13],player->model.m[14]);
		Vector3 camPos = player->model * Vector3(0, 5, -15);
		camera->lookAt(camPos, playerPos, player->model.rotateVector(Vector3(0, 1, 0)));

		if(this->lastShoot > 0) this->lastShoot -= delta;
		if (keystate[SDL_SCANCODE_SPACE]){
			if(this->lastShoot <= 0){
				BulletManager::getInstance()->createBullet(playerPos, f * 500, 10, this->entity_id);
				this->lastShoot = this->shootCD;
			}
		}

		if(Game::getInstance()->mouse_locked){
			int center_x = floor(Game::getInstance()->window_width * 0.5);
			int center_y = floor(Game::getInstance()->window_height * 0.5);
			SDL_WarpMouseInWindow(Game::getInstance()->window, center_x, center_y);
			this->mouse_position.x = center_x;
			this->mouse_position.y = center_y;
		}
	}
}

void Controller::setEntity(int entity_id)
{
	this->entity_id = entity_id;
}

void Controller::setCamera(Camera* cam)
{
	this->camera = cam;
}