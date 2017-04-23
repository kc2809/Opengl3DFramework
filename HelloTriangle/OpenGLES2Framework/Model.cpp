#include "Model.h"

Model::Model()
{
	meshs = 0;
	nMesh = 0;
	drawArray = false;
	this->minVertex = Vector3(1000, 1000, 1000);
	this->maxVertex = Vector3(-1000, -1000, -1000);

	id = 0;
}


Model::~Model()
{
	if (meshs != 0)
	{
		delete[] meshs;
	}
	if (id != 0)
	{
		delete[] id;
	}
}

void Model::SetId(char *id)
{
	if (this->id != 0)
	{
		delete[] this->id;
	}
	this->id = new char[strlen(id) + 1];
	strcpy(this->id, id);
}

char *Model::GetId()
{
	return id;
}


Mesh *Model::GetMeshs(){
	return meshs;
}

GLuint Model::GetNMesh(){
	return nMesh;
}

bool Model::IsDrawArray(){
	return drawArray;
}

void Model::InitModel(char *filename)
{
	FILE *f;
	int numberVertices, numIndice;
	Vertex *verticesData;
	GLushort *indice;
	f = fopen(filename, "r");

	if (f != 0){
		fscanf(f, "NrVertices: %d\n", &numberVertices);
		verticesData = new Vertex[numberVertices];
		for (int i = 0; i < numberVertices; i++){
			fscanf(f, "%*d. pos:[%f,%f,%f]; norm:[%f,%f,%f]; binorm:[%f,%f,%f]; tgt:[%f,%f,%f]; uv:[%f,%f];\n",
				&verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z,
				&verticesData[i].norm.x, &verticesData[i].norm.y, &verticesData[i].norm.z,
				&verticesData[i].binorm.x, &verticesData[i].binorm.y, &verticesData[i].binorm.z,
				&verticesData[i].tgt.x, &verticesData[i].tgt.y, &verticesData[i].tgt.z,
				&verticesData[i].uv.x, &verticesData[i].uv.y);

			minVertex.x = minVertex.x > verticesData[i].pos.x ? verticesData[i].pos.x : minVertex.x;
			minVertex.y = minVertex.y > verticesData[i].pos.y ? verticesData[i].pos.y : minVertex.y;
			minVertex.z = minVertex.z > verticesData[i].pos.z ? verticesData[i].pos.z : minVertex.z;
			maxVertex.x = maxVertex.x < verticesData[i].pos.x ? verticesData[i].pos.x : maxVertex.x;
			maxVertex.y = maxVertex.y < verticesData[i].pos.y ? verticesData[i].pos.y : maxVertex.y;
			maxVertex.z = maxVertex.z < verticesData[i].pos.z ? verticesData[i].pos.z : maxVertex.z;
		}

		fscanf(f, "NrIndices: %d\n", &numIndice);
		indice = new GLushort[numIndice];
		for (int i = 0; i < numIndice; i += 3){
			fscanf(f, "%*d.%hu,%hu,%hu\n", &indice[i], &indice[i + 1], &indice[i + 2]);
		}

		meshs = new Mesh[1];
		nMesh = 1;
		this->Init(verticesData, indice, numberVertices, numIndice, 0);
		delete[] verticesData;
		delete[] indice;

		fclose(f);
	}
	printf("\n[Sucessfully] Loaded file %s", filename);
}

