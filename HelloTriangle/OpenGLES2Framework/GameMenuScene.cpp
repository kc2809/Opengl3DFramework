#include "GameMenuScene.h"
#include "SceneManager.h"

GameMenuScene::GameMenuScene()
{
}


GameMenuScene::~GameMenuScene()
{
}

void GameMenuScene::Init(char *sceneFile){
	Scene::Init(sceneFile);

	if (camera3D)
	{
		delete camera3D;
		camera3D = 0;
	}

	for (GLuint i = 0; i < numOfButtons; i++)
	{
		if (strcmp(buttons[i].GetName(), "gameplayButton") == 0)
		{
			playButton = &buttons[i];
		}
		if (strcmp(buttons[i].GetName(), "highScoreButton") == 0)
		{
			highScoreButton = &buttons[i];
		}
		if (strcmp(buttons[i].GetName(), "helpButton") == 0)
		{
			helpButton = &buttons[i];
		}
		if (strcmp(buttons[i].GetName(), "quitButton") == 0)
		{
			quitButton = &buttons[i];
		}
	}
}

void GameMenuScene::Draw(){
	Scene::Draw();
}

void GameMenuScene::Update(){

	for (unsigned int i = 0; i < numOfButtons; i++)
	{
		buttons[i].Update(Timer::FrameTime());
	}

	//Click button play Game
	if (playButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(PLAYGAME);
	}

	//Click button high Score
	if (highScoreButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(HIGHSCORE);
	}

	//Click button help
	if (helpButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(HELP);
	}

	//Click button quit
	if (quitButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(QUIT);
	}

	if (INPUT->GetState() == 1) INPUT->Reset();

}