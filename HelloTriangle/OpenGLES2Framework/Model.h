#pragma once
#include<stddef.h>
#include "Texture.h"
#include "Vertex.h"

struct Mesh{
	GLuint vboId, iboId, nVertices, nIndice;
	Texture texture;
	Vector3 kd;
	char name[50];
};

class Model
{
private:
	Mesh *meshs;
	GLuint nMesh;
	bool drawArray;
	Vector3 minVertex, maxVertex;
	std::vector<Mesh> LoadMTL(char *path, char *pathfile);

	char *id;
public:
	Model();
	~Model();

	void SetId(char *id);
	char *GetId();

	Mesh *GetMeshs();
	GLuint GetNMesh();
	bool IsDrawArray();

	void InitModel(char *filename);
	void Init(Vertex *verticesData, GLushort *indice, unsigned int numberVertices, unsigned int numberIndice, GLshort offset);
	void Init(Vertex *verticesData, unsigned int numberVertices, GLshort offset);
	bool LoadObj(char *path);
	
	Vector3 GetMinVertex();
	Vector3 GetMaxVertex();
};

