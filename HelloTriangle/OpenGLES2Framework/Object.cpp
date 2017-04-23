#include "Object.h"


Object::Object()
{
	scale = Vector3(1.0f, 1.0f, 1.0f);
	worldMatrix.SetIdentity();
	shader = 0;
	speed = 0.0f;
	name= 0;
}


Object::~Object()
{
	if (name){
		delete[] name;
		name = 0;
	}
}

void Object::SetRotate(Vector3 rotate)
{
	this->rotate = rotate;
}

void Object::SetScale(Vector3 scale)
{
	this->scale = scale;
}

void Object::SetPosition(Vector3 position)
{
	this->position = position;
}

void Object::SetSpeed(GLfloat speed){
	this->speed = speed;
}

void Object::SetName(char *name){
	if (this->name)
	{
		delete[] this->name;
		this->name = 0;
	}
	this->name = new char[strlen(name)+1];
	strcpy(this->name, name);
}

Vector3 Object::GetRotate()
{
	return rotate;
}

Vector3 Object::GetScale()
{
	return scale;
}

Vector3 Object::GetPosition()
{
	return position;
}

GLfloat Object::GetSpeed(){
	return speed;
}

char *Object::GetName(){
	return name;
}

void Object::SetShader(Shader *shader)
{
	this->shader = shader;
}

//-------------------
/// MOVE OBJECT
//-------------------
void Object::MoveLeftRight(GLfloat deltaTime)
{
	position.x += deltaTime*speed;
}


void Object::MoveUpDown(GLfloat deltaTime)
{
	position.y += deltaTime*speed;
}


void Object::MoveForBack(GLfloat deltaTime)
{
	position.z += deltaTime*speed;
}


//-------------------
/// ROTATE OBJECT
//-------------------
void Object::RotateX(GLfloat deltaTime)
{
	rotate.x += deltaTime*speed;
}


void Object::RotateY(GLfloat deltaTime)
{
	rotate.y += deltaTime*speed;
}


void Object::RotateZ(GLfloat deltaTime)
{
	rotate.z += deltaTime*speed;
}


void Object::Update(GLfloat deltaTime){
	///---------------------------------------------------------
	///J, I, L, K to move object left, forward, right, backward
	///N, M to rotate object left right
	//----------------------------------------------------------

	if (InputManager::GetInstance()->IsPressed(LEFT)){
		RotateY(deltaTime);
	}
	if (InputManager::GetInstance()->IsPressed(UP)){
		RotateX(deltaTime);
	}
	if (InputManager::GetInstance()->IsPressed(DOWN)){
		RotateX(-deltaTime);
	}
	if (InputManager::GetInstance()->IsPressed(RIGHT)){
		RotateY(-deltaTime);
	}
}


Matrix Object::CalculateWorldmatrix()
{
	Matrix  scaleMatrix, translationMatrix, rotationXMatrix, rotationYMatrix, rotationZMatrix;
	scaleMatrix.SetScale(scale);
	translationMatrix.SetTranslationVector(position);
	rotationXMatrix.SetRotationX(rotate.x);
	rotationYMatrix.SetRotationY(rotate.y);
	rotationZMatrix.SetRotationZ(rotate.z);
	return worldMatrix = translationMatrix * rotationYMatrix * rotationXMatrix * rotationZMatrix * scaleMatrix;
}

void Object::Draw(Matrix viewMatrix)
{

}