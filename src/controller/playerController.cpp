#include "playerController.h"

#include "../includes.h"
#include "../utils.h"

#include "../world/bulletManager.h"
#include "../entity/entityPlane.h"
#include "../sound/soundManager.h"
#include "../game.h"

PlayerController::PlayerController(){
	if(XBOX360::getInstance()->getNumJoysticks() > 0){
		this->joystick = XBOX360::getInstance()->openJoystick(0);
	} else this->joystick = NULL;
	this->keystate = NULL;
	this->lastShoot = 0;
	this->debug = false;
}

PlayerController::~PlayerController(){
}

void PlayerController::init(){
}

void PlayerController::update(double delta){
	int x, y;
	this->mouse_state = SDL_GetMouseState(&x,&y);
	this->mouse_delta.set( this->mouse_position.x - x, this->mouse_position.y - y );
	this->mouse_position.set(x,y);
	this->keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_TAB]) debug = !debug;
	
	Entity* player = World::getInstance()->findByID(this->entity_id);
	if( player != NULL && !debug){
		double speed = delta * 50;

		/* Update Player position */
		/* Keyboard */
		EntityPlane* plane = (EntityPlane*) player;
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) plane->pitch(-1);
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) plane->pitch(1);
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) plane->roll(1);
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) plane->roll(-1);
		if (keystate[SDL_SCANCODE_Q]) plane->yaw(-1);
		if (keystate[SDL_SCANCODE_E]) plane->yaw(1);
		/* Controller */
		JoystickState state = XBOX360::getInstance()->getJoystickState(this->joystick);
		if(state.axis[0] < -0.3) plane->roll(-state.axis[0]);
		if(state.axis[0] > 0.3) plane->roll(-state.axis[0]);
		if(state.axis[1] < -0.3) plane->pitch(state.axis[1]);
		if(state.axis[1] > 0.3) plane->pitch(state.axis[1]);
		if(state.button[8]) plane->yaw(-1);
		if(state.button[9]) plane->yaw(1);

		/* Advance plane */
		Vector3 f = player->model.rotateVector(Vector3(0, 0, 1));
		if (keystate[SDL_SCANCODE_LSHIFT] || state.button[13]) {
			SoundManager::getInstance()->playSound("data/sound/player_sprint.wav");
			speed *= 3;
		}
		player->model.traslate(f.x * speed, f.y * speed, f.z * speed);

		/* Shoot bullets! */
		if(keystate[SDL_SCANCODE_SPACE] || state.button[10]) {
			if(plane->shootBullet()) 
				SoundManager::getInstance()->playSound("data/sound/player_default_weapon.wav", 3000);
		}
		/* Shoot missile! */
		if(keystate[SDL_SCANCODE_F] || state.button[12]) {
			if(plane->shootMissile()) 
				SoundManager::getInstance()->playSound("data/sound/player_missile.wav", 3000);
		}
	}
	/*else{
		double speed = delta * 100;

		if(Game::getInstance()->mouse_locked){
			camera->rotate(mouse_delta.x * 0.005, Vector3(0,-1,0));
			camera->rotate(mouse_delta.y * 0.005, camera->getLocalVector( Vector3(-1,0,0)));
		}

		if (keystate[SDL_SCANCODE_LSHIFT]) speed *= 10;
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) camera->move(Vector3(0,0,1) * speed);
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) camera->move(Vector3(0,0,-1) * speed);
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) camera->move(Vector3(1,0,0) * speed);
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) camera->move(Vector3(-1,0,0) * speed);
    
		if(Game::getInstance()->mouse_locked){
			int center_x = floor(Game::getInstance()->window_width * 0.5);
			int center_y = floor(Game::getInstance()->window_height * 0.5);
			SDL_WarpMouseInWindow(Game::getInstance()->window, center_x, center_y);
        
			this->mouse_position.x = center_x;
			this->mouse_position.y = center_y;
		}
	}*/
}