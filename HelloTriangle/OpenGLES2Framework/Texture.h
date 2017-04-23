#pragma once
#include<Utilities.h>
#include "InputManager.h"

enum TILING{ REPEAT, CLAMP_TO_EDGE };

class Texture
{
private:
	GLuint m_hTexture;
	int width, height;
	
	char *id;
public:
	Texture();
	~Texture();

	void SetId(char *id);
	char *GetId();

	void Init2DTextures(char *pFile, TILING tiling);
	void Init(char *TGAbuffer, TILING tiling, GLuint width, GLuint height, GLint bpp);
	GLuint GetIdTexture();
	int GetWidth();
	int GetHeight();

	void InitCubeTexture(char *pFile, TILING tiling);
	char **ExtractFace(char *bufferTGA, int height, int bpp);
};

