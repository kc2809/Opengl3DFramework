#include <math.h>
#include "GamePlayScene.h"
#include "SceneManager.h"

GamePlayScene::GamePlayScene()
{
	levelsFolder = 0;
	egg = 0;
	numOfLevel = 0;
	rats = 0;
	items = 0;
	cube = 0;
	outside = 0;
	currentLevel = 0;
	giftBox = 0;
	timeShowGiftBox = -1.0f;
	timeReceive = -1.0f; isEggReceive = false;
	informFrame = 0;
}

GamePlayScene::~GamePlayScene()
{
	if (levelsFolder){
		delete[] levelsFolder;
	}
	if (egg){
		delete egg;
	}
	if (cube){
		delete cube;
	}
	if (outside)
	{
		delete outside;
		outside = 0;
	}
	if (items){
		delete[] items;
	}
	Rat::ClearRat(rats);
	if (giftBox){
		delete giftBox;
	}
}

void GamePlayScene::Init(char *sceneFile){

	Scene::Init(sceneFile);
	
	for (GLuint i = 0; i < numOfObject3Ds; i++)
	{
		if (strcmp(object3Ds[i].GetName(), "Cat") == 0)
		{
			cat = &object3Ds[i];
		}
	}
	cat->SetSpeed(camera3D->GetSpeed());
	diffEgg = Vector3(0.47f, -8.0f, 14.56f) - cat->GetPosition();
	for (GLuint i = 0; i < numOfButtons; i++)
	{
		if (strcmp(buttons[i].GetName(), "home") == 0)
		{
			homeButton = &buttons[i];
		}
		if (strcmp(buttons[i].GetName(), "sound") == 0)
		{
			soundButton = &buttons[i];
		}
		if (strcmp(buttons[i].GetName(), "left") == 0)
		{
			leftButton = &buttons[i];
		}
		if (strcmp(buttons[i].GetName(), "right") == 0)
		{
			rightButton = &buttons[i];
		}
	}

	for (GLuint i = 0; i < numOfObject2Ds; i++)
	{
		if (strcmp(object2Ds[i].GetName(), "informFrame") == 0)
		{
			informFrame = &object2Ds[i];
		}
	}
	
	Texture **tempTexture = new Texture*[1]; tempTexture[0] = RESOURCES->GetTextureByID("eggicon");
	eggReceive.SetShader(RESOURCES->GetShaderByID("Basic"));
	eggReceive.SetTexture(tempTexture, 1);
	eggReceive.Init();

	Reset();
}

void GamePlayScene::Reset(){
	INPUT->Reset();
	score = 0;
	currentLevel = 0;
	NextLevel();
	strcpy(result[0], "you");
	strcpy(result[1], "lost");
	if (!soundButton->GetUsingTexture()) SOUND->PlayMusic(S_BACKGROUND, 1);
	flag = LOOK;
}

