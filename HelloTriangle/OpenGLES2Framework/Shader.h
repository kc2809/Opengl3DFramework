#pragma once
#include <Utilities.h>
#include <vector>
class Shader 
{
private:
	char *id;
public:
	void SetId(char *id);
	char *GetId();

	GLuint program, vertexShader, fragmentShader;
	std::vector<GLenum> states;

	//Method
	Shader();
	GLuint LoadProgram ( GLuint vertexShader, GLuint fragmentShader ) ;
	GLuint LoadShader ( GLenum type, const char * filename ) ;
	int Init(const char* vertexShaderPath, const char* fragmentShaderPath);
	virtual ~Shader();
	void AddState(GLenum state);
	void EnableState();
	void DisableState();
};
