#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra\textparser.h"

std::map<std::string, Mesh*> Mesh::cache;

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffersARB(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffersARB(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffersARB(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffersARB(1, &colors_vbo_id);
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

	if(vertices_vbo_id)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id );
		glVertexPointer(3, GL_FLOAT, 0, NULL );
	}
	else
		glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		if(normals_vbo_id)
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id );
			glNormalPointer(GL_FLOAT, 0, NULL );
		}
		else
			glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if(uvs_vbo_id)
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id );
			glTexCoordPointer(2, GL_FLOAT, 0, NULL );
		}
		else
			glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0] );
	}

	if (colors.size())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		if(colors_vbo_id)
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id );
			glColorPointer(4, GL_FLOAT, 0, NULL );
		}
		else
			glColorPointer(4, GL_FLOAT, 0, &colors[0] );
	}
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (colors.size())
		glDisableClientState(GL_COLOR_ARRAY);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

void Mesh::render( int primitive, Shader* sh )
{
	if(!sh || !sh->compiled)
        return render(primitive);
    
	assert(vertices.size() && "No vertices in this mesh");

    int vertex_location = sh->getAttribLocation("a_vertex");
	assert(vertex_location != -1 && "No a_vertex found in shader");

	if(vertex_location == -1)
		return;

    glEnableVertexAttribArray(vertex_location);
	if(vertices_vbo_id)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id );
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	}
	else
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0] );

	int normal_location = -1;
    if (normals.size())
    {
        normal_location = sh->getAttribLocation("a_normal");
        if(normal_location != -1)
        {
            glEnableVertexAttribArray(normal_location);
			if(normals_vbo_id)
			{
				glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id );
	            glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, NULL );
			}
			else
	            glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, &normals[0] );
        }
    }
    
	int uv_location = -1;
	if (uvs.size())
	{
        uv_location = sh->getAttribLocation("a_uv");
        if(uv_location != -1)
        {
            glEnableVertexAttribArray(uv_location);
			if(uvs_vbo_id)
			{
				glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id );
	            glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, NULL );
			}
			else
	            glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0] );
        }
    }
    
	int color_location = -1;
	if (colors.size())
	{
        color_location = sh->getAttribLocation("a_color");
        if(color_location != -1)
        {
            glEnableVertexAttribArray(color_location);
			if(colors_vbo_id)
			{
				glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id );
	            glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, NULL );
			}
			else
	            glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, &colors[0] );
        }
    }
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );

	glDisableVertexAttribArray( vertex_location );
	if(normal_location != -1) glDisableVertexAttribArray( normal_location );
	if(uv_location != -1) glDisableVertexAttribArray( uv_location );
	if(color_location != -1) glDisableVertexAttribArray( color_location );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

void Mesh::buildCollisionModel(){
	int size = this->vertices.size();
	this->collision_model = newCollisionModel3D();
	this->collision_model->setTriangleNumber(size / 3);
	for(int i = 0; i < size; i+=3){
		this->collision_model->addTriangle(
			this->vertices[i+0].x, this->vertices[i+0].y, this->vertices[i+0].z,
			this->vertices[i+1].x, this->vertices[i+1].y, this->vertices[i+1].z,
			this->vertices[i+2].x, this->vertices[i+2].y, this->vertices[i+2].z
		);
	}
	this->collision_model->finalize();
}

void Mesh::uploadToVRAM()
{
	//delete old
	if(vertices_vbo_id) glDeleteBuffersARB(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffersARB(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffersARB(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffersARB(1, &colors_vbo_id);

	glGenBuffersARB( 1, &vertices_vbo_id ); //generate one handler (id)
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id ); //bind the handler
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW_ARB ); //upload data

	if(normals.size())
	{
		glGenBuffersARB( 1, &normals_vbo_id); //generate one handler (id)
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id ); //bind the handler
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW_ARB ); //upload data
	}

	if(uvs.size())
	{
		glGenBuffersARB( 1, &uvs_vbo_id); //generate one handler (id)
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id ); //bind the handler
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW_ARB ); //upload data
	}

	if(colors.size())
	{
		glGenBuffersARB( 1, &colors_vbo_id); //generate one handler (id)
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id ); //bind the handler
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, colors.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW_ARB ); //upload data
	}

}

