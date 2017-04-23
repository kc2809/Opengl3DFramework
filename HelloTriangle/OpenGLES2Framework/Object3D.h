#pragma once
#include "Object.h"
class Object3D : public Object
{
protected:
	Model **models;
	Texture **textures, **cubeTextures;
	GLuint numOfTextures, numOfCubeTextures, numOfModels , usingModel;

	Vector3 minVertex, maxVertex;
public:
	Object3D();
	virtual ~Object3D();

	void SetModel(Model *model, GLuint offset);
	void SetNumOfModels(GLuint numOfModels);
	void SetUsingModel(GLuint usingModel);
	void SetBox();
	void SetNumOfTextures(GLuint numOfTexture);
	void SetTexture(Texture *texture, GLuint offset);
	void SetNumOfCubeTextures(GLuint numOfCubeTexture);
	void SetCubeTexture(Texture *cubeTexture, GLuint offset);

	void Draw(Matrix &viewMatrix);

	void Update(GLfloat deltaTime);
	bool IsCollision(Object3D*);
	bool IsInObject(Object3D*);
	void printBox();

	Vector3 GetMinVertex();
	Vector3 GetMaxVertex();
};

