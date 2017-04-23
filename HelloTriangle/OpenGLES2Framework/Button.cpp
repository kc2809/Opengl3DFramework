#include "Button.h"


Button::Button()
{
	
}


Button::~Button()
{

}

void Button::Init(){
	Object2D::Init();
}

bool Button::isClick(){
	static int soundflag = 1;
	if (INPUT->GetState() < 0) soundflag = 1;
	if (INPUT->GetState() >= 0){
		Vector2 coordPress = INPUT->GetCoordPress();
		if (coordPress.x >= position.x && coordPress.x <= position.x + width*scale.x
			&& coordPress.y >= position.y && coordPress.y <= position.y + height*scale.y
			&&INPUT->GetState() != 1){
			if (soundflag) {
				SOUND->PlaySound(S_CLICK, 1);
				soundflag = 0;
			}
			return true;
		}
	}
	return false;
}

bool Button::isEnter()
{
	if (INPUT->GetState() == 1){
		Vector2 coordPress = INPUT->GetCoordPress();
		Vector2 coordRelease = INPUT->GetCoordRelease();
		if (coordPress.x >= position.x && coordPress.x <= position.x + width*scale.x &&
			coordPress.y >= position.y && coordPress.y <= position.y + height*scale.y&&
			coordRelease.x >= position.x && coordRelease.x <= position.x + width*scale.x &&
			coordRelease.y >= position.y && coordRelease.y <= position.y + height*scale.y){
			printf("\nEnter");
			INPUT->Reset();
			return true;
		}
	}
	return false;
}

void Button::Update(float deltatime)
{
	if (numOfTextures > 1)
	{
		if (isClick())
		{
			usingTexture = 1;
		}
		else
		{
			usingTexture = 0;
		}
	}

}

void Button::Draw(Matrix &viewMatrix)
{
	Object2D::Draw(viewMatrix);
}