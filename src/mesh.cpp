#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra\textparser.h"

std::map<std::string, Mesh*> Mesh::cache;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(4, vbos);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
}

void Mesh::render(int primitive)
{
	assert(vertices.size() && "No vertices in this mesh");

    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT, 0, &uvs[0] );
	}

	if (colors.size())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT, 0, &colors[0] );
	}
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (colors.size())
		glDisableClientState(GL_COLOR_ARRAY);
}

void Mesh::render( int primitive, Shader* sh )
{
	if(!sh || !sh->compiled)
        return render(primitive);
    
	assert(vertices.size() && "No vertices in this mesh");

    int location = sh->getAttribLocation("a_vertex");
	assert(location != -1 && "No a_vertex found in shader");

	if(location == -1)
		return;

    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0] );

    if (normals.size())
    {
        location = sh->getAttribLocation("a_normal");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, &normals[0] );
        }
    }
    
	if (uvs.size())
	{
        location = sh->getAttribLocation("a_uv");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0] );
        }
    }
    
	if (colors.size())
	{
        location = sh->getAttribLocation("a_color");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, &colors[0] );
        }
    }
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );
    
}

void Mesh::renderVBO( int primitive, Shader* sh )
{
	if(!sh || !sh->compiled)
        return render(primitive);
    
	assert(vertices.size() && "No vertices in this mesh");

    int location = sh->getAttribLocation("a_vertex");
	assert(location != -1 && "No a_vertex found in shader");

	if(location == -1)
		return;

    glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (normals.size())
    {
        location = sh->getAttribLocation("a_normal");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
	if (uvs.size())
	{
        location = sh->getAttribLocation("a_uv");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
			glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
	if (colors.size())
	{
        location = sh->getAttribLocation("a_color");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::upload()
{
	assert(vertices.size() && "No vertices in this mesh");

	glGenBuffers(1, &vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), &vertices[0], GL_STATIC_DRAW);

	if (normals.size())
	{
		glGenBuffers(1, &vbos[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, normals.size()*3*sizeof(float), &normals[0], GL_STATIC_DRAW);
    }

	if (uvs.size())
	{
		glGenBuffers(1, &vbos[2]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
		glBufferData(GL_ARRAY_BUFFER, uvs.size()*2*sizeof(float), &uvs[0], GL_STATIC_DRAW);
    }

	if (colors.size())
	{
		glGenBuffers(1, &vbos[3]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
		glBufferData(GL_ARRAY_BUFFER, colors.size()*4*sizeof(float), &colors[0], GL_STATIC_DRAW);
    }
}

void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(size,0,-size) );
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(-size,0,size) );
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size,0,-size) );

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(1,0) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,0) );
}

Mesh* Mesh::load(const char* filename)
{
	std::map<std::string, Mesh*>::iterator it = cache.find(filename);
	if(it != cache.end()){
		return it->second;
	}

	TextParser parser;

	if(!parser.create(filename)){
		std::cout << "Error loading Mesh: " << filename << std::endl;
		return NULL;
	}

	Mesh* mesh = new Mesh();
	cache[filename] = mesh;

	parser.seek("*MESH_NUMVERTEX");
	int num_vertices = parser.getint();
	parser.seek("*MESH_NUMFACES");
	int num_faces = parser.getint();
 
	std::vector<Vector3> vertices_unicos;
	vertices_unicos.resize(num_vertices);
 
	for(int i = 0; i<num_vertices;i++){
		parser.seek("*MESH_VERTEX");
		parser.getint();
		float x = parser.getfloat();
		float y = parser.getfloat();
		float z = parser.getfloat();
		vertices_unicos[i] = Vector3(x,z,-y);
	}

	for(int i = 0; i<num_faces;i++){
		parser.seek("*MESH_FACE");
		parser.getword();
		parser.getword();
		int fx = parser.getint();
		parser.getword();
		int fy = parser.getint();
		parser.getword();
		int fz = parser.getint();
		mesh->vertices.push_back(vertices_unicos[fx]);
		mesh->vertices.push_back(vertices_unicos[fy]);
		mesh->vertices.push_back(vertices_unicos[fz]);
	}

	parser.seek("*MESH_NUMTVERTEX");
	int num_uvs = parser.getint();
 
	std::vector<Vector2> uvs_unicos;
	uvs_unicos.resize(num_uvs);
 
	for(int i = 0; i<num_vertices;i++){
		parser.seek("*MESH_TVERT");
		parser.getint();
		float u = parser.getfloat();
		float v = parser.getfloat();
		uvs_unicos[i] = Vector2(u,v);
	}

	parser.seek("*MESH_NUMTVFACES");
	int num_tfaces = parser.getint();

	for(int i = 0; i<num_faces;i++){
		parser.seek("*MESH_TFACE");
		parser.getint();
		int uvx = parser.getint();
		int uvy = parser.getint();
		int uvz = parser.getint();
		mesh->uvs.push_back(uvs_unicos[uvx]);
		mesh->uvs.push_back(uvs_unicos[uvy]);
		mesh->uvs.push_back(uvs_unicos[uvz]);
	}
 
	mesh->upload();
	return mesh;
}