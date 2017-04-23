#include "GameScoreScene.h"
#include "SceneManager.h"

GameScoreScene::GameScoreScene()
{
	scoreFile = 0;
	scores = 0;
	path = 0;
}


GameScoreScene::~GameScoreScene()
{
	if (scores){
		delete[] scores;
		scores = 0;
	}
	if (scoreFile != 0){
		delete[] scoreFile;
		scoreFile = 0;
	}
	if (path){
		delete[] path;
		path = 0;
	}
}


void GameScoreScene::Init(char *sceneFile){

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
			replayButton = &buttons[i];
		}
	}
	LoadHighScore(scoreFile);
}

void GameScoreScene::LoadHighScore(const char* path){
	if (this->path){
		delete[] this->path;
		this->path = 0;
	}
	this->path = new char[strlen(path) + 1];
	strcpy(this->path, path);
	FILE *file = fopen(path, "r");
	if (file == 0) {
		numOfHighScores = 5;
		scores = new int[numOfHighScores];
		for (int i = 0; i < numOfHighScores; i++){
			scores[i] = 0;
		}
		SaveHishScore(path);
	}
	else{
		fscanf(file, "%d\n", &numOfHighScores);
		scores = new int[numOfHighScores];
		for (int i = 0; i < numOfHighScores; i++){
			fscanf(file, "%d\n", &scores[i]);
			printf("%d->", scores[i]);
		}
	}
	fclose(file);
}

int GameScoreScene::AddScore(int score){
	int index = numOfHighScores -1;
	while (score > scores[index] && index>=0) index--;
	index++;
	for (int i = numOfHighScores - 1; i > index; i--) scores[i] = scores[i - 1];
	if (index <= numOfHighScores - 1) {
		scores[index] = score;
		SaveHishScore(path);
		return index;
	}
	return -1;
}

void GameScoreScene::SaveHishScore(const char* path){
	FILE *f = fopen(path, "w");
	if (f == 0) {
		printf("[ERROR] Can't save score in file %s", path);
	}
	else{
		fprintf(f, "%d\n", numOfHighScores);
		for (int i = 0; i < numOfHighScores; i++){
			fprintf(f, "%d\n", scores[i]);
		}
	}
	fclose(f);
}


void GameScoreScene::Draw(){

	Scene::Draw();
	float ratiox = INPUT->GetRatioWidth();
	float ratioy = INPUT->GetRatioHeight();
	TEXTG->SetScale(Vector2(0.5f, 0.5f));
	TEXTG->DrawText("TOP 5:", -350.0f, 150.0f, camera2D->GetViewMatrix());
	char score[20];
	for (int i = 0; i < numOfHighScores; i++){
		sprintf(score, "%d ... %d", i+1, scores[i]);
		TEXTG->DrawText(score, -200.0f, 100.0f - i*80.0f, camera2D->GetViewMatrix());
	}
	
}

void GameScoreScene::Update(){
	for (unsigned int i = 0; i < numOfButtons; i++)
	{
		buttons[i].Update(Timer::FrameTime());
	}

	//Click button play Game
	if (backButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(HOME);
	}

	if (replayButton->isEnter())
	{
		SceneManager::GetInstance()->ChangeScene(PLAYGAME);
	}

	if (INPUT->GetState() == 1) INPUT->Reset();
}


void GameScoreScene::SetScoreFile(char *file){
	if (scoreFile){
		delete[] scoreFile;
		scoreFile = 0;
	}
	scoreFile = new char[strlen(file) + 1];
	strcpy(scoreFile, file);
}

int GameScoreScene::GetNumOfHighScores(){
	return numOfHighScores;
}