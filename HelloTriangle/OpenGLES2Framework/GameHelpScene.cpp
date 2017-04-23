#include "GameHelpScene.h"
#include "SceneManager.h"

GameHelpScene::GameHelpScene()
{
}


GameHelpScene::~GameHelpScene()
{
}

void GameHelpScene::Init(char *sceneFile){

	Scene::Init(sceneFile);
	if (camera3D)
	{
		delete camera3D;
		camera3D = 0;
	}

	for (GLuint i = 0; i < numOfButtons; i++)
	{
		if (strcmp(buttons[i].GetName(), "BACK") == 0)
		{
			backButton = &buttons[i];
		}
		else
		{
			startButton = &buttons[i];
		}
	}
}

void GameHelpScene::Draw(){
	if (camera3D){
		delete camera3D;
		camera3D = 0;
	}
	char* help[] = {
		"Throw    to kill all",
		"in the room.",
		"",
		"    to move left and",
		"       to move right",
		"",
		" warning:    you be timed",
		"",
		"Try to get the best score",
		"    Good luck !",
	};
	float ratiox = INPUT->GetRatioWidth();
	float ratioy = INPUT->GetRatioHeight();
	Scene::Draw();
	TEXTG->SetScale(Vector2(0.3f, 0.3f));
	for (int i = 0; i < 10; i++){
		TEXTG->DrawText(help[i], -380.0f, 150.0f-40.0f*i, camera2D->GetViewMatrix());
	}
	TEXTG->SetScale(Vector2(0.5, 0.5));
	TEXTG->DrawText("Let's      !", -300, -310, camera2D->GetViewMatrix());
	
}

void GameHelpScene::Update(){
	for (unsigned int i = 0; i < numOfButtons; i++)
	{
		buttons[i].Update(Timer::FrameTime());
	}
	//Click button play Game
	if (backButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(HOME);
	}
	if (startButton->isEnter())
	{
		SceneManager::GetInstance()->ChangeScene(PLAYGAME);
	}
	
	if (INPUT->GetState() == 1) INPUT->Reset();
}
