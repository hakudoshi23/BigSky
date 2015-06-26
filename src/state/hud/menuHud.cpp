#include "menuHud.h"

#include "../../includes.h"

#include "../../game.h"
#include "../loadingState.h"
#include "../controlsState.h"
#include "../../sound/soundManager.h"

MenuHUD::MenuHUD(Camera* cam) {
	if(XBOX360::getInstance()->getNumJoysticks() > 0){
		this->joystick = XBOX360::getInstance()->openJoystick(0);
	} else this->joystick = NULL;
	this->keystate = NULL;

	this->options.push_back("Nueva Partida");
	this->options.push_back("Controles");
	this->options.push_back("Salir");
	this->options_increment = 40;
	this->index_change_cd = 0.2f;
	this->last_index_change = 0;
	this->current_index = 0;
	this->x_offset = 200;
	this->y_offset = 340;

	this->camera = cam;
	this->background = new Mesh();
	this->pointer = new Mesh();
	this->logo = new Mesh();
}

MenuHUD::~MenuHUD() {
	delete this->camera;
	delete this->background;
	delete this->pointer;
	delete this->logo;

	delete this->keystate;
	delete this->joystick;
}

void MenuHUD::init() {
	int hp_width = 150;
	int shield_width = 100;

	this->setQuad(this->background, 
		Game::getInstance()->window_width, 
		Game::getInstance()->window_height, 
		0.5f, 0.5f);
	this->setQuad(this->pointer, 32, 32,
		Game::getInstance()->window_width - this->x_offset - 30,
		Game::getInstance()->window_height - this->y_offset + 5);
	this->setQuad(this->logo, 512, 128,
		Game::getInstance()->window_width - 200,
		Game::getInstance()->window_height - 80);
	
	this->optionIndexChange();
	this->last_index_change = Game::getInstance()->time + 1;
}
   
void MenuHUD::render() {
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
	Texture* bg = Texture::load("data/textures/mainMenu/background.tga");
	Texture* p = Texture::load("data/textures/mainMenu/pointer.tga");
	Texture* l = Texture::load("data/textures/mainMenu/logo_alpha.tga");
	glDepthMask(GL_FALSE);
	/* Render with texture */
	_s2->enable();
	_s2->setMatrix44("u_mvp", mvp );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_s2->setTexture("u_texture", bg->texture_id);
	this->background->render(GL_TRIANGLE_STRIP, _s2);
	_s2->setTexture("u_texture", p->texture_id);
	this->pointer->render(GL_TRIANGLE_STRIP, _s2);
	_s2->setTexture("u_texture", l->texture_id);
	this->logo->render(GL_TRIANGLE_STRIP, _s2);
	glDisable(GL_BLEND);
	_s2->disable();
	/* Render text */
	_st->enable();
	_s2->setMatrix44("u_mvp", mvp );
	this->printText("v1.0.2",
		Game::getInstance()->window_width - 70, 20);
	int posY = Game::getInstance()->window_height - this->y_offset, 
		posX = Game::getInstance()->window_width - this->x_offset;
	for(int i = 0; i < this->options.size(); i++){
		this->printText(this->options[i], posX, 
			Game::getInstance()->window_height - (posY + this->options_increment * i));
	}
	_st->disable();
	glDepthMask(GL_TRUE);
}

void MenuHUD::update(double delta) {
	JoystickState state = XBOX360::getInstance()->getJoystickState(this->joystick);
	this->keystate = SDL_GetKeyboardState(NULL);

	if(Game::getInstance()->time - this->last_index_change > this->index_change_cd){
		if(this->keystate[SDL_SCANCODE_W] || state.button[0]) {
			this->last_index_change = Game::getInstance()->time;
			this->current_index--;
			if(this->current_index < 0) this->current_index = this->options.size() - 1;
			this->optionIndexChange();
		}
		if(this->keystate[SDL_SCANCODE_S] || state.button[1]) {
			this->last_index_change = Game::getInstance()->time;
			this->current_index++;
			if(this->current_index >= this->options.size()) this->current_index = 0;
			this->optionIndexChange();
		}
		if(this->keystate[SDL_SCANCODE_E] || this->keystate[SDL_SCANCODE_RETURN] || state.button[10]) {
			this->last_index_change = Game::getInstance()->time;
			this->enter();
		}
	}
}

void MenuHUD::optionIndexChange() {
	SoundManager::getInstance()->playSound("data/sound/main_menu_button.wav");
	this->setQuad(this->pointer, 32, 32,
		Game::getInstance()->window_width - this->x_offset - 30,
		Game::getInstance()->window_height - this->y_offset + 5 + 
		(this->options_increment * (this->options.size() - this->current_index - 1)));
}

void MenuHUD::enter() {
	switch(this->current_index){
		case 0:
			std::cout << "Nueva Partida" << std::endl;
			Game::swapState(new LoadingState());
			break;
		case 1:
			std::cout << "Controles" << std::endl;
			Game::swapState(new ControlsState());
			break;
		case 2:
			std::cout << "Salir" << std::endl;
			Game::swapState(NULL);
			break;
	}
}