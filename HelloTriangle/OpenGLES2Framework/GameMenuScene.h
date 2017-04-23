#pragma once
#include "Scene.h"
#include <stdlib.h>
class GameMenuScene :
	public Scene
{
public:
	GameMenuScene();
	~GameMenuScene();

	void Init(char *sceneFile);
	void Draw();
	void Update();
private:
	Button *playButton, *highScoreButton, *helpButton, *quitButton;
};

