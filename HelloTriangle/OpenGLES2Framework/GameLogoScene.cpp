#include "GameLogoScene.h"
#include "SceneManager.h"

GameLogoScene::GameLogoScene()
{
}


GameLogoScene::~GameLogoScene()
{
}

void GameLogoScene::Init(char *sceneFile){
	Scene::Init(sceneFile);
	if (camera3D)
	{
		delete camera3D;
		camera3D = 0;
	}
	for (GLuint i = 0; i < numOfObject2Ds; i++)
	{
		if (strcmp(object2Ds[i].GetName(), "logo") == 0)
		{
			logo = &object2Ds[i];

		}
		if (strcmp(object2Ds[i].GetName(), "Cat") == 0)
		{
			cat = &object2Ds[i];
			
		}
		if (strcmp(object2Ds[i].GetName(), "Rat") == 0)
		{
			rat = &object2Ds[i];
		}
		if (strcmp(object2Ds[i].GetName(), "surface") == 0)
		{
			surface = &object2Ds[i];
			
		}
	}
	rat->SetSpeed(600*INPUT->GetRatioWidth());
	surface->SetSpeed(600*INPUT->GetRatioWidth());
}

void GameLogoScene::Draw(){
	Scene::Draw();
}

void GameLogoScene::Update(){
	rat->MoveLeftRight(Timer::FrameTime());

	if (surface->GetPosition().x  >= logo->GetPosition().x)
	{
		surface->SetPosition(Vector3(logo->GetPosition().x / INPUT->GetRatioWidth(), logo->GetPosition().y / INPUT->GetRatioHeight(), 0.1));
		cat->SetUsingTexture(1);
	}
	else
	{
		surface->MoveLeftRight(Timer::FrameTime());
	}
	


	if (rat->GetPosition().x > INPUT->GetScreenWidth()/2){
		SceneManager::GetInstance()->ChangeScene(HOME);
	}
}