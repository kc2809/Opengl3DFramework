#pragma once
#include "Object2D.h"
#include "Camera.h"
#define TEXTG Text::GetInstance()

class Text :
	public Object2D
{
public:
	Text();
	~Text();
	
	static Text* GetInstance();
	static void FreeInstance();

	void Init();
	void Update(float);
	void Draw(Matrix &viewMatrix);
	void DrawText(const char* text, float x, float y, Matrix &viewMatrix);
	void SetScale(Vector2 scale);
	void SetTexture(Texture *);
	void SetShader(Shader *);

	void DrawTextCenter(const char* text, float x, float y, Matrix &viewMatrix);
private:

	static Text *instance;

	GLint numIndices;
	int numPart, partId;
	int widthUnit, heightUnit;
	GLuint *vboId, iboId;

	Texture *texture;
	Shader* shader;
};

