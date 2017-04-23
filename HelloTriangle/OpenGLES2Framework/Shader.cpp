
#include "Shader.h"

Shader::Shader()
{
	states.clear();
	id = 0;
}

void Shader::SetId(char *id)
{
	if (this->id)
	{
		delete[] this->id;
	}
	this->id = new char[strlen(id) + 1];
	strcpy(this->id, id);
}

char *Shader::GetId()
{
	return id;
}

int Shader::Init(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	vertexShader = LoadShader(GL_VERTEX_SHADER, vertexShaderPath);
	if ( vertexShader == 0 )
	{
		printf("\nLoading %s fail", vertexShaderPath);
		return -1;
	}
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	if ( fragmentShader == 0 )
	{
		printf("\nLoading %s fail", fragmentShaderPath);
		glDeleteShader( vertexShader );
		return -2;
	}
	program = LoadProgram(vertexShader, fragmentShader);	
	return true;
}
void Shader::AddState(GLenum state)
{
	states.push_back( state );
}
void Shader::EnableState()
{
	for( GLuint i = 0; i < states.size(); i++ )
	{
		glEnable( states[i] );
		if ( states[i] == GL_BLEND )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
	}
}
void Shader::DisableState()
{
	for( GLuint i = 0; i < states.size(); i++ )
	{
		glDisable( states[i] );
	}
}
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param Shaderrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint Shader::LoadShader ( GLenum type, const char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;
	
	FILE * pf = fopen( filename, "rb" );
	if (pf == 0)
	{
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * buffer = new char[size + 1];
	fread(buffer, sizeof(char), size, pf);
	buffer[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **)&buffer, NULL );
	delete [] buffer;
	buffer = NULL;
	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			printf ( "\nError compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}
	return shader;
}


GLuint Shader::LoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			printf ( "\nError linking program:\n%s\n", infoLog );            

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}
Shader::~Shader()
{
	if ( program )
	{
		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	if (id != 0)
	{
		delete[] id;
	}
}
