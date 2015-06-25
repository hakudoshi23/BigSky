#include "controlsState.h"

#include "../includes.h"
#include <iostream>

#include "hud/controlsHud.h"
#include "../sound/soundManager.h"

ControlsState::ControlsState() {
	this->hud = new ControlsHUD();
	this->hud->init();
}

ControlsState::~ControlsState() {
	delete this->hud;
}

void ControlsState::init() {
	glClearColor(0, 0, 0, 1);
}
   
void ControlsState::render() {
	this->hud->render();
}

void ControlsState::update(double delta) {
	this->hud->update(delta);
}

void ControlsState::destroy() {
}