void Mesh::createQuad(float center_x, float center_y, float w, float h, bool flip_uvs )
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );

	//texture coordinates
	uvs.push_back( Vector2(1.0f,flip_uvs ? 0.0f : 1.0f) );
	uvs.push_back( Vector2(0.0f,flip_uvs ? 1.0f : 0.0f) );
	uvs.push_back( Vector2(1.0f,flip_uvs ? 1.0f : 0.0f) );
	uvs.push_back( Vector2(0.0f,flip_uvs ? 0.0f : 1.0f) );
	uvs.push_back( Vector2(0.0f,flip_uvs ? 1.0f : 0.0f) );
	uvs.push_back( Vector2(1.0f,flip_uvs ? 0.0f : 1.0f) );

	//all of them have the same normal
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
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
	/* Check cache */
	std::map<std::string, Mesh*>::iterator it = cache.find(filename);
	if(it != cache.end()){
		return it->second;
	}

	/* Check BIN files */
	std::string binFile(filename);
	binFile = binFile.substr(0, binFile.rfind(".")) + ".bin";
	if (FILE *file=fopen(binFile.c_str(),"r")){
		Mesh* mesh = new Mesh();
		mesh->loadBinary(binFile.c_str());
		mesh->buildCollisionModel();
		fclose(file);
		cache[filename] = mesh;
		std::cout << " * Mesh [BIN]: " << binFile << std::endl;
		return mesh;
	}

	TextParser parser;

	if(!parser.create(filename)){
		std::cout << "Error loading Mesh: " << filename << std::endl;
		return NULL;
	}
	
	std::cout << " * Mesh: " << filename << std::endl;

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
 
	for(int i = 0; i<num_uvs;i++){
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

	for(int i = 0; i<num_faces;i++){
		parser.seek("*MESH_VERTEXNORMAL");
		parser.getint();
		float nx = parser.getfloat();
		float ny = parser.getfloat();
		float nz = parser.getfloat();
		mesh->normals.push_back(Vector3(nx, nz, -ny));
		parser.seek("*MESH_VERTEXNORMAL");
		parser.getint();
		nx = parser.getfloat();
		ny = parser.getfloat();
		nz = parser.getfloat();
		mesh->normals.push_back(Vector3(nx, nz, -ny));
		parser.seek("*MESH_VERTEXNORMAL");
		parser.getint();
		nx = parser.getfloat();
		ny = parser.getfloat();
		nz = parser.getfloat();
		mesh->normals.push_back(Vector3(nx, nz, -ny));
	}

	mesh->saveBinary(binFile.c_str());
 
	mesh->buildCollisionModel();
	mesh->uploadToVRAM();
	return mesh;
}

void Mesh::loadBinary(const char* filename){
	int s;
	FILE* f = fopen(filename,"rb");

	//Read Vertices
	fread(&s,sizeof(int),1, f);
	this->vertices.resize(s);
	fread(&this->vertices[0], sizeof(Vector3),s, f);

	//Read UVs
	fread(&s,sizeof(int),1, f);
	this->uvs.resize(s);
	fread(&this->uvs[0], sizeof(Vector2),s, f);

	//Read Normals
	fread(&s,sizeof(int),1, f);
	this->normals.resize(s);
	fread(&this->normals[0], sizeof(Vector3),s, f);

	fclose(f);
}

void Mesh::saveBinary(const char* filename){
	FILE* f = fopen(filename,"wb");

	//Write Vertices
	int s = vertices.size();
	fwrite(&s, sizeof(int),1, f);
	fwrite(&vertices[0], sizeof(Vector3),vertices.size(), f);

	//Write UVs
	s = uvs.size();
	fwrite(&s, sizeof(int),1, f);
	fwrite(&uvs[0], sizeof(Vector2),uvs.size(), f);

	//Write Normals
	s = normals.size();
	fwrite(&s, sizeof(int),1, f);
	fwrite(&normals[0], sizeof(Vector3),normals.size(), f);

	fclose(f);
}