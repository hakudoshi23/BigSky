#include "menuState.h"

#include "../includes.h"
#include <iostream>

#include "hud/menuHud.h"
#include "../sound/soundManager.h"

MenuState::MenuState() {
	this->hud = new MenuHUD();
	this->hud->init();
}

MenuState::~MenuState() {
	delete this->hud;
}

void MenuState::init() {
	glClearColor(0.623529, 0.752941, 0.796078, 1.0);
	SoundManager::getInstance()->playSound("data/sound/main_menu_bg.wav", 0, true, 0.5f);
}
   
void MenuState::render() {
	this->hud->render();
}

void MenuState::update(double delta) {
	this->hud->update(delta);
}

void MenuState::destroy() {
	SoundManager::getInstance()->stop();
}