#include "ResourceManager.h"

ResourceManager *ResourceManager::resourceManager = 0;

ResourceManager::ResourceManager()
{
	models = 0;
	textures = 0;
	cubeTextures = 0;
	shaders = 0;
}


ResourceManager::~ResourceManager()
{
	if (models)
	{
		delete[] models;
	}
	if (textures)
	{
		for (GLuint i = 0; i < nTexture; i++)
		{
			delete textures[i];
			textures[i] = 0;
		}
		delete textures;
		textures = 0;
	}

	if (font)
	{
		delete font;
		font = 0;
	}
	if (cubeTextures)
	{
		delete[] cubeTextures;
	}
	if (shaders)
	{
		delete[] shaders;
	}
}

ResourceManager *ResourceManager::GetInstance()
{
	if (resourceManager == 0)
	{
		resourceManager = new ResourceManager;
	}
	return resourceManager;
}

void ResourceManager::DestroyInstance()
{
	if (resourceManager != 0)
	{
		delete resourceManager;
		resourceManager = 0;
	}
}

void ResourceManager::Load(char *filename)
{
	FILE *f;

	char bufferString[60], bufferString2[60];
	f = fopen(filename, "r");
	if (f != 0)
	{
		// Load Models
		fscanf(f, "#MODELS %u\n", &nModel);
		if (nModel > 0)
		{
			if (models != 0)
			{
				delete[] models;
			}
			models = new Model[nModel];
		}
		for (GLuint i = 0; i < nModel; i++)
		{
			fscanf(f, "ID %s\nFILE %s\n",bufferString, bufferString2);
			models[i].SetId(bufferString);
			if (strcmp(bufferString2 + (strlen(bufferString2) - 4 ), ".obj") == 0){
				models[i].LoadObj(bufferString2);
			}
			else{
				models[i].InitModel(bufferString2);
			}
		}

		// Load 2D textures
		fscanf(f, "\n#2DTEXTURES\nFILE %s\n", bufferString);
		LoadTexture2D(bufferString);

		// Load Shaders
		fscanf(f, "\n#SHADERS %u\n", &nShader);
		if (nShader > 0)
		{
			if (shaders != 0)
			{
				delete[] shaders;
			}
			shaders = new Shader[nShader];
		}
		for (GLuint i = 0; i < nShader; i++)
		{
			GLuint nState;
			fscanf(f, "ID %s\n", bufferString);
			shaders[i].SetId(bufferString);
			fscanf(f, "VS %s\nFS %s\nStates %u\n", bufferString, bufferString2, &nState);

			shaders[i].Init(bufferString, bufferString2);

			for (GLuint j = 0; j < nState; j++)
			{
				fscanf(f, "STATE %s\n", bufferString);
				if (strcmp(bufferString, "SCISSOR_TEST") == 0){
					shaders[i].AddState(GL_SCISSOR_TEST);
				}
				else if (strcmp(bufferString, "STENCIL_TEST") == 0){
					shaders[i].AddState(GL_STENCIL_TEST);
				}
				else if (strcmp(bufferString, "DEPTH_TEST") == 0){
					shaders[i].AddState(GL_DEPTH_TEST);
				}
				else if (strcmp(bufferString, "CULLING") == 0){
					shaders[i].AddState(GL_CULL_FACE);
				}
				else if (strcmp(bufferString, "ALPHA") == 0){
					shaders[i].AddState(GL_BLEND);
				}
			}

		}
		fclose(f);
	}
}