void GamePlayScene::Draw(){
	
	char string[30];
	float ratiox = INPUT->GetRatioWidth();
	float ratioy = INPUT->GetRatioHeight();
	
	if (outside){
		outside->Draw(camera3D->GetViewMatrix());
	}

	if (cube){
		cube->Draw(camera3D->GetViewMatrix());
	}

	for (GLuint i = 0; i < numOfItems; i++){
		items[i].Draw(camera3D->GetViewMatrix());
	}
	if (giftBox){
		giftBox->Draw(camera3D->GetViewMatrix());
	}
	if (flag == END){
		TEXTG->SetScale(Vector2(0.7f, 0.7f));
		sprintf(string, "%d", score);;
		informFrame->Draw(camera2D->GetViewMatrix());
		for (GLuint i = 0; i < 2; i++) 
		{
			TEXTG->DrawText(result[i], -102.4*strlen(result[i]) / 2 * 0.7, 40 - 102.4*0.7*i, camera2D->GetViewMatrix());
		}
		TEXTG->SetScale(Vector2(0.9f, 0.9f));
		TEXTG->DrawText(string, -102.4*strlen(string) / 2 * 0.9, -130.0f, camera2D->GetViewMatrix());
	}
	else if (flag == NEXT)
	{
		static GLuint c = 0;
		informFrame->Draw(camera2D->GetViewMatrix());
		TEXTG->SetScale(Vector2(0.7, 0.7));
		TEXTG->DrawText("next", -143.4, 5, camera2D->GetViewMatrix());
		TEXTG->DrawText("level", -179, -70, camera2D->GetViewMatrix());
		c++;
		if (c == 100)
		{
			INPUT->Reset();
			flag = LOOK;
			c = 0;
		}
	}
	else
	{
		if (egg){
			egg->Draw(camera3D->GetViewMatrix());
		}
		rats->DrawAll(camera3D->GetViewMatrix());
	}


	for (unsigned int i = 0; i < numOfObject3Ds; i++)
	{
		object3Ds[i].Draw(camera3D->GetViewMatrix());
	}
	for (unsigned int i = 0; i < numOfObject2Ds; i++)
	{
		if (strcmp(object2Ds[i].GetName(), "informFrame") != 0)
		{
			object2Ds[i].Draw(camera2D->GetViewMatrix());
		}
	}
	for (unsigned int i = 0; i < numOfButtons; i++)
	{
		buttons[i].Draw(camera2D->GetViewMatrix());
	}

	if (timeReceive > 0){ // Show text receive score or egg
		if (isEggReceive){
			TEXTG->SetScale(Vector2(1.0f - timeReceive, 1.0f - timeReceive));
			TEXTG->DrawTextCenter(textReceive, 0.0f, 100.0f, camera2D->GetViewMatrix());
			eggReceive.SetPosition(Vector3(TEXTG->GetWidth()*strlen(textReceive) , 100.0f - TEXTG->GetHeight()/2, 0.0f));
			eggReceive.SetScale(Vector3(1.0f - timeReceive, 1.0f - timeReceive, 1.0f));
			eggReceive.Draw(camera2D->GetViewMatrix());
		}
		else{
			TEXTG->SetScale(Vector2(1.0f - timeReceive, 1.0f - timeReceive));
			TEXTG->DrawTextCenter(textReceive, 0.0f, 100.0f, camera2D->GetViewMatrix());
		}
	}
	// Draw text
	TEXTG->SetScale(Vector2(0.5f, 0.5f));
	sprintf(string, "Score:%d", score);
	TEXTG->DrawText(string, -100.0f, 300.0f, camera2D->GetViewMatrix());
	sprintf(string, "Level:%d", currentLevel);
	TEXTG->DrawText(string, -600.0f, 300.0f, camera2D->GetViewMatrix());

	TEXTG->SetScale(Vector2(0.35f, 0.35f));
	sprintf(string, "%02d:%02d", (int)timeGame / 60, (int)timeGame % 60);
	TEXTG->DrawText(string, 450, 190, camera2D->GetViewMatrix());
	sprintf(string, "%d", numberOfEgg);
	TEXTG->DrawText(string, 450, 110, camera2D->GetViewMatrix());
	sprintf(string, "%d", numOfRats);
	TEXTG->DrawText(string, 450, 20, camera2D->GetViewMatrix());


	Vector2 throwForce = INPUT->GetVectorDrag();
	if (flag == LOOK && INPUT->GetState() == 2 && throwForce.x != 0 && throwForce.y != 0 && egg)
	{
		cat->SetRotate(Vector3(0.0f, -atan(throwForce.x / abs(throwForce.y))*0.5f, 0.0f));
		Vector3 posCat = cat->GetPosition(), rotateCat = cat->GetRotate();
		egg->SetRotateEgg(posCat, diffEgg, rotateCat);
		egg->DrawTrajectory(throwForce, camera3D->GetViewMatrix(), cube, items, numOfItems);
	}
}

