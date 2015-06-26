#include "loseHud.h"

#include "../../includes.h"

#include "../../game.h"
#include "../menuState.h"
#include "../../sound/soundManager.h"

LoseHUD::LoseHUD(Camera* cam) {
	if(XBOX360::getInstance()->getNumJoysticks() > 0){
		this->joystick = XBOX360::getInstance()->openJoystick(0);
	} else this->joystick = NULL;
	this->keystate = NULL;

	this->camera = cam;
	this->portrait = new Mesh();
	this->title = new Mesh();
	this->text = new Mesh();
}

LoseHUD::~LoseHUD() {
	delete this->camera;
	delete this->title;
	delete this->text;
	delete this->keystate;
	delete this->joystick;
}

void LoseHUD::init() {
	this->setQuad(this->portrait, 256, 256, 0.2f, 0.4f);
	this->setQuad(this->title, 512, 128, 0.5f, 0.8f);
	this->setQuad(this->text, 512, 128, 0.7f, 0.5f);
	this->last_time = Game::getInstance()->time + 1;
}
   
void LoseHUD::render() {
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
	Texture* p = Texture::load("data/textures/lose/portrait.tga");
	Texture* ti = Texture::load("data/textures/lose/title.tga");
	Texture* te = Texture::load("data/textures/lose/text.tga");
	glDepthMask(GL_FALSE);
	/* Render with texture */
	_s2->enable();
	_s2->setMatrix44("u_mvp", mvp );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_s2->setTexture("u_texture", p->texture_id);
	this->portrait->render(GL_TRIANGLE_STRIP, _s2);
	_s2->setTexture("u_texture", ti->texture_id);
	this->title->render(GL_TRIANGLE_STRIP, _s2);
	_s2->setTexture("u_texture", te->texture_id);
	this->text->render(GL_TRIANGLE_STRIP, _s2);
	glDisable(GL_BLEND);
	_s2->disable();
	/* Render text */
	_st->enable();
	_s2->setMatrix44("u_mvp", mvp );
	this->printText("Pulsa Intro / Mando A para continuar...", 50, 50);
	_st->disable();
	glDepthMask(GL_TRUE);
}

void LoseHUD::update(double delta) {
	JoystickState state = XBOX360::getInstance()->getJoystickState(this->joystick);
	this->keystate = SDL_GetKeyboardState(NULL);

	if(Game::getInstance()->time - this->last_time > 1){
		if(this->keystate[SDL_SCANCODE_E] || this->keystate[SDL_SCANCODE_RETURN] || state.button[10]) {
			Game::swapState(new MenuState());
		}
	}
}