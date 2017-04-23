#include <stdlib.h>
#include "InputManager.h"

InputManager *InputManager::instance = 0;

InputManager::InputManager()
{
	screenWidth = 1280;
	screenHeight = 720;
	keyPressed = 0;
	state = -1;
}

InputManager *InputManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new InputManager;
	}
	return instance;
}

void InputManager::KeyPressed(char key){
	switch (key)
	{
	case 'A':
	{
				keyPressed |= (1 << A);
	}
		break;

	case 'W':
	{
				keyPressed |= (1 << W);
	}
		break;

	case 'D':
	{
				keyPressed |= (1 << D);
	}
		break;

	case 'S':
	{
				keyPressed |= (1 << S);
	}
		break;

	case 'E':
	{
				keyPressed |= (1 << E);
	}
		break;

	case 'X':
	{
				keyPressed |= (1 << X);
	}
		break;

	case 37:
	{
			   keyPressed |= (1 << LEFT);
	}
		break;

	case 38:
	{
			   keyPressed |= (1 << UP);
	}
		break;

	case 39:
	{
			   keyPressed |= (1 << RIGHT);
	}
		break;

	case 40:
	{
			   keyPressed |= (1 << DOWN);
	}
		break;

	case 'J':
	{
				keyPressed |= (1 << J);
	}
		break;

	case 'I':
	{
				keyPressed |= (1 << I);
	}
		break;

	case 'L':
	{
				keyPressed |= (1 << L);
	}
		break;

	case 'K':
	{
				keyPressed |= (1 << K);
	}
		break;

	case 'N':
	{
				keyPressed |= (1 << N);
	}
		break;

	case 'M':
	{
				keyPressed |= (1 << M);
	}
		break;
	default:
		break;
	}
}

void InputManager::KeyReleased(char key)
{
	switch (key)
	{
	case 'A':
	{
				keyPressed &= ~(1 << A);
	}
		break;

	case 'W':
	{
				keyPressed &= ~(1 << W);
	}
		break;

	case 'D':
	{
				keyPressed &= ~(1 << D);
	}
		break;

	case 'S':
	{
				keyPressed &= ~(1 << S);
	}
		break;

	case 'E':
	{
				keyPressed &= ~(1 << E);
	}
		break;

	case 'X':
	{
				keyPressed &= ~(1 << X);
	}
		break;

	case 37:
	{
			   keyPressed &= ~(1 << LEFT);
	}
		break;

	case 38:
	{
			   keyPressed &= ~(1 << UP);
	}
		break;

	case 39:
	{
			   keyPressed &= ~(1 << RIGHT);
	}
		break;

	case 40:
	{
			   keyPressed &= ~(1 << DOWN);
	}
		break;

	case 'J':
	{
				keyPressed &= ~(1 << J);
	}
		break;

	case 'I':
	{
				keyPressed &= ~(1 << I);
	}
		break;

	case 'L':
	{
				keyPressed &= ~(1 << L);
	}
		break;

	case 'K':
	{
				keyPressed &= ~(1 << K);
	}
		break;

	case 'N':
	{
				keyPressed &= ~(1 << N);
	}
		break;

	case 'M':
	{
				keyPressed &= ~(1 << M);
	}
		break;
	default:
		break;
	}
}


bool InputManager::IsPressed(char key)
{
	return ((keyPressed &(1 << key)) != 0);
}

void InputManager::DestroyInstance()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

InputManager::~InputManager()
{
}

void InputManager::SetMouseClick(int type, int x, int y){
	this->state = type;
	if (type == 0){
		xPressed = x - (float)screenWidth/2;
		yPressed = -y + (float)screenHeight / 2;
	}
	else if(type>0){
		xReleased = x - (float)screenWidth / 2;
		yReleased = -y + (float)screenHeight / 2;
	}
}

void InputManager::SetSize(int x, int y){
	screenWidth = x;
	screenHeight = y;
}


void InputManager::SetSize(GLfloat ratio){
	screenWidth = SCREEN_DEFAULT_W;
	screenHeight = (ratio!= 0.0)? SCREEN_DEFAULT_W/ratio: SCREEN_DEFAULT_H;
}

int InputManager::GetScreenWidth() const{
	return screenWidth;
}
int InputManager::GetScreenHeight() const{
	return screenHeight;
}
float InputManager::GetRatioWidth() const{
	return (GLfloat) screenWidth / SCREEN_DEFAULT_W;
}
float InputManager::GetRatioHeight() const{
	return (GLfloat) screenHeight/SCREEN_DEFAULT_H;
}


int InputManager::GetState(){
	return state;
}
void InputManager::Reset(){
	xPressed = -(float)screenWidth; xReleased = (float)screenWidth;
	yPressed = -(float)screenHeight; yReleased = (float)screenHeight;
	state = -1;
}


Vector2 InputManager::GetVectorDrag()
{
	if (xPressed > screenWidth / 2 || xPressed<-screenWidth / 2 ||
		xReleased>screenWidth / 2 || xReleased<-screenWidth / 2 ||
		yPressed>screenHeight / 2 || yPressed<-screenHeight / 2 ||
		yReleased>screenHeight / 2 || yReleased<-screenHeight / 2 ||
		(abs(xReleased - xPressed) < 15) && (abs(yReleased - yPressed) < 15))
	{
		return Vector2(0.0, 0.0);
	}
	return Vector2(xReleased - xPressed, yReleased - yPressed);
}


Vector2 InputManager::GetCoordPress(){
	return Vector2(xPressed, yPressed);
}


Vector2 InputManager::GetCoordRelease()
{
	return Vector2(xReleased, yReleased);
}
