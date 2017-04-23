#pragma once
#include "Object.h"
#include "InputManager.h"

class Camera : public Object
{
private:
	Matrix projection;
	Matrix viewMatrix;
	bool flag;
public:
	Camera();
	~Camera();


	void MoveForBack(float deltaTime);
	void MoveLeftRight(float deltaTime);
	void MoveUpDown(float deltaTime);

	void Update(float frame);
	void Init3D(float fov, float asp, float setNear, float setFar, float speed);
	void Init2D(float left, float right, float bottom, float top, float nearr, float farr, float speed);

	Matrix CalcualteViewMatrix();
	Matrix& GetViewMatrix();
};

