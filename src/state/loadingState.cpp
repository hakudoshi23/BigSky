#include "loadingState.h"

#include "../includes.h"
#include <iostream>

#include "../texture.h"
#include "hud/loadingHud.h"
#include "../sound/soundManager.h"

LoadingState::LoadingState() {
	this->hud = new LoadingHUD(&this->done);
	this->hud->init();
	this->done = false;
}

LoadingState::~LoadingState() {
	delete this->hud;
}

void LoadingState::init() {
	glClearColor(0, 0, 0, 1);
}
   
void LoadingState::render() {
	this->hud->render();
}

void LoadingState::update(double delta) {
	this->hud->update(delta);

	//TODO: Fixme
	/* This does some weird things... */
	if(this->hud != NULL && !this->done){
		Mesh::load("data/mesh/x3_runner/x3_runner.ASE");
		Mesh::load("data/mesh/x3_fighter/x3_fighter.ASE");
		Mesh::load("data/mesh/asteroid/asteroides.ASE");
		Mesh::load("data/mesh/frigate/frigate.ASE");

		Texture::load("data/mesh/x3_runner/x3_runner.tga");
		Texture::load("data/mesh/x3_fighter/x3_fighter.tga");
		Texture::load("data/mesh/asteroid/asteroide.tga");
		Texture::load("data/mesh/frigate/frigate.tga");

		Shader::Load("data/shaders/phong");

		this->done = true;
	}
}

void LoadingState::destroy() {
	SoundManager::getInstance()->stop();
}