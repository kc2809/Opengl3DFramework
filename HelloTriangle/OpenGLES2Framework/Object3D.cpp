#include "Object3D.h"


Object3D::Object3D()
{
	models = 0;
	textures = 0;
	cubeTextures = 0;
	numOfModels = 0;
	numOfTextures = 0;
	numOfCubeTextures = 0;
	usingModel = 0;
}


Object3D::~Object3D()
{
	if (models){
		delete[] models;
	}
	if (textures){
		delete[] textures;
	}
	if (cubeTextures){
		delete[] cubeTextures;
	}
}

void Object3D::SetModel(Model *model, GLuint offset)
{
	if (offset < numOfModels){
		this->models[offset] = model;
	}
}

void Object3D::SetNumOfModels(GLuint numberModels){
	numOfModels = numberModels;
	
	if (numOfModels > 0){
		if (models){
			delete[]models;
			models = 0;
		}
		models = new Model*[numOfModels];
	}
}

void Object3D::SetUsingModel(GLuint usingModel){
	this->usingModel = usingModel;
}

void Object3D::SetBox(){
	minVertex.x = models[usingModel]->GetMinVertex().x*scale.x + position.x;
	minVertex.y = models[usingModel]->GetMinVertex().y*scale.y + position.y;
	minVertex.z = models[usingModel]->GetMinVertex().z*scale.z + position.z;
	maxVertex.x = models[usingModel]->GetMaxVertex().x*scale.x + position.x;
	maxVertex.y = models[usingModel]->GetMaxVertex().y*scale.y + position.y;
	maxVertex.z = models[usingModel]->GetMaxVertex().z*scale.z + position.z;
}

void Object3D::SetNumOfTextures(GLuint numberTexture)
{
	numOfTextures = numberTexture;
	if (textures){
		delete[] textures;
	}
	if (numOfTextures > 0){
		textures = new Texture*[numOfTextures];
	}
}

void Object3D::SetTexture(Texture *texture, GLuint offset)
{
	if (offset < numOfTextures){
		this->textures[offset] = texture;
	}
}

void Object3D::SetNumOfCubeTextures(GLuint numberCubeTexture)
{
	numOfCubeTextures = numberCubeTexture;
	if (cubeTextures)
	{
		delete[] cubeTextures;
	}
	if (numberCubeTexture > 0)
	{
		cubeTextures = new Texture*[numberCubeTexture];
	}
}

void Object3D::SetCubeTexture(Texture *cubeTexture, GLuint offset)
{
	if (offset < numOfCubeTextures)
	{
		this->cubeTextures[offset] = cubeTexture;
	}
}

void Object3D::Draw(Matrix &viewMatrix){

	worldMatrix = viewMatrix * CalculateWorldmatrix();

	shader->EnableState();

	glUseProgram(shader->program);

	Mesh *meshs = models[usingModel]->GetMeshs();
	GLuint nMesh = models[usingModel]->GetNMesh();
	for (GLuint i = 0; i < nMesh; i++){

		glBindBuffer(GL_ARRAY_BUFFER, meshs[i].vboId);
		if (!models[usingModel]->IsDrawArray()){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshs[i].iboId);
		}

		//finding location of uniforms / attributes
		GLint a_position = glGetAttribLocation(shader->program, "a_position");
		GLint a_uv = glGetAttribLocation(shader->program, "a_uv");
		GLint u_W = glGetUniformLocation(shader->program, "u_W");
		GLint u_texture = glGetUniformLocation(shader->program, "u_texture");
		GLint u_s_texture = glGetUniformLocation(shader->program, "u_s_texture");
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


		if (u_s_texture != -1){
			for (GLuint i = 0; i < numOfCubeTextures; i++){
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextures[i]->GetIdTexture());

				glUniform1i(u_s_texture, 0);
			}
		}
		//request draw 
		if (models[usingModel]->IsDrawArray()){
			if (u_texture != -1){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, meshs[i].texture.GetIdTexture());

				glUniform1i(u_texture, 0);
			}

			glDrawArrays(GL_TRIANGLES, 0, meshs[i].nVertices);
		}
		else{
			if (u_texture != -1){
				for (GLuint j = 0; j < numOfTextures; j++){
					glActiveTexture(GL_TEXTURE0 + j);
					glBindTexture(GL_TEXTURE_2D, textures[j]->GetIdTexture());

					glUniform1i(u_texture, 0);
				}
			}

			glDrawElements(GL_TRIANGLES, meshs[i].nIndice, GL_UNSIGNED_SHORT, 0);
		}
		//
		//unbind buffer

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	shader->DisableState();
}

void Object3D::Update(GLfloat deltaTime){

}

bool Object3D::IsCollision(Object3D *object3d){
	if (maxVertex.x >= object3d->minVertex.x && minVertex.x <= object3d->maxVertex.x
		&& maxVertex.y >= object3d->minVertex.y && minVertex.y <= object3d->maxVertex.y
		&& maxVertex.z >= object3d->minVertex.z && minVertex.z <= object3d->maxVertex.z)
	{
		printf("\nCenter: %f %f %f in", minVertex.x, minVertex.y, minVertex.z);
		printf("\nBox: %f %f %f - %f %f %f\n", object3d->minVertex.x, object3d->minVertex.y, object3d->minVertex.z,
			object3d->maxVertex.x, object3d->maxVertex.y, object3d->maxVertex.z);
		return true;
	}
	return false;
}

bool Object3D::IsInObject(Object3D *object3d){
	if (minVertex.x >= object3d->minVertex.x && maxVertex.x <= object3d->maxVertex.x
		&& minVertex.y >= object3d->minVertex.y && maxVertex.y <= object3d->maxVertex.y
		&& minVertex.z >= object3d->minVertex.z && maxVertex.z <= object3d->maxVertex.z)
	{
		return true;
	}
	return false;
}

void Object3D::printBox(){
	printf("\nBox: %f %f %f - %f %f %f\n", minVertex.x, minVertex.y, minVertex.z,
		maxVertex.x, maxVertex.y, maxVertex.z);
}

Vector3 Object3D::GetMinVertex(){
	return minVertex;
}

Vector3 Object3D::GetMaxVertex(){
	return maxVertex;
}