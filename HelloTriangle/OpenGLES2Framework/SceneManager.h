#pragma once
#include "GamePlayScene.h"
#include "GameMenuScene.h"
#include "GameHelpScene.h"
#include "GameScoreScene.h"
#include "GameLogoScene.h"

class SceneManager
{
private:
	static SceneManager *sceneManager;
	
	Scene *currentScene;
	GameMenuScene *menuScene;
	GamePlayScene *gameScene;
	GameHelpScene *helpScene;
	GameScoreScene *scoreScene;
	GameLogoScene *logoScene;
	SceneManager();
	~SceneManager();
public:
	static SceneManager *GetInstance();
	static void DestroyInstance();

	void Init(char *file);
	void Draw();
	void Update();

	void ChangeScene(SCENE_STATE);
	int ChangeScore(int score);
};

