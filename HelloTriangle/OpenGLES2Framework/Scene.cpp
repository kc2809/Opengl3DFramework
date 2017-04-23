#include "Scene.h"


Scene::Scene()
{
	numOfObject2Ds = 0;
	object2Ds = 0;
	numOfButtons = 0;
	buttons = 0;
	numOfObject3Ds = 0;
	object3Ds = 0;
	camera2D = 0;
	camera3D = 0;

	soundId = S_NONE;
}


Scene::~Scene()
{
	if (buttons){
		delete[] buttons;
		buttons = 0;
	}
	if (object3Ds){
		delete[] object3Ds;
		object3Ds = 0;
	}
	if (object2Ds){
		delete[] object2Ds;
		object2Ds = 0;
	}
	if (camera3D)
	{
		delete camera3D;
		camera3D = 0;
	}
	if (camera2D)
	{
		delete camera2D;
		camera2D = 0;
	}
}


void Scene::Init(char *fileName)
{
	FILE *file;
	float x, y, z; // to read by fscanf
	GLuint t;
	char buffer[128];

	file = fopen(fileName, "r");

	if (file)
	{
		//	LOAD OBJECT3DS
		fscanf(file, "#Objects3D %d\n", &numOfObject3Ds);
		if (numOfObject3Ds>0){
			if (object3Ds)
			{
				delete[] object3Ds;
				object3Ds = 0;
			}
			object3Ds = new Object3D[numOfObject3Ds];
		}

		for (GLuint i = 0; i < numOfObject3Ds; i++)
		{
			// Load name
			fscanf(file, "\nNAME %s\n", buffer);
			object3Ds[i].SetName(buffer);

			//Load model
			fscanf(file, "MODELS %u\n", &t);
			object3Ds[i].SetNumOfModels(t);
			for (GLuint j = 0; j < t; j++)
			{
				fscanf(file, "MODEL %s\n", &buffer);
				object3Ds[i].SetModel(RESOURCES->GetModelByID(buffer), j);
			}

			// Load 2d texture
			fscanf(file, "2DTEXTURE %u\n", &t);
			object3Ds[i].SetNumOfTextures(t);
			for (GLuint j = 0; j < t; j++)
			{
				fscanf(file, "TEXTURE %s\n", buffer);
				object3Ds[i].SetTexture(RESOURCES->GetTextureByID(buffer), j);
			}
			// Load cube texture
			fscanf(file, "CUBETEXTURES %u\n", &t);
			object3Ds[i].SetNumOfCubeTextures(t);
			for (GLuint j = 0; j < t; j++)
			{
				fscanf(file, "CUBETEXTURE %s\n", buffer);
				object3Ds[i].SetCubeTexture(RESOURCES->GetCubeTextureByID(buffer), j);
			}

			// Load shader
			fscanf(file, "SHADER %s\n", buffer);
			object3Ds[i].SetShader(RESOURCES->GetShaderByID(buffer));

			// Other
			fscanf(file, "POSITION %f,%f,%f\n", &x, &y, &z);
			object3Ds[i].SetPosition(Vector3(x, y, z));
			fscanf(file, "ROTATION %f,%f,%f\n", &x, &y, &z);
			object3Ds[i].SetRotate(Vector3(x, y, z));
			fscanf(file, "SCALE %f,%f,%f\n", &x, &y, &z);
			object3Ds[i].SetScale(Vector3(x, y, z));
			object3Ds[i].SetBox();
			//object3Ds[i].printBox();
		}

		// LOAD OBJECT2DS
		fscanf(file, "#Objects2D %d\n", &numOfObject2Ds);
		if (numOfObject2Ds > 0){
			if (object2Ds)
			{
				delete[] object2Ds;
				object2Ds = 0;
			}
			object2Ds = new Object2D[numOfObject2Ds];
		}
		for (GLuint i = 0; i < numOfObject2Ds; i++)
		{
			// Set name
			int sx, sy;
			fscanf(file, "\nNAME %s\n", buffer);
			object2Ds[i].SetName(buffer);

			// Load texture
			GLuint numOfTextures;
			fscanf(file, "TEXTURES %u\n", &numOfTextures);
			Texture **tempTextures = new Texture*[numOfTextures];
			for (GLuint j = 0; j < numOfTextures; j++)
			{
				fscanf(file, "TEXTURE %s\n", buffer);
				tempTextures[j] = RESOURCES->GetTextureByID(buffer);
			}
			object2Ds[i].SetTexture(tempTextures, numOfTextures);
			object2Ds[i].Init();

			// Load shader
			fscanf(file, "SHADER %s\n", buffer);
			object2Ds[i].SetShader(RESOURCES->GetShaderByID(buffer));

			// Other
			fscanf(file, "POSITION %f,%f,%f\n", &x, &y, &z);
			object2Ds[i].SetPosition(Vector3(x, y, z));
			fscanf(file, "ROTATION %f\n", &z);
			object2Ds[i].SetRotate(Vector3(0.0f, 0.0f, z));
			fscanf(file, "SCALE %f,%f\n", &x, &y);
			object2Ds[i].SetScale(Vector3(x, y, 0.0f));
		}

		//	LOAD BUTTONS
		fscanf(file, "#BUTTONS %d\n", &numOfButtons);
		if (numOfButtons > 0){
			if (buttons)
			{
				delete[] buttons;
				buttons = 0;
			}
			buttons = new Button[numOfButtons];
		}

		for (GLuint i = 0; i < numOfButtons; i++)
		{
			// Set name
			int sx, sy;
			fscanf(file, "\nNAME %s\n", buffer);
			buttons[i].SetName(buffer);

			// Load textures
			GLuint numOfTextures;
			fscanf(file, "TEXTURES %u\n", &numOfTextures);
			Texture **tempTextures = new Texture*[numOfTextures];
			for (GLuint j = 0; j < numOfTextures; j++)
			{
				fscanf(file, "TEXTURE %s\n", buffer);
				tempTextures[j] = RESOURCES->GetTextureByID(buffer);
			}
			buttons[i].SetTexture(tempTextures, numOfTextures);
			buttons[i].Init();

			// Load shader
			fscanf(file, "SHADER %s\n", buffer);
			buttons[i].SetShader(RESOURCES->GetShaderByID(buffer));

			// Other
			fscanf(file, "POSITION %f,%f,%f\n", &x, &y, &z);
			buttons[i].SetPosition(Vector3(x, y, z));
			fscanf(file, "ROTATION %f\n", &z);
			buttons[i].SetRotate(Vector3(0.0f, 0.0f, z));
			fscanf(file, "SCALE %f,%f\n", &x, &y);
			buttons[i].SetScale(Vector3(x, y, 0.0f));
		}

		// Load Camera 3D
		float fov, _near, _far, speed;
		fscanf(file, "\n#CAMERA3D\nNEAR %f\nFAR %f\nFOV %f\nSPEED %f\n", &_near, &_far, &fov, &speed);
		camera3D = new Camera;
		camera3D->Init3D(fov, (float)INPUT->GetScreenWidth() / INPUT->GetScreenHeight(), _near, _far, speed);

		// Load Camera 2D
		fscanf(file, "\n#CAMERA2D\nNEAR %f\nFAR %f\nSPEED %f\n", &_near, &_far, &speed);
		camera2D = new Camera;
		camera2D->Init2D(-INPUT->GetScreenWidth() / 2.0f, INPUT->GetScreenWidth() / 2.0f, -INPUT->GetScreenHeight() / 2.0f, INPUT->GetScreenHeight() / 2.0f, _near, _far, speed);

		fclose(file);
		printf("[SUCCESS] Can't load file %s\n", fileName);
	}
	else
	{
		printf("[ERROR] Can't load file %s\n", fileName);
	}
}


void Scene::Draw(){
	
	for (unsigned int i = 0; i < numOfObject3Ds; i++)
	{
		object3Ds[i].Draw(camera3D->GetViewMatrix());
	}
	for (unsigned int i = 0; i < numOfObject2Ds; i++)
	{
		object2Ds[i].Draw(camera2D->GetViewMatrix());
	}
	for (unsigned int i = 0; i < numOfButtons; i++)
	{
		buttons[i].Draw(camera2D->GetViewMatrix());
	}
}


void Scene::Update()
{
}


GLint Scene::GetSoundId() const {
	return soundId;
}


void Scene::SetSoundId(GLint soundId) {
	Scene::soundId = soundId;
}