void GamePlayScene::Update(){
	static bool collision = false;
	camera3D->Update(Timer::FrameTime());

	if ( timeReceive < 0){
		timeReceive = -1;
		isEggReceive = false;
	}
	else{
		timeReceive -= Timer::FrameTime();
	}
	// Test Butoon
	for (GLuint i = 0; i < numOfButtons; i++)
	{
		if (strcmp(buttons[i].GetName(), "sound") != 0)
		{
			buttons[i].Update(Timer::FrameTime());
		}
	}

	if (soundButton->isEnter()){
		soundButton->GetUsingTexture() = 1 - soundButton->GetUsingTexture();
		if (!soundButton->GetUsingTexture()) SOUND->PlayMusic(S_BACKGROUND, 1);
		if (soundButton->GetUsingTexture()){
			SOUND->StopMusic();
		}
		INPUT->Reset();
	}

	if (homeButton->isEnter()){
		SceneManager::GetInstance()->ChangeScene(HOME);
		SOUND->StopMusic();
		INPUT->Reset();
	}

	if (flag != END){
		if (rats != 0){
			timeGame -= Timer::FrameTime();
		}
		// Button move camera
		if (leftButton->isClick())
		{
			cat->MoveLeftRight(-Timer::FrameTime());
			cat->SetBox();
			if (cat->IsInObject(cube)){
				camera3D->MoveLeftRight(-Timer::FrameTime());
				egg->MoveLeftRight(-Timer::FrameTime());
			}
			else{
				cat->MoveLeftRight(Timer::FrameTime());
			}
		}

		if (rightButton->isClick())
		{
			cat->MoveLeftRight(Timer::FrameTime());
			cat->SetBox();
			if (cat->IsInObject(cube)){
				camera3D->MoveLeftRight(Timer::FrameTime());
				egg->MoveLeftRight(Timer::FrameTime());
			}
			else{
				cat->MoveLeftRight(-Timer::FrameTime());
			}
		}
		// Gift box
		if (timeShowGiftBox > 0 && timeGame < timeShowGiftBox){
			if (giftBox){
				delete giftBox;
			}
			giftBox = new GiftBox;
			giftBox->CreateRandomGiftBox(cat, diffEgg, cube, items, numOfItems);
			timeShowGiftBox = -1;
		}
		if (giftBox){
			giftBox->RotateY(Timer::FrameTime());
			if (giftBox->Show()){
				delete giftBox;
				giftBox = 0;
			}
			else if (egg->IsCollision(giftBox)){
				int EggReceive = 3 + rand() % 5; // Collect 3 to 8 egg
				numberOfEgg += EggReceive;
				sprintf(textReceive, "%d", EggReceive); isEggReceive = true;
				timeReceive = 0.5f;
				delete giftBox;
				giftBox = 0;
				collision = true;
			}
		}
		// Update rats
		rats->UpdateAll(Timer::FrameTime());
		for (GLuint i = 0; i < numOfButtons; i++){
			if (buttons[i].isClick() != 0 && egg->GetPosition().z == 14.56f){
				goto lab;
			}
		}
		// state throw
		Vector2 throwForce = INPUT->GetVectorDrag();
		if (flag == LOOK && INPUT->GetState() == 1 && throwForce.x != 0 && throwForce.y != 0 && egg){
			egg->SetThrow();
			egg->SetForceEgg(INPUT->GetVectorDrag());

			numberOfEgg--;
			INPUT->Reset();
			cat->SetUsingModel(1);
			flag = THROW;
			if (!soundButton->GetUsingTexture()) SOUND->PlaySound(S_THROWEGG, 1);
			collision = false;
		}
		// Update time
		if (flag == THROW)
		{
			Rat *collisionRat = 0;
			// Test collision
			if (!collision)
			{
				for (GLuint i = 0; i < numOfItems; i++){
					if (egg->IsCollision(items + i)){
						score--;
						sprintf(textReceive, "-1");
						timeReceive = 0.5f;
						collision = true;
						break;
					}
				}

				//collision rat with eggs without collided with item
				collisionRat = rats->IsCollision(egg);
				if (collisionRat && !collision){
					score += collisionRat->GetScore();
					sprintf(textReceive, "%d", collisionRat->GetScore());
					timeReceive = 0.5f;
					if (!soundButton->GetUsingTexture()) SOUND->PlaySound(S_COLLISION, 1);
					Rat::RemoveRat(rats, collisionRat);
					collision = true;
					numOfRats--;
				}
			}
			if (egg->IsInObject(cube) && !collision)
			{
				// Move egg
				egg->Throw(Timer::FrameTime());
				if (INPUT->GetState() == 1){
					INPUT->Reset();
				}
			}
			else{
				if(!collisionRat) SOUND->PlaySound(S_EGGCRUSH, 1);
				// Destroy and create egg
				if (numberOfEgg > 0){
					CreateEgg();
				}
				else{
					delete egg;
					egg = 0;
				}

				cat->SetUsingModel(0);
				flag = LOOK;
			}
		}
		// Die game
		if (rats == 0 && timeReceive == -1) // if out of rats and end draw score receive
		{
			score += numberOfEgg;
			if (!NextLevel()){
				numberOfEgg = 0;
				GameOver();
			}
		}
		else if ( (numberOfEgg == 0 && egg == 0  && timeReceive == -1) || timeGame <= 0.0f){
			GameOver();
		}
	}
	else{
		if (INPUT->GetState() ==1){
			SceneManager::GetInstance()->ChangeScene(HIGHSCORE);
			INPUT->Reset();
		lab:
			flag = THROW;
			collision = 1;
		}
	}
	
}

