#include "controlsHud.h"

#include "../../includes.h"

#include "../../game.h"
#include "../menuState.h"
#include "../../sound/soundManager.h"

ControlsHUD::ControlsHUD(Camera* cam) {
	if(XBOX360::getInstance()->getNumJoysticks() > 0){
		this->joystick = XBOX360::getInstance()->openJoystick(0);
	} else this->joystick = NULL;
	this->keystate = NULL;

	this->camera = cam;
	this->background = new Mesh();
}

ControlsHUD::~ControlsHUD() {
	delete this->camera;
	delete this->background;
	delete this->keystate;
	delete this->joystick;
}

void ControlsHUD::init() {
	int height = (512.0f / 1024.0f) * Game::getInstance()->window_width;
	this->setQuad(this->background, 
		Game::getInstance()->window_width, 
		height, 
		0.5f, 0.5f);
	this->last_time = Game::getInstance()->time + 1;
}
   
void ControlsHUD::render() {
	this->camera->setOrthographic(
		0, Game::getInstance()->window_width, 
		0, Game::getInstance()->window_height,
		-1, 1);
	this->camera->set();
	Matrix44 mvp = this->camera->viewprojection_matrix;
	/* Shaders */
	Shader* _s2 = Shader::Load("data/shaders/default");
	Shader* _st = Shader::Load("data/shaders/text");
	/* Textures */
	Texture* bg = Texture::load("data/textures/controls/bg.tga");
	glDepthMask(GL_FALSE);
	/* Render with texture */
	_s2->enable();
	_s2->setMatrix44("u_mvp", mvp );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_s2->setTexture("u_texture", bg->texture_id);
	this->background->render(GL_TRIANGLE_STRIP, _s2);
	glDisable(GL_BLEND);
	_s2->disable();
	/* Render text */
	_st->enable();
	_s2->setMatrix44("u_mvp", mvp );
	this->printText("Pulsa Intro / Mando A para volver al Menu", 50, 50);
	_st->disable();
	glDepthMask(GL_TRUE);
}

void ControlsHUD::update(double delta) {
	JoystickState state = XBOX360::getInstance()->getJoystickState(this->joystick);
	this->keystate = SDL_GetKeyboardState(NULL);

	if(Game::getInstance()->time - this->last_time > 1){
		if(this->keystate[SDL_SCANCODE_E] || this->keystate[SDL_SCANCODE_RETURN] || state.button[10]) {
			Game::swapState(new MenuState());
		}
	}
}