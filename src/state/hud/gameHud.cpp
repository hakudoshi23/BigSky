#include "gameHud.h"

#include "../../includes.h"

#include "../../game.h"
#include "../../entity/entityPlane.h"

GameHUD::GameHUD() {
	this->camera = new Camera();
	this->camera->setOrthographic(
		0, Game::getInstance()->window_width, 
		0, Game::getInstance()->window_height,
		-1, 1);
	this->health = new Mesh();
	this->shield = new Mesh();
	this->crosshair = new Mesh();
	this->pointers = new Mesh();
}

GameHUD::~GameHUD() {
	delete this->health;
	delete this->shield;
	delete this->camera;
}

void GameHUD::init() {
	int hp_width = 150;
	int shield_width = 100;

	this->setQuad(this->health, hp_width, 10, 
		hp_width / 2 + 20,
		Game::getInstance()->window_height - 30);
	this->setQuadColor(this->health, Vector4(1,0,0,1));

	this->setQuad(this->shield, shield_width, 10, 
		shield_width / 2 + 20,
		Game::getInstance()->window_height - 50);
	this->setQuadColor(this->shield, Vector4(0,0,1,1));

	this->setQuad(this->crosshair, 50);
	this->moveQuad(this->crosshair, 0, 118);
	this->setQuadColor(this->crosshair, Vector4(0,0,0,0));
}
   
void GameHUD::render() {
	this->camera->set();
	Matrix44 mvp = this->camera->viewprojection_matrix;
	Shader* _s = Shader::Load("data/shaders/color");
	Shader* _s2 = Shader::Load("data/shaders/default");
	Shader* _st = Shader::Load("data/shaders/text");
	Texture* _r = Texture::load("data/textures/reticule_r.tga");
	Texture* _ch = Texture::load("data/textures/crosshair.tga");
	/* Render with colors */
	_s->enable();
	_s->setMatrix44("u_mvp", mvp );
	this->health->render(GL_TRIANGLE_STRIP, _s);
	this->shield->render(GL_TRIANGLE_STRIP, _s);
	_s->disable();
	/* Render with texture */
	_s2->enable();
	_s2->setMatrix44("u_mvp", mvp );
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_s2->setTexture("u_texture", _ch->texture_id);
	this->crosshair->render(GL_TRIANGLE_STRIP, _s2);
	if(!this->pointers->vertices.empty()) {
		_s2->setTexture("u_texture", _r->texture_id);
		this->pointers->render(GL_TRIANGLES, _s2);
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	_s2->disable();
}

void GameHUD::update(double delta) {
	this->pointers->clear();
	EntityPlane* player = (EntityPlane*) World::getInstance()->findByName("player");
	if(player != NULL){
		this->setQuad(this->health, (int)player->health, 10, 
			(int)player->health / 2 + 20,
			Game::getInstance()->window_height - 30);
		this->setQuadColor(this->health, Vector4(1,0,0,1));

		this->setQuad(this->shield, (int)player->shield, 10, 
			(int)player->shield / 2 + 20,
			Game::getInstance()->window_height - 50);
		this->setQuadColor(this->shield, Vector4(0,0,1,1));

		Vector3 playerPos = Vector3(player->model.m[12], player->model.m[13], player->model.m[14]);
		Vector3 f = player->model.rotateVector(Vector3(0, 0, 1)), _f;
		std::vector<Entity*> planes = World::getInstance()->findByTag("enemy");
		//TODO: fix vector calculations
		for(int i = 0; i < planes.size(); i++){
			Entity* e = planes[i];
			_f = Vector3(e->model.m[12], e->model.m[13], e->model.m[14]) - playerPos;
			_f.normalize();
			if(f.dot(_f) > 0){
				Vector3 pos = Vector3(e->model.m[12], e->model.m[13], e->model.m[14]);
				pos = World::getInstance()->camera->viewprojection_matrix.project(pos);
				this->addQuad(this->pointers, 50, 50, pos.x, pos.y);
			}
		}
	}
}