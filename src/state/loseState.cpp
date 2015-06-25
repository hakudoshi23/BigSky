#include "loseState.h"

#include "../includes.h"
#include <iostream>

#include "hud/loseHud.h"
#include "../sound/soundManager.h"

LoseState::LoseState() {
	this->hud = new LoseHUD();
	this->hud->init();
}

LoseState::~LoseState() {
	delete this->hud;
}

void LoseState::init() {
	glClearColor(0, 0, 0, 1);
}
   
void LoseState::render() {
	this->hud->render();
}

void LoseState::update(double delta) {
	this->hud->update(delta);
}

void LoseState::destroy() {
}