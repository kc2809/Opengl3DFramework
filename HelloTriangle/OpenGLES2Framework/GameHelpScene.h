#pragma once
#include "Scene.h"
class GameHelpScene :
	public Scene
{
public:
	GameHelpScene();
	~GameHelpScene();
	void Init(char *sceneFile);
	void Draw();
	void Update();
private:
	Button *backButton, *startButton;
};

