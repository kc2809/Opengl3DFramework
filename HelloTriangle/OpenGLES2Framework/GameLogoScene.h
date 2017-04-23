#pragma once
#include "Scene.h"
class GameLogoScene :
	public Scene
{
private:
	Object2D *rat, *surface, *cat, *logo;
public:
	GameLogoScene();
	~GameLogoScene();

	void Init(char *sceneFile);
	void Draw();
	void Update();

};

