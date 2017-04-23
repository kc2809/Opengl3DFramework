#include "Text.h"

Text* Text::instance = 0;

Text::Text()
{
	vboId = 0;
	width = 0;
	height = 0;
}


Text::~Text()
{
	if (vboId){
		delete[] vboId;
	}
}

Text* Text::GetInstance(){
	if (instance == 0){
		instance = new Text();
		instance->Init();
	}
	return instance;
}

void Text::SetScale(Vector2 scale)
{
	width = (texture->GetWidth()*INPUT->GetRatioWidth() / widthUnit) *scale.x;
	height = (texture->GetHeight()*INPUT->GetRatioHeight() / heightUnit) *scale.y;
	this->scale.x = scale.x;
	this->scale.y = scale.y;
}


void Text::FreeInstance(){
	if (instance) {
		delete instance;
		instance = 0;
	}
}

void Text::Init(){

	partId = 0;
	widthUnit = 10;
	heightUnit = 10;
	int numVertex = 4;

	texture = RESOURCES->GetFont();
	shader = RESOURCES->GetShaderByID("Basic");
	width = texture->GetWidth()*INPUT->GetRatioWidth()/ widthUnit;
	height = texture->GetHeight()*INPUT->GetRatioHeight()/ heightUnit;
	numPart = widthUnit * heightUnit;

	
	position = Vector3(0.0f, 0.0f, 0.5f);
	rotate = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 0.0f);

	Vertex verticesData[4];
	verticesData[0].pos = Vector3(0.0, 0.0, 0.0f);
	verticesData[1].pos = Vector3((float)width, 0.0f, 0.0f);
	verticesData[2].pos = Vector3(0.0f, (float)height, 0.0f);
	verticesData[3].pos = Vector3((float)width, (float)height, 0.0f);

	vboId = new GLuint[numPart];
	for (register int id = 0; id < numPart; id++)
	{
		int i = id % widthUnit;
		int j = heightUnit - 1 - id / heightUnit;

		float dx = 1.0f / widthUnit;
		float dy = 1.0f / heightUnit;

		verticesData[0].uv = Vector2(dx * i, dy * j);
		verticesData[1].uv = Vector2(dx * (i + 1), dy * j);
		verticesData[2].uv = Vector2(dx * i, dy * (j + 1));
		verticesData[3].uv = Vector2(dx * (i + 1), dy * (j + 1));

		GLuint vboId;
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, numVertex*sizeof(Vertex), verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->vboId[id] = vboId;
	}
	//bind indices
	GLushort indice[6] = { 0, 1, 2, 1, 2, 3 };
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indice, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Text::SetTexture(Texture *texture)
{
	this->texture = texture;
}

void Text::SetShader(Shader *shader)
{
	this->shader = shader;
}
void Text::Update(float){}

void Text::DrawText(const char* text, float x, float y, Matrix &viewMatrix)
{
	for (unsigned int i = 0; i < strlen(text); i++)
	{
		partId = text[i] - 32;
		if (partId >= numPart) partId = 0;
		position.x = x*INPUT->GetRatioWidth() + width * (i);
		position.y = y*INPUT->GetRatioHeight();
		Draw(viewMatrix);
	}
}

void Text::Draw(Matrix &viewMatrix){
	shader->EnableState();

	worldMatrix = viewMatrix * CalculateWorldmatrix();

	glUseProgram(shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId[partId]);
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
	if (u_texture != -1){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetIdTexture());
		glUniform1i(u_texture, 0);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//
	//unbind buffer

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader->DisableState();
}

void Text::DrawTextCenter(const char* text, float x, float y, Matrix &viewMatrix){
	for (unsigned int i = 0; i < strlen(text); i++)
	{
		partId = text[i] - 32;
		if (partId >= numPart) partId = 0;
		position.x = x*INPUT->GetRatioWidth() + width * (i) - (width*(float)strlen(text) / 2);
		position.y = y*INPUT->GetRatioHeight() - height / 2;
		Draw(viewMatrix);
	}
}