void Model::Init(Vertex *verticesData, GLushort *indice, unsigned int numberVertices, unsigned int numIndice, GLshort offset)
{
	meshs[offset].nVertices = numberVertices;
	meshs[offset].nIndice = numIndice;

	glGenBuffers(1, &meshs[offset].vboId);
	glBindBuffer(GL_ARRAY_BUFFER, meshs[offset].vboId);
	glBufferData(GL_ARRAY_BUFFER, numberVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &meshs[offset].iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshs[offset].iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndice * sizeof(GLushort), indice, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::Init(Vertex *verticesData, unsigned int numberVertices, GLshort offset)
{
	meshs[offset].nVertices = numberVertices;

	glGenBuffers(1, &meshs[offset].vboId);
	glBindBuffer(GL_ARRAY_BUFFER, meshs[offset].vboId);
	glBufferData(GL_ARRAY_BUFFER, numberVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	drawArray = true;
}

bool Model::LoadObj(char *path)
{
	unsigned int offset = -1;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;

	bool read = false;
	std::vector<Mesh> temp_mesh;
	std::vector<unsigned int> indiceMesh;
	std::vector<unsigned int> temp_offsetmesh;

	char string[128];

	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		return false;
	}

	while (true)
	{

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "mtllib") == 0){
			fscanf(file, "%s\n", string);
			temp_mesh = LoadMTL(path, string);
		}

		if (strcmp(lineHeader, "usemtl") == 0)
		{
			if (read)
			{
				indiceMesh.push_back(vertexIndices.size());
				temp_offsetmesh.push_back(offset);
			}

			fscanf(file, "%s\n", string);

			offset = 0;
			read = true;
			while (strcmp(temp_mesh[offset].name, string) != 0){
				offset++;
				if (offset > temp_mesh.size())
				{
					read = false;
					break;
				}
			}
			
		}
		else if (strcmp(lineHeader, "v") == 0)
		{
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			//Get box contain
			minVertex.x = minVertex.x > vertex.x ? vertex.x : minVertex.x;
			minVertex.y = minVertex.y > vertex.y ? vertex.y : minVertex.y;
			minVertex.z = minVertex.z > vertex.z ? vertex.z : minVertex.z;
			maxVertex.x = maxVertex.x < vertex.x ? vertex.x : maxVertex.x;
			maxVertex.y = maxVertex.y < vertex.y ? vertex.y : maxVertex.y;
			maxVertex.z = maxVertex.z < vertex.z ? vertex.z : maxVertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("File can't be read by our simple parser \n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	if (offset >= 0)
	{
		indiceMesh.push_back(vertexIndices.size());
		temp_offsetmesh.push_back(offset);
	}

	nMesh = temp_offsetmesh.size();
	meshs = new Mesh[nMesh];

	unsigned int index = 0;

	for (unsigned int j = 0; j < temp_offsetmesh.size(); j++){

		Vertex *verticesData;
		unsigned int numVertices = indiceMesh[j] - index;

		verticesData = new Vertex[numVertices];
		meshs[j] = temp_mesh[temp_offsetmesh[j]];
		// For each vertex of each triangle
		for (unsigned int i = index; i < indiceMesh[j]; i++){

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			verticesData[i - index].pos = temp_vertices[vertexIndex - 1];
			verticesData[i - index].uv = temp_uvs[uvIndex - 1];
			verticesData[i - index].norm = temp_normals[normalIndex - 1];
		}
	
		Init(verticesData, numVertices, j);

		index = indiceMesh[j];
		delete[] verticesData;

	}
	printf("\n[Sucessfully] Loaded file %s", path);
	printf("\nBox: %f %f %f - %f %f %f\n", minVertex.x, minVertex.y, minVertex.z,
		maxVertex.x, maxVertex.y, maxVertex.z);
	return true;
}

std::vector<Mesh> Model::LoadMTL(char *path, char *pathfile){
	char filename[128];
	strcpy(filename, path);

	unsigned int offset = strlen(path);
	while (path[offset-1] != '/' && offset > 0){
		offset--;
	}
	
	strcpy(filename + offset, pathfile);

	std::vector<Mesh> temp_mesh;

	FILE * file = fopen(filename, "r");
	if (file == NULL){
		printf("Impossible to open the file %s !\n",filename);
	}

	Mesh *mesh = 0;
	while (1){

		char lineHeader[128], string[128];
		
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // EOF = End Of File. Quit the loop.
		}
		// else : parse lineHeader
		
		if (strcmp(lineHeader, "newmtl") == 0)
		{
			if (mesh)
			{
				temp_mesh.push_back(*mesh);
				delete mesh;
			}
			mesh = new Mesh;
			fscanf(file, "%s\n", string);
			strcpy(mesh->name, string);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0)
		{
			fscanf(file, "%s\n", string);
			unsigned int i = strlen(path);
			
			strcpy(filename + offset, string);
			mesh->texture.Init2DTextures(filename, REPEAT);

		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}
	if (mesh)
	{
		temp_mesh.push_back(*mesh);
		delete mesh;
	}
	return temp_mesh;
}

Vector3 Model::GetMinVertex()
{
	return minVertex;
}


Vector3 Model::GetMaxVertex()
{
	return maxVertex;
}
