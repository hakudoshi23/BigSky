#include "winState.h"

#include "../includes.h"
#include <iostream>

#include "hud/winHud.h"
#include "../sound/soundManager.h"

WinState::WinState(int score) {
	this->hud = new WinHUD(score);
	this->hud->init();
}

WinState::~WinState() {
	delete this->hud;
}

void WinState::init() {
	glClearColor(0, 0, 0, 1);
}
   
void WinState::render() {
	this->hud->render();
}

void WinState::update(double delta) {
	this->hud->update(delta);
}

void WinState::destroy() {
}