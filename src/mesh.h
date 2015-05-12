/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"
#include <map>

class Shader;

class Mesh
{
public:

	static std::map<std::string, Mesh*> cache;

	unsigned int vbos[4];

	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors;	 //here we store colors by vertex

	Mesh();
	~Mesh();
	void clear();
	void render(int primitive);
	void render(int primitive, Shader* sh);
	void renderVBO(int primitive, Shader* sh);
	void upload();
	
	void createPlane(float size);

	static Mesh* load(const char* filename);
};

#endif