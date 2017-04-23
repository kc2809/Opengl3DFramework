#pragma once
#include "Object3D.h"
#include "Camera.h"

#define g 9.81f

const Vector3 forceThrowEgg(40.0f, 40.0f, -20.0f);

class Egg :
	public Object3D
{
private:
	float time;
	Vector3 beginPos;
	Vector2 forceEgg;
public:
	Egg(Vector3 pos);
	Egg();
	~Egg();

	void SetRotateEgg(Vector3 &postion, Vector4 diff, Vector3 &rotate);
	void SetForceEgg(Vector2 force);
	void Throw(GLfloat deltaTime);
	void SetThrow();
	void DrawTrajectory(Vector2 force, Matrix &viewMatrix, Object3D *cube, Object3D *items, GLuint nitems);
};

