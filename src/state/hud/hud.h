#ifndef HUD_H
#define HUD_H

#include "../../includes.h"
#include "../../framework.h"

#include "../../game.h"
#include "../../mesh.h"

class HUD
{
public:
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void update(double delta) = 0;

	void setQuad(Mesh* m, float size){
		this->setQuad(m, size, size);
	}

	void setQuad(Mesh* m, float x_size, float y_size){
		this->setQuad(m, x_size, y_size,
			Game::getInstance()->window_width / 2,
			Game::getInstance()->window_height / 2);
	}

	void setQuad(Mesh* m, float x_size, float y_size, float x, float y){
		this->setQuad(m, x_size, y_size,
			(int)(Game::getInstance()->window_width * x),
			(int)(Game::getInstance()->window_height * y));
	}

	void setQuad(Mesh* m, float x_size, float y_size, int x, int y){
		m->vertices.clear();
		float hxs = (float)x_size / 2;
		float hys = (float)y_size / 2;
		m->vertices.push_back( Vector3(x - hxs, y + hys, 0) );
		m->vertices.push_back( Vector3(x - hxs, y - hys, 0) );
		m->vertices.push_back( Vector3(x + hxs, y + hys, 0) );
		m->vertices.push_back( Vector3(x + hxs, y - hys, 0) );
		m->uvs.push_back( Vector2(0, 1) );
		m->uvs.push_back( Vector2(0, 0) );
		m->uvs.push_back( Vector2(1, 1) );
		m->uvs.push_back( Vector2(1, 0) );
		this->setQuadColor(m, Vector4(1,1,1,1));
	}

	void addQuad(Mesh* m, float x_size, float y_size, float x, float y){
		this->addQuad(m, x_size, y_size,
			(int)(Game::getInstance()->window_width * x),
			(int)(Game::getInstance()->window_height * y));
	}

	void addQuad(Mesh* m, float x_size, float y_size, int x, int y){
		//m->vertices.clear();
		float hxs = (float)x_size / 2;
		float hys = (float)y_size / 2;
		m->vertices.push_back( Vector3(x - hxs, y + hys, 0) );
		m->vertices.push_back( Vector3(x - hxs, y - hys, 0) );
		m->vertices.push_back( Vector3(x + hxs, y + hys, 0) );
		m->vertices.push_back( Vector3(x + hxs, y + hys, 0) );
		m->vertices.push_back( Vector3(x - hxs, y - hys, 0) );
		m->vertices.push_back( Vector3(x + hxs, y - hys, 0) );
		m->uvs.push_back( Vector2(0, 1) );
		m->uvs.push_back( Vector2(0, 0) );
		m->uvs.push_back( Vector2(1, 1) );
		m->uvs.push_back( Vector2(1, 1) );
		m->uvs.push_back( Vector2(0, 0) );
		m->uvs.push_back( Vector2(1, 0) );
	}

	void setQuadColor(Mesh* m, Vector4 color){
		m->colors.clear();
		m->colors.push_back(color);
		m->colors.push_back(color);
		m->colors.push_back(color);
		m->colors.push_back(color);
	}

	void moveQuad(Mesh* m, int x, int y){
		int i, size = m->vertices.size();
		for(i = 0; i < size; i++){
			m->vertices[i].x += x;
			m->vertices[i].y += y;
		}
	}

	void printText(std::string text, int x, int y){
		glColor3f(1, 1, 1);
		glRasterPos2f(x, y);
		for (int i = 0; i < text.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
};


#endif