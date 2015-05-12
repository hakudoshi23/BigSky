#include "world.h"
#include "../utils.h"
#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

#include "../entity/entityMesh.h"
#include "../entity/entitySkybox.h"

#include <cmath>

World::World() : Entity("world")
{
	this->clipper = new Clipper();
	this->terrain = NULL;
	this->skybox = NULL;
}

World::~World()
{
	std::cout << "Detroy World!" << std::endl;
}

void World::setCamera(Camera* camera)
{
	this->camera = camera;
}

void World::init()
{
	Shader* shader = new Shader();
	if( !shader->load("data/shaders/simple.vs","data/shaders/simple.fs") )
	{
		std::cout << "shader not found or error" << std::endl;
		exit(0);
	}

	this->skybox = new EntitySkybox();
	this->skybox->setMesh("data/mesh/skybox/cubemap.ASE");
	this->skybox->setTexture("data/mesh/skybox/cielo.tga");
	this->skybox->setShader(shader);
	this->addChild(this->skybox);

	this->terrain = new EntityMesh("terrain");
	this->terrain->setMesh("data/mesh/island/island.ASE");
	this->terrain->setTexture("data/mesh/island/island_color.tga");
	this->terrain->frustum_culling = false;
	this->terrain->setShader(shader);
	this->addChild(this->terrain);

	int i;
	EntityMesh* e;
	for(i = 0; i<100;i++){
		e = new EntityMesh("plane");
		e->setMesh("data/mesh/spitfire/spitfire.ASE");
		e->setTexture("data/mesh/spitfire/spitfire_color_spec.tga");
		e->setShader(shader);
		e->model.setTranslation(rand() % 200-100, 1500 + rand() % 200-100, 1400 + rand() % 200-100);
		this->addChild(e);
	}
}

void World::render()
{
	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	camera->set();
	this->clipper->ExtractFrustum(this->camera);

	drawGrid(500);

	this->skybox->model.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);

	this->processEvent("render");
}

void World::update(double delta)
{
	this->processEvent("update", &delta);
}

void World::processEvent(std::string name, void* data)
{
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		(*it)->processEvent(name, data);
}

Entity* World::findByID(int id)
{
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		if((*it)->id == id) return *it;
	return NULL;
}

Entity* World::findByName(std::string name)
{
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		if((*it)->name == name) return *it;
	return NULL;
}