void ResourceManager::LoadTexture2D(char *fileName)
{
	char buffer[128];
	FILE *file = fopen(fileName, "r");
	if (file)
	{
		//LOAD TEXTURES
		fscanf(file, "2DTEXTURES: %s\n", buffer);
		char *fileTGA = new char[strlen(fileName) + strlen(buffer) + 1];
		strcpy(fileTGA, fileName);
		GLuint len = strlen(fileTGA);
		while (len)
		{
			if (fileTGA[len] == '/') break;
			len--;
		}
		strcpy(fileTGA + len + 1, buffer);
		GLint bigWidth, bigHeight, bpp;
		char *tempBigBuffer = LoadTGA(fileTGA, bigWidth, bigHeight, bpp);

		fscanf(file, "\nnum: %d\n", &nTexture);
		textures = new Texture*[nTexture];
		for (GLuint i = 0; i < nTexture; i++)
		{
			GLuint x, y, width, height;
			fscanf(file, "\n%*d. %s\n", buffer);
			fscanf(file, "%d, %d\n%d, %d\n", &x, &y, &width, &height);
			textures[i] = ReadTexture(tempBigBuffer, bigWidth, bigHeight, bpp, buffer, x, y, width, height);
		}
		delete[]tempBigBuffer;
		delete[]fileTGA;

		//LOAD CUBETEXTURES
		fscanf(file, "\nCUBETEXTURES %u\n", &nCubeTexture);
		if (nCubeTexture > 0)
		{
			if (cubeTextures)
			{
				delete[] cubeTextures;
			}
			cubeTextures = new Texture[nCubeTexture];
		}
		for (GLuint i = 0; i < nCubeTexture; i++)
		{
			fscanf(file, "\n%*d. %s\n", buffer);
			cubeTextures[i].SetId(buffer);
			fscanf(file, "%s\n", buffer);
			fileTGA = new char[strlen(fileName) + strlen(buffer) + 1];
			strcpy(fileTGA, fileName);
			len = strlen(fileTGA);
			while (len)
			{
				if (fileTGA[len] == '/') break;
				len--;
			}
			strcpy(fileTGA + len + 1, buffer);
			fscanf(file, "tiling %s\n", buffer);
			if (strcmp(buffer, "REPEAT") == 0)
			{
				cubeTextures[i].InitCubeTexture(fileTGA, REPEAT);
			}
			else
			{
				cubeTextures[i].InitCubeTexture(fileTGA, CLAMP_TO_EDGE);
			}

			delete[]fileTGA;
		}

		//LOAD FONT
		fscanf(file, "\nFONT: %s\n", &buffer);
		fileTGA = new char[strlen(fileName) + strlen(buffer) + 1];
		strcpy(fileTGA, fileName);
		len = strlen(fileTGA);
		while (len)
		{
			if (fileTGA[len] == '/') break;
			len--;
		}
		strcpy(fileTGA + len + 1, buffer);
		font = new Texture;
		font->Init2DTextures(fileTGA, REPEAT);
		delete[]fileTGA;
	}
	else
	{

	}
}


Texture *ResourceManager::GetFont()
{
	return font;
}


Texture *ResourceManager::ReadTexture(char *bigTGABuffer, GLint bigWidth, GLint bigHeight, GLint bpp, char *id, GLint x, GLint y, GLint width, GLint height)
{
	Texture *temp= new Texture;
	temp->SetId(id);
	bigWidth *= bpp / 8;
	width *= bpp / 8;
	x *= bpp / 8;
	y = bigHeight - y- height;
	GLuint len = width*height;
	char *tempBuffer = new char[len];
	for (GLuint i = 0; i < height; i++)
	{
		for (GLuint j = 0; j < width; j++)
		{
			tempBuffer[i*width + j] = bigTGABuffer[(y + i)*bigWidth + (x + j)];
		}
	}

	temp->Init(tempBuffer, REPEAT, width/(bpp/8), height, bpp);
	delete[]tempBuffer;

	return temp;
}


Model *ResourceManager::GetModelByID(char *ID)
{
	for (unsigned i = 0; i < nModel; i++){
		if (strcmp(models[i].GetId(), ID) == 0)
		{
			return models + i;
		}
	}
	printf("Unable to retrieve the id of the model %s\n", ID);
	return 0;
}

Texture *ResourceManager::GetTextureByID(char *ID)
{
	for (unsigned i = 0; i < nTexture; i++){
		if (strcmp(textures[i]->GetId(), ID) == 0)
		{
			return textures[i];
		}
	}
	printf("Unable to retrieve the id of the texture %s\n", ID);
	return 0;
}

Texture *ResourceManager::GetCubeTextureByID(char *ID)
{
	for (unsigned i = 0; i < nCubeTexture; i++){
		if (strcmp(cubeTextures[i].GetId(), ID) == 0)
		{
			return cubeTextures + i;
		}
	}
	printf("Unable to retrieve the id of the cubetexture %s\n" , ID);
	return 0;
}

Shader *ResourceManager::GetShaderByID(char *ID)
{
	for (unsigned i = 0; i < nShader; i++){
		if (strcmp(shaders[i].GetId(), ID) == 0)
		{
			return shaders + i;
		}
	}
	printf("Unable to retrieve the id of the cubetexture %s\n", ID);
	return 0;
}