void GamePlayScene::GameOver(){
	flag = END;
	int index = SceneManager::GetInstance()->ChangeScore(score);
	if (index == 0) {
		strcpy(result[0], "highest");
		strcpy(result[1], "score");
	}
	else if (index > 0){
		strcpy(result[0], "high");
		strcpy(result[1], "score");
	}
	else {
		strcpy(result[0], "you");
		strcpy(result[1], "lost");
	}
}

void GamePlayScene::CreateEgg(){

	if (egg){
		delete egg;
	}
	egg = new Egg();
	Vector3 posCat = cat->GetPosition(), rotateCat = cat->GetRotate();
	egg->SetRotateEgg( posCat, diffEgg, rotateCat);
	egg->SetScale(Vector3(0.2f, 0.2f, 0.2f));
	egg->SetNumOfModels(1);
	egg->SetModel(RESOURCES->GetModelByID("Egg"), 0);
	egg->SetShader(RESOURCES->GetShaderByID("Basic"));
	egg->SetSpeed(camera3D->GetSpeed());
}

void GamePlayScene::LoadLevel(char *file){

	char buffterString[128];
	FILE *f;
	f = fopen(file, "r");
	if(f == 0)
	{
		printf("Can not read the file %s\n", file);
		return;
	}

	int ratScore;
	Vector3 pos, rotation, scale, move, minpos, maxpos;

	if (outside)
	{
		delete outside;
		outside = 0;
	}

	outside = new Object3D;

	GLuint t;
	fscanf(f, "#OUTSIDE\nMODEL %s\n", &buffterString);
	outside->SetNumOfModels(1);
	outside->SetModel(RESOURCES->GetModelByID(buffterString), 0);
	// Load 2d texture
	fscanf(f, "2DTEXTURE %u\n", &t);
	outside->SetNumOfTextures(t);
	for (GLuint j = 0; j < t; j++)
	{
		fscanf(f, "TEXTURE %s\n", buffterString);
		outside->SetTexture(RESOURCES->GetTextureByID(buffterString), j);
	}
	// Load cube texture
	fscanf(f, "CUBETEXTURES %u\n", &t);
	outside->SetNumOfCubeTextures(t);
	for (GLuint j = 0; j < t; j++)
	{
		fscanf(f, "CUBETEXTURE %s\n", buffterString);
		outside->SetCubeTexture(RESOURCES->GetCubeTextureByID(buffterString), j);
	}

	// Load shader
	fscanf(f, "SHADER %s\n", buffterString);
	outside->SetShader(RESOURCES->GetShaderByID(buffterString));

	// Other
	fscanf(f, "POSITION %f,%f,%f\nROTATION %f,%f,%f\nSCALE %f,%f,%f\n", &pos.x, &pos.y, &pos.z,
		&rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z);
	outside->SetPosition(pos);
	outside->SetRotate(rotation);
	outside->SetScale(scale);

	outside->SetBox();
	

	if (cube)
	{
		delete cube;
	}
	cube = new Object3D;
	fscanf(f, "\n#ROOM\nMODEL %s\n", buffterString);
	cube->SetNumOfModels(1);
	cube->SetModel(RESOURCES->GetModelByID(buffterString) , 0);
	fscanf(f, "CUBETEXTURE %s\n", buffterString);
	cube->SetNumOfCubeTextures(1);
	cube->SetCubeTexture(RESOURCES->GetCubeTextureByID(buffterString), 0);
	fscanf(f, "SHADER %s\n", buffterString);
	cube->SetShader(RESOURCES->GetShaderByID(buffterString));
	fscanf(f, "POSITION %f,%f,%f\nROTATION %f,%f,%f\nSCALE %f,%f,%f\n", &pos.x, &pos.y, &pos.z, 
		&rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z);
	cube->SetPosition(pos);
	cube->SetRotate(rotation);
	cube->SetScale(scale);
	cube->SetBox();

	fscanf(f, "#EGG %d\n", &numberOfEgg);
	fscanf(f, "\n#TIME %f\n", &timeGame);
	if (timeGame < 10.0f){
		timeGame = 10.0f;
	}

	fscanf(f, "\n#ITEMS %d\n", &numOfItems);
	if (numOfItems > 0)
	{
		if (items)
		{
			delete[] items;
		}
		items = new Object3D[numOfItems];
	}
	for (GLuint i = 0; i < numOfItems; i++){
		GLuint t;
		fscanf(f, "%*d.\nMODEL %s\n", &buffterString);
		items[i].SetNumOfModels(1);
		items[i].SetModel(RESOURCES->GetModelByID(buffterString), 0);
		// Load 2d texture
		fscanf(f, "2DTEXTURE %u\n", &t);
		items[i].SetNumOfTextures(t);
		for (GLuint j = 0; j < t; j++)
		{
			fscanf(f, "TEXTURE %s\n", buffterString);
			items[i].SetTexture(RESOURCES->GetTextureByID(buffterString), j);
		}
		// Load cube texture
		fscanf(f, "CUBETEXTURES %u\n", &t);
		items[i].SetNumOfCubeTextures(t);
		for (GLuint j = 0; j < t; j++)
		{
			fscanf(f, "CUBETEXTURE %s\n", buffterString);
			items[i].SetCubeTexture(RESOURCES->GetCubeTextureByID(buffterString), j);
		}

		// Load shader
		fscanf(f, "SHADER %s\n", buffterString);
		items[i].SetShader(RESOURCES->GetShaderByID(buffterString));

		// Other
		fscanf(f, "POSITION %f,%f,%f\nROTATION %f,%f,%f\nSCALE %f,%f,%f\n", &pos.x, &pos.y, &pos.z,
			&rotation.x, &rotation.y, &rotation.z, &scale.x, &scale.y, &scale.z);
		items[i].SetPosition(pos);
		items[i].SetRotate(rotation);
		items[i].SetScale(scale);

		items[i].SetBox();
	}

	Rat::ClearRat(rats);
	rats = 0;
	fscanf(f, "\n#RATS %d\n", &numOfRats);
	for (GLuint i = 0; i < numOfRats; i++){
		fscanf(f, "%*d.\nSCORE %d\n", &ratScore);
		fscanf(f, "POSITION %f,%f,%f\n", &pos.x, &pos.y, &pos.z);
		fscanf(f, "SCALE %f,%f,%f\n", &scale.x, &scale.y, &scale.z);
		fscanf(f, "MOVE %f,%f,%f\n", &move.x, &move.y, &move.z);
		fscanf(f, "MINPOS %f,%f,%f\n", &minpos.x, &minpos.y, &minpos.z);
		fscanf(f, "MAXPOS %f,%f,%f\n", &maxpos.x, &maxpos.y, &maxpos.z);

		Rat *rat = new Rat();
		rat->SetShader(RESOURCES->GetShaderByID("Basic"));
		rat->SetNumOfModels(3);
		rat->SetModel(RESOURCES->GetModelByID("Rat"), 0);
		rat->SetModel(RESOURCES->GetModelByID("Rat1"), 1);
		rat->SetModel(RESOURCES->GetModelByID("Rat2"), 2);
		rat->SetPosition(pos);
		rat->SetScore(ratScore);
		rat->SetScale(scale);
		rat->setMove(move);
		rat->setMinPosition(minpos);
		rat->setMaxPosition(maxpos);
		rat->SetBox();
		Rat::AddRat(rats, rat);
	}
}

void GamePlayScene::SetNumOfLevel(GLuint numOfLevel){
	this->numOfLevel = numOfLevel;
}

void GamePlayScene::SetLevelsForder(char *levelsFolder){
	if (this->levelsFolder){
		delete[] this->levelsFolder;
	}
	this->levelsFolder = new char[strlen(levelsFolder) + 1];
	strcpy(this->levelsFolder, levelsFolder);
}

int GamePlayScene::NextLevel(){
	if (currentLevel < numOfLevel){
		currentLevel++;
		char path[128];
		sprintf(path, "%s/Level%d.txt", levelsFolder ,currentLevel);
		LoadLevel(path);
		if (giftBox){
			delete giftBox;
			giftBox = 0;
		}
		timeShowGiftBox = 10.0f + (timeGame - 10.0f)*((float)rand() / RAND_MAX);

		camera3D->SetPosition(Vector3(0.024f, -6.581f, 19.557f));
		cat->SetPosition(Vector3(0.0f, -10.0f, 15.0f));
		cat->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
		cat->SetUsingModel(0);
		cat->SetBox();
		CreateEgg();
		flag = NEXT;
		
		return 1;
	}
	else{
		printf("Out of Levels\n");
		return 0;
	}
}