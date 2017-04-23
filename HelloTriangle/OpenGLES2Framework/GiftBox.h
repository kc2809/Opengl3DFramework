#pragma once
#include "Object3D.h"
#include "Egg.h"
#include <stdlib.h>  

class GiftBox :
	public Object3D
{
public:
	GiftBox();
	~GiftBox();
	void CreateRandomGiftBox(Object3D *cat, Vector3 &diffCatEgg, Object3D *cube, Object3D *items, GLuint nItmes);
	bool Show();
private:

	float time,timeShow;
};

