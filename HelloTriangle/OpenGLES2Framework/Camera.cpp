#include "Camera.h"

Camera::Camera()
{
	flag = true;
}

Camera::~Camera()
{
}

void Camera::MoveLeftRight(GLfloat deltaTime)
{
	Vector4 lr = Vector4(deltaTime*speed, 0, 0, 0);
	Vector4 lrW = worldMatrix*lr;
	position = position + Vector3(lrW.x, lrW.y, lrW.z);
	flag = true;
}


void Camera::MoveUpDown(GLfloat deltaTime)
{
	Vector4 ud = Vector4(0, deltaTime*speed, 0, 0);
	Vector4 udW = worldMatrix*ud;
	position = position + Vector3(udW.x, udW.y, udW.z);
	flag = true;
}


void Camera::MoveForBack(GLfloat deltaTime)
{
	Vector4 fb = Vector4(0, 0, deltaTime*speed, 0);
	Vector4 fbW = worldMatrix*fb;
	position = position + Vector3(fbW.x, fbW.y, fbW.z);
	flag = true;
}

void Camera::Init3D(float fov, float aspect, float setNear, float setFar, float speed){
	projection.SetPerspective(fov, aspect, setNear, setFar);
	this->speed = speed;
}

void Camera::Init2D(float left, float right, float bottom, float top, float nearr, float farr, float speed)
{
	projection.SetOrtho(left, right, bottom, top, nearr, farr);
	this->speed = speed;
}


void Camera::Update(GLfloat deltaTime)
{
	//----------------------------------------------------------
	///A, W, D, S to move camera left, forward, right, backward
	///E, X to move camera up, down
	///<-, ^, ->, v to rotate camera
	///---------------------------------------------------------

	if (INPUT->IsPressed(A))
	{
		MoveLeftRight(-deltaTime*speed);
	}
	if (INPUT->IsPressed(W))
	{
		MoveForBack(-deltaTime*speed);
	}
	if (INPUT->IsPressed(D))
	{
		MoveLeftRight(deltaTime*speed);
	}
	if (INPUT->IsPressed(S)){
		MoveForBack(deltaTime*speed);
	}
	if (INPUT->IsPressed(E))
	{
		MoveUpDown(deltaTime*speed);
	}
	if (INPUT->IsPressed(X))
	{
		MoveUpDown(-deltaTime*speed);
	}
	if (INPUT->IsPressed(LEFT))
	{
		RotateY(deltaTime*speed);
		flag = true;
	}
	if (INPUT->IsPressed(UP))
	{
		RotateX(deltaTime*speed);
		flag = true;
	}
	if (INPUT->IsPressed(RIGHT))
	{
		RotateY(-deltaTime*speed);
		flag = true;
	}
	if (INPUT->IsPressed(DOWN))
	{
		RotateX(-deltaTime*speed);
		flag = true;
	}
	// flag = true then calculate viewmatrix again
	
}

Matrix Camera::CalcualteViewMatrix(){
	Matrix matrixRotateX, matrixRotateY, matrixTranslation;
	matrixRotateX.SetRotationX(-rotate.x);
	matrixRotateY.SetRotationY(-rotate.y);
	matrixTranslation.SetTranslation(-position);
	return matrixRotateX*matrixRotateY*matrixTranslation;
}


Matrix& Camera::GetViewMatrix()
{
	if (flag)
	{
		viewMatrix = projection * CalcualteViewMatrix();
		flag = false;
	}
	return viewMatrix;
}