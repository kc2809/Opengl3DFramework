#include "GiftBox.h"


GiftBox::GiftBox()
{
	time = 0;
	timeShow = 0;
}

GiftBox::~GiftBox()
{
}

void GiftBox::CreateRandomGiftBox(Object3D *cat, Vector3 &diffCatEgg, Object3D *cube, Object3D *items, GLuint nItmes){
	SetShader(RESOURCES->GetShaderByID("Basic"));
	SetNumOfModels(1);
	SetModel(RESOURCES->GetModelByID("giftBox"),0);
	SetScale(Vector3(0.04f, 0.04f, 0.04f));
	SetSpeed(0.5);
	// Begin trajectory from random position cat and force
	//reduction of egg throwing power for exceeding the camera's radar
	Vector2 force( -20+40*((float)rand() / RAND_MAX), -10+20*((float)rand() / RAND_MAX));
	Vector3 beginPos(cube->GetMinVertex().x + (cube->GetMaxVertex().x - cube->GetMinVertex().x)*(float)rand() / RAND_MAX,
		cat->GetPosition().y + diffCatEgg.y,
		cat->GetPosition().z + diffCatEgg.z);
	float t = 0.0f;
	while (t < 6.0f){
		position = Vector3(beginPos.x + force.x*t,
			beginPos.y + force.y*t - 0.5f*g*t*t,
			beginPos.z + -20.0f*t);
		SetBox();
		if (!IsInObject(cube)){
			goto setGiftBox;
		}
		for (GLuint i = 0; i < nItmes; i++){
			if (IsCollision(items + i)){
				goto setGiftBox;
			}
		}
		t += 0.04f;
	}
	setGiftBox:

	timeShow = 5.0f + 3.0f*((float)rand() / RAND_MAX);
}

bool GiftBox::Show(){
	time += Timer::FrameTime();
	if (timeShow > time){
		return false;
	}
	return true;
}