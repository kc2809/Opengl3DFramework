#pragma once
#include "Scene.h"
class GameScoreScene :
	public Scene
{
public:
	GameScoreScene();
	~GameScoreScene();
	void Init(char *sceneFile);
	void Draw();
	void Update();
	int AddScore(int score);
	void SaveHishScore(const char*);

	void SetScoreFile(char *scoreFile);
	GLint GetNumOfHighScores();
private:
	char *scoreFile;
	char *path;
	int numOfHighScores;
	int* scores;
	Button *backButton, *replayButton;

	void LoadHighScore(const char* );
};

