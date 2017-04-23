#pragma once
#include "Object.h"
#define RESOURCES ResourceManager::GetInstance()

class ResourceManager
{
private:
	GLuint nModel, nTexture , nCubeTexture, nShader;
	Model *models;
	Texture **textures, *cubeTextures, *font;
	Shader *shaders;

	static ResourceManager *resourceManager;

	ResourceManager();
	~ResourceManager();
public:
	static ResourceManager *GetInstance();
	static void DestroyInstance();

	void Load(char *filename);
	void Write(char *filename);
	void LoadTexture2D(char *fileName);
	Texture *ReadTexture(char *bigTGABuffer, GLint bigWidth, GLint bigHeight, GLint bpp, char *id, GLint x, GLint y, GLint width, GLint height);
	Model *GetModelByID(char *ID);
	Texture *GetTextureByID(char *ID);
	Texture *GetFont();
	Texture *GetCubeTextureByID(char *ID);
	Shader *GetShaderByID(char *ID);
};

