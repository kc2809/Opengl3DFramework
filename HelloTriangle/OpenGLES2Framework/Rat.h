#pragma once
#include "Object3D.h"
#include "Egg.h"
#include <stdlib.h>
#include <time.h>   

// linked list
class Rat :
	public Object3D
{
private:
	Rat *next;
	GLubyte score;
	Vector3 move, minPosition, maxPosition;

	float keyframe;
public:
	Rat();
	~Rat();

	
	void static AddRat(Rat *&list, Rat *rat);
	Rat static *AddRat(Rat *&list, GLbyte type, Vector3 position, Vector3 scale);
	void static RemoveRat(Rat *&list, Rat *rat);

	Rat *IsCollision(Egg *egg);

	void Update(GLfloat deltaTime);
	void UpdateAll(GLfloat deltaTime);
	void DrawAll(Matrix world);

	void static ClearRat(Rat *&list);

	void setMove(Vector3);
	void setMaxPosition(Vector3);
	void setMinPosition(Vector3);
	void SetScore(GLubyte score);
	GLubyte GetScore();
};

