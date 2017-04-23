#pragma once

#include <Utilities.h>
#define INPUT InputManager::GetInstance()
#define SCREEN_DEFAULT_W 1280
#define SCREEN_DEFAULT_H 720
//state 0: press , 1 release, 2 drag
enum KEY
{
	//For CAMERA
	A = 1,
	W = 2,
	D = 3,
	S = 4,
	E = 5,
	X = 6,
	LEFT = 7,
	UP = 8,
	RIGHT = 9,
	DOWN = 10,

	//For OBJECT
	J = 11,
	I = 12,
	L = 13,
	K = 14,
	N = 15,
	M = 16
};

class InputManager
{
public:
	static InputManager *GetInstance();
	void KeyReleased(char key);
	void KeyPressed(char key);
	bool IsPressed(char keyEnum);
	static void DestroyInstance();

	void SetMouseClick(int type, int x, int y);
	void SetSize(int x, int y);
	void SetSize(GLfloat ratio);

	int GetScreenWidth() const;
	int GetScreenHeight() const;
	float GetRatioWidth() const;
	float GetRatioHeight() const;

	void Reset();

	Vector2 GetVectorDrag();
	Vector2 GetCoordPress();
	Vector2 GetCoordRelease();
	int GetState();
private:
	InputManager();
	~InputManager();

	int screenWidth,screenHeight;
	float ratioWidth, ratioHeight;

	long keyPressed;
	int state;
	float xPressed, yPressed, xReleased, yReleased;

	static InputManager *instance;
};