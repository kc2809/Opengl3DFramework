#include "Object2D.h"


Object2D::Object2D()
{
	textures = 0;
	numOfTextures = 0;
	usingTexture = 0;
}


Object2D::~Object2D()
{
	if (textures)
	{
		delete[]textures;
	}
}

void Object2D::SetPosition(Vector3 position)
{
	this->position.x = position.x*INPUT->GetRatioWidth();
	this->position.y = position.y*INPUT->GetRatioHeight();
	this->position.z = position.z;
}

void Object2D::SetScale(Vector3 scale)
{
	this->scale.x =scale.x;
	this->scale.y = scale.y;
}

void Object2D::SetTexture(Texture **textures, GLuint numOfTextures)
{
	this->textures = textures;
	this->numOfTextures = numOfTextures;
	SetSize(textures[usingTexture]->GetWidth(), textures[usingTexture]->GetHeight());
}

void Object2D::SetSize(GLint width, GLint height){
	this->width = width;
	this->height = height;
}

GLuint Object2D::GetHeight()
{
	return height;
}

GLuint Object2D::GetWidth()
{
	return width;
}

void Object2D::SetUsingTexture(GLuint offset){
	usingTexture = (offset <= numOfTextures) ? offset : 0;
	SetSize(textures[usingTexture]->GetWidth(), textures[usingTexture]->GetHeight());
}

GLuint &Object2D::GetUsingTexture()
{
	return usingTexture;
}

void Object2D::Init(){

	Vertex verticesData[4];
	verticesData[0].pos = Vector3((float)width, (float)height, 0.0f);
	verticesData[1].pos = Vector3((float)width, 0.0f, 0.0f);
	verticesData[2].pos = Vector3(0.0f, 0.0f, 0.0f);
	verticesData[3].pos = Vector3(0.0f, (float)height, 0.0f);
	verticesData[0].uv = Vector2(1.0f, 1.0f);
	verticesData[1].uv = Vector2(1.0f, 0.0f);
	verticesData[2].uv = Vector2(0.0f, 0.0f);
	verticesData[3].uv = Vector2(0.0f, 1.0f);
	GLushort indice[6] = { 0, 1, 2, 0, 2, 3 };

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indice, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object2D::Update(float){
}

void Object2D::Draw(Matrix &viewMatrix){
	worldMatrix = viewMatrix * CalculateWorldmatrix();

	shader->EnableState();

	glUseProgram(shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//finding location of uniforms / attributes
	GLint a_position = glGetAttribLocation(shader->program, "a_position");
	GLint a_uv = glGetAttribLocation(shader->program, "a_uv");
	GLint u_W = glGetUniformLocation(shader->program, "u_W");
	GLint u_texture = glGetUniformLocation(shader->program, "u_texture");
	//pass data to vertex shader


	if (a_position != -1)
	{
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
	}

	if (a_uv != -1)
	{
		glEnableVertexAttribArray(a_uv);
		glVertexAttribPointer(a_uv, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	}

	if (u_W != -1){
		glUniformMatrix4fv(u_W, 1, GL_FALSE, (GLfloat*)&worldMatrix);
	}
	//request draw 
	if (u_texture != -1 && numOfTextures > 0){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[usingTexture]->GetIdTexture());

		glUniform1i(u_texture, 0);
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//
	//unbind buffer

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader->DisableState();
}