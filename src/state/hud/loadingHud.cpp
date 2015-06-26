#include "loadingHud.h"

#include "../../includes.h"

#include "../../game.h"
#include "../mainState.h"
#include "../../sound/soundManager.h"

LoadingHUD::LoadingHUD(Camera* cam, bool* ready) {
	if(XBOX360::getInstance()->getNumJoysticks() > 0){
		this->joystick = XBOX360::getInstance()->openJoystick(0);
	} else this->joystick = NULL;
	this->keystate = NULL;

	this->ready = ready;

	this->camera = cam;
	this->progress = new Mesh();
	this->mission = new Mesh();
	this->logo = new Mesh();
}

LoadingHUD::~LoadingHUD() {
	delete this->camera;
	delete this->progress;
	delete this->logo;

	delete this->keystate;
	delete this->joystick;
}

void LoadingHUD::init() {
	this->setQuad(this->progress, 500, 30, 0.5f, 0.2f);
	this->setQuad(this->mission, 512, 128, 0.5f, 0.5f);
	this->setQuad(this->logo, 512, 128, 0.5f, 0.8f);
}
   
void LoadingHUD::render() {
	this->camera->setOrthographic(
		0, Game::getInstance()->window_width, 
		0, Game::getInstance()->window_height,
		-1, 1);
	this->camera->set();
	Matrix44 mvp = this->camera->viewprojection_matrix;
	/* Shaders */
	Shader* _s = Shader::Load("data/shaders/color");
	Shader* _s2 = Shader::Load("data/shaders/default");
	Shader* _st = Shader::Load("data/shaders/text");
	/* Textures */
	Texture* l = Texture::load("data/textures/mainMenu/logo_alpha.tga");
	Texture* m = Texture::load("data/textures/mission.tga");
	glDepthMask(GL_FALSE);
	/* Render with colors */
	_s->enable();
	_s->setMatrix44("u_mvp", mvp );
	//this->progress->render(GL_TRIANGLE_STRIP, _s);
	_s->disable();
	/* Render with texture */
	_s2->enable();
	_s2->setMatrix44("u_mvp", mvp );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_s2->setTexture("u_texture", l->texture_id);
	this->logo->render(GL_TRIANGLE_STRIP, _s2);
	_s2->setTexture("u_texture", m->texture_id);
	this->mission->render(GL_TRIANGLE_STRIP, _s2);
	glDisable(GL_BLEND);
	_s2->disable();
	/* Render text */
	_st->enable();
	_s2->setMatrix44("u_mvp", mvp );
	this->printText(
		(*this->ready) 
		? "Pulsa Intro / Mando A para empezar!"
		: "Cargando...", 
		50, 50);
	_st->disable();
	glDepthMask(GL_TRUE);
}

void LoadingHUD::update(double delta) {
	JoystickState state = XBOX360::getInstance()->getJoystickState(this->joystick);
	this->keystate = SDL_GetKeyboardState(NULL);

	if(*this->ready){
		if(this->keystate[SDL_SCANCODE_E] || this->keystate[SDL_SCANCODE_RETURN] || state.button[10]) {
			Game::swapState(new MainState());
		}
	}
}