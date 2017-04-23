#pragma once
#include "Model.h"
#include "Shader.h"
#include <string.h>
#include"InputManager.h"
#include "ResourceManager.h"

class Object
{
protected:
	Vector3 scale, position, rotate;
	Matrix worldMatrix;
	Shader *shader;
	GLfloat speed;

	char *name;
public:
	Object();
	virtual ~Object();

	void SetRotate(Vector3 rotate);
	void SetScale(Vector3 scale);
	void SetPosition(Vector3 translation);
	void SetSpeed(GLfloat speed);
	void SetName(char *name);
	Vector3 GetRotate();
	Vector3 GetScale();
	Vector3 GetPosition();
	GLfloat GetSpeed();
	char *GetName();

	void MoveLeftRight(GLfloat deltaTime);
	void MoveUpDown(GLfloat deltaTime);
	void MoveForBack(GLfloat deltaTime);
	void RotateX(GLfloat deltaTime);
	void RotateY(GLfloat deltaTime);
	void RotateZ(GLfloat deltaTime);

	void SetShader(Shader *shader);

	virtual void Update(GLfloat deltaTime);

	Matrix CalculateWorldmatrix();
	virtual void Draw(Matrix viewMatrix);

};

