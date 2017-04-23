#pragma once
#include "GameScoreScene.h"
#include "Scene.h"
#include "Rat.h"
#include "GiftBox.h"

enum GamePlayState{ LOOK , THROW, NEXT, END };

class GamePlayScene : public Scene
{
private:
	float timeGame, timeShowGiftBox;
	GiftBox *giftBox;
	// Levels
	char *levelsFolder;
	GLuint numOfLevel, currentLevel, numberOfEgg, numOfRats;
	GameScoreScene *gameScoreScene;
	// Game
	GamePlayState flag;

	int score;

	Egg *egg;
	Rat *rats;
	Object3D *cube,*outside, *cat,*items;
	Object2D *informFrame;
	Button *homeButton, *soundButton, *leftButton, *rightButton;

	char result[2][15];
	GLuint numOfItems;

	Vector3 diffEgg;
	void LoadLevel(char *file);
	void CreateEgg();
	void GameOver();

	char textReceive[6];
	float timeReceive;
	Object2D eggReceive; bool isEggReceive;
public:
	GamePlayScene();
	~GamePlayScene();

	void Init(char *sceneFile);
	void Draw();
	void Update();
	void Reset();

	void SetNumOfLevel(GLuint numOfLevel);
	void SetLevelsForder(char *levelsFolder);
	int NextLevel();
};

