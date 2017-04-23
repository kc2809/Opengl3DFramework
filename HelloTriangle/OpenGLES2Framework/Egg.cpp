#include "Egg.h"


Egg::Egg()
{
	time = 0.0f;
}

Egg::Egg(Vector3 pos) : Egg()
{
	position = pos;
}

Egg::~Egg()
{
}

void Egg::SetRotateEgg(Vector3 &postion, Vector4 diff,Vector3 &rotate){
	Matrix rotateX, rotateY, rotateZ;
	rotateX.SetRotationX(rotate.x);
	rotateY.SetRotationY(rotate.y);
	rotateZ.SetRotationY(rotate.z);

	SetPosition(Vector3((diff*rotateX*rotateY*rotateZ).xyz) + postion);
}

void Egg::SetForceEgg(Vector2 force){
	force.x /= INPUT->GetScreenWidth();
	force.y /= INPUT->GetScreenHeight();
	forceEgg = force;
}
/*
 x = Xo + Vx * t
 y = Yo + Vy *t - 1/2 * g * t * t
 z = Zo + Vz *t 
 */
void Egg::Throw(GLfloat Time){
	position = Vector3(beginPos.x + forceThrowEgg.x * forceEgg.x * time,
		beginPos.y + forceThrowEgg.y * forceEgg.y*time - 0.5f*g*time*time,
		beginPos.z + forceThrowEgg.z * time);
	speed = forceEgg.y;
	RotateX(Time);
	time += Time;
	SetBox();
}

void Egg::SetThrow(){
	beginPos = position;
	this->time = 0;
}

void Egg::DrawTrajectory(Vector2 force, Matrix &viewMatrix, Object3D *cube, Object3D *items, GLuint nitems){
	std::vector<Vector3> Eggtrajectorys;
	time += 2.0f*Timer::FrameTime();

	force.x /= INPUT->GetScreenWidth();
	force.y /= INPUT->GetScreenHeight();

	if (time >= 1.0f){
		time -= 1.0f;
	}
	float k = 0.0f;

	Object3D trajectory;
	trajectory.SetNumOfModels(1);
	trajectory.SetModel(RESOURCES->GetModelByID("fireball"), 0);
	trajectory.SetScale(Vector3(0.01f, 0.01f, 0.01f));
	trajectory.SetShader(RESOURCES->GetShaderByID("Basic"));

	while (k <= 2.0f)
	{
		float t;
		for (float j = 0; j < 0.2; j += 0.05f){
			// Draw trajectory
			t = k + j;
			Vector3 pos(position.x + forceThrowEgg.x*force.x*t,
				position.y + forceThrowEgg.y*force.y*t - 0.5f*g*t*t,
				position.z + forceThrowEgg.z*t);
			trajectory.SetPosition(pos);
			trajectory.SetBox();
			// Colision
			if (!trajectory.IsInObject(cube)){
				goto draw;
			}
			for (GLuint i = 0; i < nitems; i++){
				if (trajectory.IsCollision(items + i)){
					goto draw;
				}
			}
		}
		// Add position
		t = k + 0.2f*time;
		Vector3 pos(position.x + forceThrowEgg.x*force.x*t,
			position.y + forceThrowEgg.y*force.y*t - 0.5f*g*t*t,
			position.z + forceThrowEgg.z*t);
		Eggtrajectorys.push_back(pos);

		k += 0.2f;
	}
	draw:
	// Draw
	trajectory.SetRotate(Vector3(time * 2 * 3.14f, 0.0f, 0.0f));
	while(!Eggtrajectorys.empty()){
		trajectory.SetPosition(Eggtrajectorys.back());
		trajectory.Draw(viewMatrix);
		Eggtrajectorys.pop_back();
	}
}
