#include "entityFactory.h"

#include "entityParticle.h"
#include "../controller/ai/aiController.h"
#include "../controller/playerController.h"

EntityPlane* EntityFactory::newEnemy() {
	EntityPlane* e = new EntityPlane("enemy");
	e->setMesh("data/mesh/x3_runner/x3_runner.ASE");
	e->setTexture("data/mesh/x3_runner/x3_runner.tga");
	e->setShader("data/shaders/phong");
	e->tags.push_back("enemy");
	e->tags.push_back("plane");

	AIController* c = new AIController();
	c->entity_id = e->id;
	c->init();
	e->controller = c;

	return e;
}

EntityPlane* EntityFactory::newPlayer() {
	EntityPlane* e = new EntityPlane("player");
	e->max_move->set(0.01, 0.03, 0.02);
	e->setMesh("data/mesh/x3_fighter/x3_fighter.ASE");
	e->setTexture("data/mesh/x3_fighter/x3_fighter.tga");
	e->setShader("data/shaders/phong");
	e->tags.push_back("player");
	e->tags.push_back("plane");

	e->max_health = 150;
	e->health = e->max_health;
	e->max_shield = 50;
	e->shield = e->max_shield;

	PlayerController* c = new PlayerController();
	c->entity_id = e->id;
	c->init();
	e->controller = c;

	EntityParticle* ep = new EntityParticle();
	ep->texture = "data/textures/smoke.tga";
	Vector3 f = e->model.rotateVector(Vector3(0, 0, -9));
	ep->model.setTranslation(f.x, f.y, f.z);
	ep->color = new Vector4(1,0,0,1);
	ep->random = 2;
	ep->freq = 0.01;
	ep->ttl = 0.5;
	e->addChild(ep);

	return e;
}

EntityMesh* EntityFactory::newAsteroid(int type) {
	EntityMesh* e = new EntityMesh("asteroid");
	switch(type){
		case 0:
			e->setMesh("data/mesh/asteroid/asteroides.ASE");
			break;
		case 1:
			e->setMesh("data/mesh/asteroid/asteroide1.ASE");
			break;
		case 2:
			e->setMesh("data/mesh/asteroid/asteroide2.ASE");
			break;
		case 3:
			e->setMesh("data/mesh/asteroid/asteroide3.ASE");
			break;
	}
	e->setTexture("data/mesh/asteroid/asteroide.tga");
	e->setShader("data/shaders/phong");
	e->tags.push_back("asteroid");
	e->tags.push_back("terrain");
	e->frustum_culling = false;
	return e;
}

EntityMesh* EntityFactory::newPlanet() {
	EntityMesh* e = new EntityMesh("planet");
	e->setMesh("data/mesh/planet/sphere.ASE");
	e->setTexture("data/mesh/planet/craters.tga");
	e->setShader("data/shaders/phong");
	e->tags.push_back("terrain");
	e->tags.push_back("planet");
	e->frustum_culling = false;

	return e;
}

EntityMesh* EntityFactory::newFrigate() {
	EntityMesh* e = new EntityMesh("frigate");
	e->setMesh("data/mesh/frigate/frigate.ASE");
	e->setTexture("data/mesh/frigate/frigate.tga");
	e->setShader("data/shaders/phong");
	e->tags.push_back("frigate");
	e->frustum_culling = false;

	return e;
}

EntitySky* EntityFactory::newSky() {
	EntitySky* e = new EntitySky();
	e->setMesh("data/mesh/cubemap/box.ASE");
	e->setTexture("data/mesh/cubemap/stars.tga");
	e->setShader("data/shaders/default");
	e->frustum_culling = false;
	e->depth_test = false;
	e->tags.push_back("sky");
	return e;
}