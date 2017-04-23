#include "SceneManager.h"

SceneManager *SceneManager::sceneManager = 0;

SceneManager::SceneManager()
{
	currentScene = 0;
}


SceneManager::~SceneManager()
{
	if (menuScene){
		delete menuScene;
		menuScene = 0;
	}
	if (gameScene){
		delete gameScene;
		gameScene = 0;
	}
	if (scoreScene){
		delete scoreScene;
		scoreScene = 0;
	}
	if (helpScene){
		delete helpScene;
		helpScene = 0;
	}
	if (logoScene){
		delete logoScene;
		logoScene = 0;
	}
}

SceneManager *SceneManager::GetInstance()
{
	if (sceneManager == 0){
		sceneManager = new SceneManager;
	}
	return sceneManager;
}

void SceneManager::DestroyInstance()
{

	if (sceneManager != 0)
	{
		delete sceneManager;
		sceneManager = 0;
	}
}

void SceneManager::Init(char *fileName)
{
	//Menu Scene]
	FILE *file;
	file = fopen(fileName, "r");
	if (file){
		char bufferString[128];
		fscanf(file, "#MENUSCENE\n%s\n", bufferString);

		//Load main menu scene
		menuScene = new GameMenuScene();
		menuScene->Init(bufferString);

		//Load scores scene
		scoreScene = new GameScoreScene();
		fscanf(file, "#SCOREFILE\n%s\n", bufferString);
		scoreScene->SetScoreFile(bufferString);
		fscanf(file, "#SCORESCENE\n%s\n", bufferString);
		scoreScene->Init(bufferString);

		// Load help scene
		fscanf(file, "#HELPSCENE\n%s\n", bufferString);
		helpScene = new GameHelpScene();
		helpScene->Init(bufferString);

		// Load gameplay scene
		gameScene = new GamePlayScene();
		GLuint t;
		fscanf(file, "#LEVELSFOLDER\n%s\n", bufferString);
		gameScene->SetLevelsForder(bufferString);
		fscanf(file, "#NumberOfLevels %u\n", &t);
		gameScene->SetNumOfLevel(t);
		fscanf(file, "#GAMEPLAYSCENE\n%s\n", bufferString);
		gameScene->Init(bufferString);


		// Load logo scene
		fscanf(file, "#LOGOSCENE\n%s\n", bufferString);
		logoScene = new GameLogoScene();
		logoScene->Init(bufferString);

		currentScene = logoScene;
	}
	else
	{
		printf("[ERROR] Cann't read file %s\n", file);
	}
}


void SceneManager::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	currentScene->Draw();
}


void SceneManager::Update()
{
	currentScene->Update();
}


void SceneManager::ChangeScene(SCENE_STATE sceneState)
{
	switch (sceneState)
	{
		case HOME:
			currentScene = menuScene;
			break;
		case PLAYGAME:
			currentScene = gameScene;
			gameScene->Reset();
			break;
		case HIGHSCORE:
			currentScene = scoreScene;
			break;
		case HELP:
			currentScene = helpScene;
			break;
		case QUIT:
			InputManager::DestroyInstance();
			ResourceManager::DestroyInstance();
			SceneManager::DestroyInstance();
			Text::FreeInstance();
			SoundManager::DestroyInstance();
			exit(EXIT_SUCCESS);
			break;
		default:
			break;
	}
}


int SceneManager::ChangeScore(int score)
{
	return scoreScene->AddScore(score);
}