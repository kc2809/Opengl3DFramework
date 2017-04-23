#include "Texture.h"


Texture::Texture()
{
	id = 0;
}


Texture::~Texture()
{
	if (id)
	{
		delete[] id;
	}
}

void Texture::SetId(char *id)
{
	if (this->id)
	{
		delete[] this->id;
	}
	this->id = new char[strlen(id) + 1];
	strcpy(this->id, id);
}

char *Texture::GetId()
{
	return id;
}

void Texture::Init2DTextures(char *pFile, TILING tiling){
	glGenTextures(1, &m_hTexture);
	glBindTexture(GL_TEXTURE_2D, m_hTexture);

	int bdp;

	char *bufferTGA = LoadTGA(pFile, width, height, bdp);

	if (bdp == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
	}
	delete[] bufferTGA;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	if (tiling == REPEAT)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	printf("\nLoaded file %s", pFile);
}


void Texture::Init(char *bufferTGA, TILING tiling, GLuint width, GLuint height, GLint bpp)
{
	this->width = width*INPUT->GetRatioWidth();
	this->height = height*INPUT->GetRatioHeight();
	glGenTextures(1, &m_hTexture);
	glBindTexture(GL_TEXTURE_2D, m_hTexture);

	if (bpp == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	if (tiling == REPEAT)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}


GLuint Texture::GetIdTexture()
{
	return m_hTexture;
}

int Texture::GetWidth()
{
	return width;
}

int Texture::GetHeight()
{
	return height;
}

void Texture::InitCubeTexture(char *pFile, TILING tiling)
{
	glGenTextures(1, &m_hTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_hTexture);

	int bpp;

	char *bufferTGA = LoadTGA(pFile, width, height, bpp);

	if (bufferTGA == NULL) {
		printf("[ERROR] Fail to load %s", pFile);
		return;
	}

	char **colorBuffer = ExtractFace(bufferTGA, width, bpp);

	for (int i = 0; i < 6; i++)
	{
		
		if (bpp == 24)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width / 4, height / 3, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer[i]);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width / 4, height / 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer[i]);
		}
	}

	delete[] bufferTGA;
	for (int i = 0; i < 6; i++)
	{
		delete colorBuffer[i];
	}
	delete[] colorBuffer;

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	if (tiling == REPEAT)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	printf("\n[Sucessfully] Loaded file %s", pFile);
}

char **Texture::ExtractFace(char *bufferTGA, int width, int bpp)
{
	int w = (width / 4) * (bpp / 8), h = width / 4;

	char **colorBuffer = new char*[6];

	for (int i = 0; i < 6; i++)
	{
		colorBuffer[i] = new char[w*h];
	}
	for (int i = 0; i < h; i++)
	{
		//GL_TEXTURE_CUBE_MAP_POSITIVE_X
		memcpy(&colorBuffer[0][w*i], &bufferTGA[w*(width + 2 + 4 * i)], w); // 4*w = width * (bpp/8)
		// w*width = h * width * (bpp/8)
	}
	for (int i = 0; i < h; i++)
	{
		//GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		memcpy(&colorBuffer[1][w*i], &bufferTGA[w*(width + 4 * i)], w);
	}
	for (int i = 0; i < h; i++)
	{
		//GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		memcpy(&colorBuffer[2][w*i], &bufferTGA[w*(1 + 4 * i)], w);
	}
	for (int i = 0; i < h; i++)
	{
		//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		memcpy(&colorBuffer[3][w*i], &bufferTGA[w*(1 + 8 * h + 4 * i)], w); // w*8*h = (w*4)*2*h : Down 2 images.
	}
	for (int i = 0; i < h; i++)
	{
		//GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		memcpy(&colorBuffer[4][w*i], &bufferTGA[w*(width + 1 + 4 * i)], w);
	}
	for (int i = 0; i < h; i++)
	{
		//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		memcpy(&colorBuffer[5][w*i], &bufferTGA[w*(width + 3 + 4 * i)], w);
	}
	return colorBuffer;
}