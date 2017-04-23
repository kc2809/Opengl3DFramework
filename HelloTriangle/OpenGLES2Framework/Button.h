#pragma once
#include "Object2D.h"
#include "SoundManager.h"
class Button :
	public Object2D
{
public:
	Button();
	~Button();

	void Init();
	void Update(float deltatime);
	void Draw(Matrix &viewMatrix);

	bool isClick();
	bool isEnter();
};

