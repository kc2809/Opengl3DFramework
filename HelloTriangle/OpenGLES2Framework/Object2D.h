#pragma once
#include "Object.h"

class Object2D : public Object
{
protected:
	GLuint vboId, iboId;
	int width, height;
	Texture **textures;
	GLuint numOfTextures, usingTexture;
public:
	Object2D();
	~Object2D();

	void SetTexture(Texture **textures, GLuint numOfTextures);
	void SetSize(int width, int height);
	GLuint GetWidth();
	GLuint GetHeight();
	void SetUsingTexture(GLuint offset);
	void SetScale(Vector3 scale);
	void SetPosition(Vector3 translation);
	GLuint &GetUsingTexture();
	virtual void Init();
	virtual void Draw(Matrix &viewMatrix);
	virtual void Update(GLfloat deltaTime);
	
};

